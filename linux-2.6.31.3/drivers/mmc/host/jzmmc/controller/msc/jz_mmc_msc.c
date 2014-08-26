/*
 *  linux/drivers/mmc/host/jz_mmc/msc/jz_mmc_msc.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Copyright (c) Ingenic Semiconductor Co., Ltd.
 */

#include <linux/dma-mapping.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/sdio.h>
#include <linux/scatterlist.h>

#include <asm/jzsoc.h>
#include <asm/jzmmc/jz_mmc_msc.h>

#define MSC_STAT_ERR_BITS 0x3f

#if 1

static int jzmmc_trace_level = 0;
static int jzmmc_trace_cmd_code = -1;
static int jzmmc_trace_data_len = -1;
module_param(jzmmc_trace_level, int, 0644);
module_param(jzmmc_trace_cmd_code, int, 0644);
module_param(jzmmc_trace_data_len, int, 0644);

#define TRACE_CMD_REQ()							\
	({								\
		if (jzmmc_trace_level & 0x1)				\
			if ( (jzmmc_trace_cmd_code == -1) || (jzmmc_trace_cmd_code == cmd->opcode) ) \
				printk("%s:     execute_cmd: opcode = %d cmdat = %#0x arg = %#0x data_flags = %#0x\n", \
				       mmc_hostname(host->mmc), cmd->opcode, cmdat, cmdarg, \
				       host->curr.data ? host->curr.data->flags : 0); \
	})

#define TRACE_CMD_RES()							\
	({								\
		if (jzmmc_trace_level & 0x1)				\
			if ( (jzmmc_trace_cmd_code == -1) || (jzmmc_trace_cmd_code == cmd->opcode) ) \
				printk("%s:     cmd done: r_type = %d resp[0] = %#0x err = %d state = %#0x\n", \
				       mmc_hostname(host->mmc), host->curr.r_type, cmd->resp[0], cmd->error, \
				       REG_MSC_STAT(host->pdev_id));	\
	})

#define TRACE_DATA_REQ()						\
	({								\
		if ( (jzmmc_trace_level & 0x2) && host->curr.data ) {	\
			if ((jzmmc_trace_data_len == -1) ||		\
			    (jzmmc_trace_data_len == host->curr.data->blksz * host->curr.data->blocks) ) \
				printk("%s:     blksz %d blocks %d flags %08x "	\
				       "tsac %d ms nsac %d\n",		\
				       mmc_hostname(host->mmc), host->curr.data->blksz, \
				       host->curr.data->blocks, host->curr.data->flags, \
				       host->curr.data->timeout_ns / 1000000, \
				       host->curr.data->timeout_clks); \
		}							\
	})

#define TRACE_DATA_DONE()						\
	({								\
		if (jzmmc_trace_level & 0x2)				\
			if ((jzmmc_trace_data_len == -1) ||		\
			    (jzmmc_trace_data_len == data->blksz * data->blocks) ) \
				printk("%s:     stat = 0x%08x error = %d bytes_xfered = %d stop = %p\n", \
				       mmc_hostname(host->mmc), stat, data->error, \
				       data->bytes_xfered, host->curr.mrq->stop); \
	})
#else
#define TRACE_CMD_REQ() do {  } while(0)
#define TRACE_CMD_RES() do {  } while(0)
#define TRACE_DATA_REQ() do {  } while(0)
#define TRACE_DATA_DONE() do {  } while(0)
#endif

//#define DEBUG 1

#ifdef DEBUG
static void jz_mmc_dump_register(int msc_id) {
	printk("***** mmc%d registers *****\n",msc_id);
	printk("\tREG_MSC_STRPCL(%d) = 0x%08x\n", msc_id, REG_MSC_STRPCL(msc_id));
	printk("\tREG_MSC_STAT(%d) = 0x%08x\n", msc_id, REG_MSC_STAT(msc_id));
	printk("\tREG_MSC_CLKRT(%d) = 0x%08x\n", msc_id, REG_MSC_CLKRT(msc_id));
	printk("\tREG_MSC_CMDAT(%d) = 0x%08x\n", msc_id, REG_MSC_CMDAT(msc_id));
	printk("\tREG_MSC_RESTO(%d) = 0x%08x\n", msc_id, REG_MSC_RESTO(msc_id));
	printk("\tREG_MSC_RDTO(%d) = 0x%08x\n", msc_id, REG_MSC_RDTO(msc_id));
	printk("\tREG_MSC_BLKLEN(%d) = 0x%08x\n", msc_id, REG_MSC_BLKLEN(msc_id));
	printk("\tREG_MSC_NOB(%d) = 0x%08x\n", msc_id, REG_MSC_NOB(msc_id));
	printk("\tREG_MSC_SNOB(%d) = 0x%08x\n", msc_id, REG_MSC_SNOB(msc_id));
	printk("\tREG_MSC_IMASK(%d) = 0x%08x\n", msc_id, REG_MSC_IMASK(msc_id));
	printk("\tREG_MSC_IREG(%d) = 0x%08x\n", msc_id, REG_MSC_IREG(msc_id));
	printk("\tREG_MSC_CMD(%d) = 0x%08x\n", msc_id, REG_MSC_CMD(msc_id));
	printk("\tREG_MSC_ARG(%d) = 0x%08x\n", msc_id, REG_MSC_ARG(msc_id));
	printk("\tREG_MSC_RES(%d) = 0x%08x\n", msc_id, REG_MSC_RES(msc_id));
	printk("\tREG_MSC_RXFIFO(%d) = 0x%08x\n", msc_id, REG_MSC_RXFIFO(msc_id));
	printk("\tREG_MSC_TXFIFO(%d) = 0x%08x\n", msc_id, REG_MSC_TXFIFO(msc_id));
	printk("\tREG_MSC_LPM(%d) = 0x%08x\n", msc_id, REG_MSC_LPM(msc_id));
}

static struct timer_list debug_timer;
#define DEBUG_INTERVAL HZ/2

static void debug_func(unsigned long arg) {
	int msc_id = (int)arg;
	jz_mmc_dump_register(msc_id);

	debug_timer.expires += DEBUG_INTERVAL;

	add_timer(&debug_timer);
}

static void start_debug_timer(int msc_id) {
	static int dbg_tm_need_init = 1;

	if (dbg_tm_need_init) {
		dbg_tm_need_init = 0;
		init_timer(&debug_timer);
		debug_timer.function = debug_func;
	}

	debug_timer.expires = jiffies + DEBUG_INTERVAL;
	debug_timer.data = (unsigned long)msc_id;

	add_timer(&debug_timer);
}

static void stop_debug_timer(int msc_id __attribute((unused))) {
	del_timer_sync(&debug_timer);
}

#endif

void jz_mmc_set_clock(struct jz_mmc_host *host, int rate);
static int jz_mmc_data_done(struct jz_mmc_host *host);

static void msc_irq_mask_all(int msc_id)
{
	REG_MSC_IMASK(msc_id) = 0xffff;
	REG_MSC_IREG(msc_id) = 0xffff;
}

static void jz_mmc_reset(struct jz_mmc_host *host)
{
	REG_MSC_STRPCL(host->pdev_id) = MSC_STRPCL_RESET;
 	while (REG_MSC_STAT(host->pdev_id) & MSC_STAT_IS_RESETTING);
}

static inline int msc_calc_clkrt(int is_low, u32 rate)
{
	u32 clkrt;
	u32 clk_src = is_low ? 24000000 : 48000000;

	clkrt = 0;
	while (rate < clk_src) {
		clkrt++;
		clk_src >>= 1;
	}
	return clkrt;
}

void jz_mmc_set_clock(struct jz_mmc_host *host, int rate)
{
	int clkrt;

	/* __cpm_select_msc_clk_high will select 48M clock for MMC/SD card
	 * perhaps this will made some card with bad quality init fail,or
	 * bad stabilization.
	*/

	// Cause there is only ONE devider in CPM, the clock must only <= 24MHz
#if !defined(CONFIG_SOC_JZ4750) && !defined(CONFIG_SOC_JZ4750D)
#if 0
	if (rate > SD_CLOCK_FAST) {
		cpm_set_clock(CGU_MSCCLK, 48 * 1000 * 1000);
		clkrt = msc_calc_clkrt(0, rate);
	} else {
		cpm_set_clock(CGU_MSCCLK, 24 * 1000 * 1000);
		clkrt = msc_calc_clkrt(1, rate);
	}
#else
	if (rate > SD_CLOCK_FAST) {
		rate = SD_CLOCK_FAST;
		cpm_set_clock(CGU_MSCCLK, 24 * 1000 * 1000);
		clkrt = msc_calc_clkrt(1, rate);
	} else {
		cpm_set_clock(CGU_MSCCLK, 24 * 1000 * 1000);
		clkrt = msc_calc_clkrt(1, rate);
	}
#endif
	REG_MSC_CLKRT(host->pdev_id) = clkrt;
#else
		/* __cpm_select_msc_clk_high will select 48M clock for MMC/SD card
	 * perhaps this will made some card with bad quality init fail,or
	 * bad stabilization.
	*/
	if (rate > SD_CLOCK_FAST) {
		rate = SD_CLOCK_FAST;
		__cpm_select_msc_clk_high(host->pdev_id,1);	/* select clock source from CPM */

		//		__cpm_select_msc_clk(host->pdev_id,1);	/* select clock source from CPM */
		clkrt = msc_calc_clkrt(0, rate);
	} else {
		__cpm_select_msc_clk(host->pdev_id,1);	/* select clock source from CPM */
		clkrt = msc_calc_clkrt(1, rate);
	}

	// printk("clock rate = %d\n", __cpm_get_mscclk(0));
	REG_MSC_CLKRT(host->pdev_id) = clkrt;
#endif
}

static void jz_mmc_enable_irq(struct jz_mmc_host *host, unsigned int mask)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	host->imask &= ~mask;
	REG_MSC_IMASK(host->pdev_id) = host->imask;
	spin_unlock_irqrestore(&host->lock, flags);
}

static void jz_mmc_disable_irq(struct jz_mmc_host *host, unsigned int mask)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	host->imask |= mask;
	REG_MSC_IMASK(host->pdev_id) = host->imask;
	spin_unlock_irqrestore(&host->lock, flags);
}

static void jz_mmc_finish_request(struct jz_mmc_host *host, struct mmc_request *mrq)
{
	host->curr.mrq = NULL;
	host->curr.cmd = NULL;
	host->curr.data = NULL;
	mmc_request_done(host->mmc, mrq);
}

static int jz_mmc_cmd_done(struct jz_mmc_host *host, unsigned int stat)
{
	struct mmc_command *cmd = host->curr.cmd;
	int i, temp[16] = {0};
	unsigned char *buf;
	unsigned int data, v, w1, w2;

	if (!cmd)
		return 0;

	host->curr.cmd = NULL;
	buf = (u8 *) temp;
	switch (host->curr.r_type) {
	case 1:
	{
		/*
		 * Did I mention this is Sick.  We always need to
		 * discard the upper 8 bits of the first 16-bit word.
		 */

		data = REG_MSC_RES(host->pdev_id);
		buf[0] = (data >> 8) & 0xff;
		buf[1] = data & 0xff;

		data = REG_MSC_RES(host->pdev_id);
		buf[2] = (data >> 8) & 0xff;
		buf[3] = data & 0xff;

		data = REG_MSC_RES(host->pdev_id);
		buf[4] = data & 0xff;

		cmd->resp[0] =
			buf[1] << 24 | buf[2] << 16 | buf[3] << 8 |
			buf[4];

		// printk("opcode = %d, cmd->resp = 0x%08x\n", cmd->opcode, cmd->resp[0]);
		break;
	}
	case 2:
	{
		data = REG_MSC_RES(host->pdev_id);
		v = data & 0xffff;
		for (i = 0; i < 4; i++) {
			data = REG_MSC_RES(host->pdev_id);
			w1 = data & 0xffff;
			data = REG_MSC_RES(host->pdev_id);
			w2 = data & 0xffff;
			cmd->resp[i] = v << 24 | w1 << 8 | w2 >> 8;
			v = w2;
		}
		break;
	}
	case 0:
		break;
	}
	if (stat & MSC_STAT_TIME_OUT_RES) {
		/* :-( our customer do not want to see SO MANY timeouts :-(
		   so only CMD5 can return timeout error!!! */

		/*
		 * Note: we can not return timeout when CMD SD_SWITCH or MMC_SWITCH
		 * because we declared that out host->caps support MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA and MMC_CAP_MMC_HIGHSPEED
		 * if in the future some error occured because of this, we must add some code to remember
		 * which mode(SDIO/SD/MMC)  the MSC is in
		 */
		switch(cmd->opcode) {
		case SD_IO_SEND_OP_COND:
			//case SD_SWITCH:
			//case MMC_SWITCH:
		case SD_SEND_IF_COND:
		case MMC_APP_CMD:
			cmd->error = -ETIMEDOUT;
			break;
		default:
			printk("jz-msc%d: ignored MSC_STAT_TIME_OUT_RES, cmd=%d\n", host->pdev_id, cmd->opcode);
		}
	} else if (stat & MSC_STAT_CRC_RES_ERR && cmd->flags & MMC_RSP_CRC) {
		printk("jz-msc%d: MSC_STAT_CRC, cmd=%d\n", host->pdev_id, cmd->opcode);
		if (cmd->opcode == MMC_ALL_SEND_CID ||
		    cmd->opcode == MMC_SEND_CSD ||
		    cmd->opcode == MMC_SEND_CID) {
			/* a bogus CRC error can appear if the msb of
			   the 15 byte response is a one */
			if ((cmd->resp[0] & 0x80000000) == 0)
				cmd->error = -EILSEQ;
		}
	}

	TRACE_CMD_RES();

	if (host->curr.data && cmd->error == 0){
		jz_mmc_enable_irq(host, MSC_IMASK_DATA_TRAN_DONE);
	} else {
		jz_mmc_finish_request(host, host->curr.mrq);
	}

	return 1;
}

#ifdef USE_DMA
void jz_mmc_data_start(struct jz_mmc_host *host)
{
	struct mmc_data *data = host->curr.data;
	unsigned int nob = data->blocks;
	unsigned int block_size = data->blksz;
	int channel;
	int mode;
#ifndef USE_DMA_DESC
	int i;
	int j = 0;
	int ds = 4;
	struct scatterlist *sgentry;
#endif


	if (data->flags & MMC_DATA_WRITE) {
		channel = host->dma.txchannel;
		mode = DMA_MODE_WRITE;
		host->dma.dir = DMA_TO_DEVICE;
	} else {
		channel = host->dma.rxchannel;
		mode = DMA_MODE_READ;
		host->dma.dir = DMA_FROM_DEVICE;
	}

	if (data->flags & MMC_DATA_STREAM)
		nob = 0xffff;

	REG_MSC_NOB(host->pdev_id) = nob;
	REG_MSC_BLKLEN(host->pdev_id) = block_size;

	host->dma.len =
	    dma_map_sg(mmc_dev(host->mmc), data->sg, data->sg_len,
		       host->dma.dir);

#ifdef USE_DMA_DESC
	jz_mmc_start_scatter_dma(channel, host, data->sg, host->dma.len, mode);
#ifdef DEBUG
	start_debug_timer(host->pdev_id);
#endif
#else
	j = 0;
	for_each_sg(data->sg, sgentry, host->dma.len, i) {
		host->sg_cpu[j].dtadr = sg_dma_address(sgentry);
		host->sg_cpu[j].dcmd = sg_dma_len(sgentry);
		dma_cache_wback_inv((unsigned long)CKSEG0ADDR(sg_dma_address(sgentry) + data->sg->offset),
				    host->sg_cpu[j].dcmd);

		if ((likely(host->sg_cpu[j].dcmd % 32 == 0)))
			ds = 32; /* 32 byte */
		else if (host->sg_cpu[j].dcmd % 16 == 0)
			ds = 16; /* 16 byte */
		else
			ds = 4; /* default to 4 byte */


		/* FIXME: bug here!!!!! wait for current dma done, then next sg */
		jz_mmc_start_dma(channel, host->sg_cpu[j].dtadr,
				 host->sg_cpu[j].dcmd, mode, ds);

		j++;

	}
#endif
}
#else

static void jz_mmc_receive_pio(struct jz_mmc_host *host)
{
	struct mmc_data *data = host->curr.data;
	int sg_len = 0, max = 0, count = 0;
	unsigned int *buf = 0;
	struct scatterlist *sg;
	unsigned int nob;

	nob = data->blocks;

	REG_MSC_NOB(host->pdev_id) = nob;
	REG_MSC_BLKLEN(host->pdev_id) = data->blksz;

	max = host->pio.len;
	if (host->pio.index < host->dma.len) {
		sg = &data->sg[host->pio.index];
		buf = sg_virt(sg) + host->pio.offset;

		/* This is the space left inside the buffer */
		sg_len = sg_dma_len(&data->sg[host->pio.index]) - host->pio.offset;
		/* Check to if we need less then the size of the sg_buffer */
		if (sg_len < max) max = sg_len;
	}

	max = max / 4;
	for(count = 0; count < max; count++) {
		while (REG_MSC_STAT(host->pdev_id) & MSC_STAT_DATA_FIFO_EMPTY)
			;
		*buf++ = REG_MSC_RXFIFO(host->pdev_id);
	}
	host->pio.len -= count;
	host->pio.offset += count;

	if (sg_len && count == sg_len) {
		host->pio.index++;
		host->pio.offset = 0;
	}
}

static void jz_mmc_send_pio(struct jz_mmc_host *host)
{
	int sg_len, max, count = 0;
	unsigned int *wbuf = 0;
	unsigned int nob;
	struct mmc_data *data = host->curr.data;
	struct scatterlist *sg;

	nob = data->blocks;

	REG_MSC_NOB(host->pdev_id) = nob;
	REG_MSC_BLKLEN(host->pdev_id) = data->blksz;

	/* This is the pointer to the data buffer */
	sg = &data->sg[host->pio.index];
	wbuf = sg_virt(sg) + host->pio.offset;

	/* This is the space left inside the buffer */
	sg_len = data->sg[host->pio.index].length - host->pio.offset;

	/* Check to if we need less then the size of the sg_buffer */
	max = (sg_len > host->pio.len) ? host->pio.len : sg_len;
	max = max / 4;
	for(count = 0; count < max; count++) {
		while (REG_MSC_STAT(host->pdev_id) & MSC_STAT_DATA_FIFO_FULL)
				;
		REG_MSC_TXFIFO(host->pdev_id) = *wbuf++;
	}

	host->pio.len -= count;
	host->pio.offset += count;

	if (count == sg_len) {
		host->pio.index++;
		host->pio.offset = 0;
	}
}

#endif

static void jz_mmc_execute_cmd(struct jz_mmc_host *host, struct mmc_command *cmd, unsigned int cmdat)
{
	u32 timeout = 0x7fffff;
	unsigned int stat;
	unsigned int stat_err_bits = 0;
	u32 cmdarg = 0;
	int err = 0;

	WARN_ON(host->curr.cmd != NULL);
	host->curr.cmd = cmd;

	/* mask interrupts */
	REG_MSC_IMASK(host->pdev_id) = 0xffff;

	/* clear status */
	REG_MSC_IREG(host->pdev_id) = 0xffff;

	if (cmd->flags & MMC_RSP_BUSY)
		cmdat |= MSC_CMDAT_BUSY;

	switch (RSP_TYPE(mmc_resp_type(cmd))) {
	case RSP_TYPE(MMC_RSP_R1):	// r1, r1b, r5, r6, r7
		cmdat |= MSC_CMDAT_RESPONSE_R1;
		host->curr.r_type = 1;
		break;
	case RSP_TYPE(MMC_RSP_R3):	// r3, r4
		cmdat |= MSC_CMDAT_RESPONSE_R3;
		host->curr.r_type = 1;
		break;
	case RSP_TYPE(MMC_RSP_R2):	// r2
		cmdat |= MSC_CMDAT_RESPONSE_R2;
		host->curr.r_type = 2;
		break;
	default:
		break;
	}

	REG_MSC_CMD(host->pdev_id) = cmd->opcode;

	cmdarg = cmd->arg;

	/* Set argument */
	if(host->plat->bus_width == 1) {
		if (cmd->opcode == 6) {
			/* set  1 bit sd card bus*/
			if (cmd->arg == 2) {
				cmdarg = 0;
				REG_MSC_ARG(host->pdev_id) = 0;
			}

			/* set  1 bit mmc card bus*/
			if (cmd->arg == 0x3b70101) {
				cmdarg = 0x3b70001;
				REG_MSC_ARG(host->pdev_id) = 0x3b70001;
			}
		} else
			REG_MSC_ARG(host->pdev_id) = cmd->arg;
	} else if(host->plat->bus_width == 8) {
		if (cmd->opcode == 6) {
			/* set  8 bit mmc card bus*/
			if (cmd->arg == 0x3b70101) {
				cmdarg = 0x3b70201;
				REG_MSC_ARG(host->pdev_id) = 0x3b70201;
			} else
				REG_MSC_ARG(host->pdev_id) = cmd->arg;
		} else
			REG_MSC_ARG(host->pdev_id) = cmd->arg;
	} else
		REG_MSC_ARG(host->pdev_id) = cmd->arg;

	/* Set command */
#ifdef USE_DMA_BUSRT_64
	cmdat |= MSC_CMDAT_RTRG_EQUALT_16;
#endif
	REG_MSC_CMDAT(host->pdev_id) = cmdat;

	TRACE_CMD_REQ();

	/* Send command */
	REG_MSC_STRPCL(host->pdev_id) = MSC_STRPCL_START_OP;

	while (timeout-- && !((stat = REG_MSC_STAT(host->pdev_id)) & MSC_STAT_END_CMD_RES))
		;

	stat_err_bits = stat & MSC_STAT_ERR_BITS;

	if (timeout == 0)
		printk("wait END_CMD_RES failed!!!\n");


	REG_MSC_IREG(host->pdev_id) = MSC_IREG_END_CMD_RES;	/* clear irq flag */

	if (cmd->flags & MMC_RSP_BUSY) {
		timeout = 0x7fffff;
		while (timeout-- && !(REG_MSC_IREG(host->pdev_id) & MSC_IREG_PRG_DONE))
			;
		REG_MSC_IREG(host->pdev_id) = MSC_IREG_PRG_DONE;	/* clear status */
	} else {
		switch(cmd->opcode) { /* R1b cmds need wait PROG_DONE */
		case 7:
		case 12:
		case 28:
		case 29:
		case 38:
			timeout = 0x7fffff;
			while (timeout-- && !(REG_MSC_IREG(host->pdev_id) & MSC_IREG_PRG_DONE))
				;
			REG_MSC_IREG(host->pdev_id) = MSC_IREG_PRG_DONE;	/* clear status */
			break;
		default:
			/* do nothing */
			break;
		}
	}

	if (SD_IO_SEND_OP_COND == cmd->opcode) {
		if(host->plat->support_sdio == 0) {
			cmd->error = -ETIMEDOUT;
			jz_mmc_finish_request(host, host->curr.mrq);
			return;
		}
	}

	TRACE_DATA_REQ();
#ifndef USE_DMA
	if (host->curr.data) {
		if (host->curr.data->flags & MMC_DATA_READ)
			jz_mmc_receive_pio(host);
		else
			jz_mmc_send_pio(host);
	}
#endif
	jz_mmc_cmd_done(host, stat);

#ifdef USE_DMA
	if (host->curr.data) {
		if(host->curr.data->flags & MMC_DATA_WRITE)
			jz_mmc_data_start(host);

		/* in case that the controller never raise interrupt(May be there are some problem, isn't it?), we must finish the request here!!! */
		err = wait_event_interruptible_timeout(host->msc_wait_queue, ((host->msc_ack) | host->eject), 2 * HZ);
		if(err == -ERESTARTSYS) {
			printk("err == -ERESTARTSYS\n");
			host->curr.mrq->cmd->error = -ENOMEDIUM;
			jz_mmc_finish_request(host, host->curr.mrq);
		}
	}
#else
#if 0
	if (host->curr.data) {
		if (host->curr.data->flags & MMC_DATA_READ)
			jz_mmc_receive_pio(host);
		else
			jz_mmc_send_pio(host);
	}
#endif
#endif

}

static int jz_mmc_data_done(struct jz_mmc_host *host)
{
	struct mmc_data *data = host->curr.data;
	int stat = 0;
	unsigned int stat_err_bits = 0;
	u32 timeout = 0x7fffff;

	if (!data)
		return 0;

#ifdef DEBUG
	stop_debug_timer(host->pdev_id);
#endif

	stat = REG_MSC_STAT(host->pdev_id);
	stat_err_bits = stat & MSC_STAT_ERR_BITS;

	REG_MSC_IREG(host->pdev_id) = MSC_IREG_DATA_TRAN_DONE;	/* clear status */

	if (host->curr.mrq && (host->curr.mrq->data->flags & MMC_DATA_WRITE)) {

		while (timeout-- && !(REG_MSC_IREG(host->pdev_id) & MSC_IREG_PRG_DONE))
			;
		if (timeout == 0)
			printk("PRG_DONE not done!!!\n");
		REG_MSC_IREG(host->pdev_id) = MSC_IREG_PRG_DONE;	/* clear status */
	}

	dma_unmap_sg(mmc_dev(host->mmc), data->sg, host->dma.len,
		     host->dma.dir);

	stat = REG_MSC_STAT(host->pdev_id);
	stat |= stat_err_bits;

	if (stat & MSC_STAT_TIME_OUT_READ) {
		printk("MMC/SD/SDIO timeout, MMC_STAT 0x%x opcode = %d data flags = 0x%0x blocks = %d blksz = %d\n",
		       stat,
		       host->curr.mrq? host->curr.mrq->cmd->opcode : -1,
		       data->flags,
		       data->blocks,
		       data->blksz);
		data->error = -ETIMEDOUT;
	} else if (stat & (MSC_STAT_CRC_READ_ERROR | MSC_STAT_CRC_WRITE_ERROR)) {
		printk("jz-msc%d: MMC/SD/SDIO CRC error, MMC_STAT 0x%x, cmd=%d\n",
		       host->pdev_id, stat,
		       host->curr.mrq? host->curr.mrq->cmd->opcode : -1);
		//data->error = -EILSEQ;
	}
	/*
	 * There appears to be a hardware design bug here.  There seems to
	 * be no way to find out how much data was transferred to the card.
	 * This means that if there was an error on any block, we mark all
	 * data blocks as being in error.
	 */
	if (data->error == 0)
		data->bytes_xfered = data->blocks * data->blksz;
	else
		data->bytes_xfered = 0;

	TRACE_DATA_DONE();

	// jz_mmc_disable_irq(host, MSC_IMASK_DATA_TRAN_DONE);

	host->curr.data = NULL;
	if (host->curr.mrq->stop) {
		jz_mmc_execute_cmd(host, host->curr.mrq->stop, 0);
	} else {
		jz_mmc_finish_request(host, host->curr.mrq);
	}

	return 1;
}

static void jiq_msc_irq(struct work_struct *ptr)
{
	struct jz_mmc_host *host = container_of(ptr, struct jz_mmc_host, msc_jiq_work);

	jz_mmc_data_done(host);

	host->msc_ack = 1;
	wake_up_interruptible(&host->msc_wait_queue);
}


static irqreturn_t jz_mmc_irq(int irq, void *devid)
{
	struct jz_mmc_host *host = devid;
	unsigned int ireg = 0;
	int handled = 0;

	ireg = REG_MSC_IREG(host->pdev_id);

	if (ireg) {
		if (ireg & MSC_IREG_DATA_TRAN_DONE) {
			jz_mmc_disable_irq(host, MSC_IMASK_DATA_TRAN_DONE);
			//schedule_work( &(((struct jz_mmc_host *) devid)->msc_jiq_work) );
			queue_work(host->msc_work_queue, &(((struct jz_mmc_host *) devid)->msc_jiq_work));
			handled = 1;
		}
	}

	return IRQ_RETVAL(handled);
}

static int jz_mmc_msc_init(struct jz_mmc_host *host)
{
	int ret = 0;

	jz_mmc_reset(host);
	// __msc_start_clk(host->pdev_id);
	REG_MSC_LPM(host->pdev_id) = 0x1;	// Low power mode
	REG_MSC_RDTO(host->pdev_id) = 0xffffffff;

	host->msc_ack = 0;
	init_waitqueue_head(&host->msc_wait_queue);

	msc_irq_mask_all(host->pdev_id);

	ret = request_irq(host->irqres->start, jz_mmc_irq, 0, "jz-msc (msc)", host);
	if (ret) {
		printk(KERN_ERR "MMC/SD: can't request MMC/SD IRQ\n");
		return ret;
	}

	host->msc_work_queue = create_rt_workqueue("mscworkqueue");

	INIT_WORK(&host->msc_jiq_work, jiq_msc_irq);

	return 0;
}

static void jz_mmc_msc_deinit(struct jz_mmc_host *host)
{
	free_irq(host->irqres->start, &host);
	destroy_workqueue(host->msc_work_queue);
}

int jz_mmc_msc_register(struct jz_mmc_msc *msc)
{
	if(msc == NULL)
		return -ENOMEM;

	msc->init = jz_mmc_msc_init;
	msc->deinit = jz_mmc_msc_deinit;
	msc->set_clock = jz_mmc_set_clock;
	msc->execute_cmd = jz_mmc_execute_cmd;

	return 0;
}
