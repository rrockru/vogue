/*
 *  linux/drivers/mmc/host/jz_mmc/dma/jz_mmc_dma.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Copyright (c) Ingenic Semiconductor Co., Ltd.
 */


#include <asm/jzsoc.h>
#include <asm/jzmmc/jz_mmc_dma.h>
#include <linux/scatterlist.h>

#ifdef USE_DMA_DESC
static inline int best_burst_size(unsigned int dma_len) {
#ifdef USE_DMA_BUSRT_64
	if (dma_len % 64 == 0)
		return 64;
#endif

	if (dma_len % 32 == 0)
		return 32;

	if (dma_len % 16 == 0)
		return 16;

#ifdef USE_DMA_BUSRT_64
	if (dma_len > 64)
		return 64;
#endif

	if (dma_len > 32)
		return 32;

	if (dma_len > 16)
		return 16;

	return 4;
}

static void sg_to_desc(struct scatterlist *sgentry, struct jz_mmc_host *host, int *desc_pos /* IN OUT */, int mode) {
	jz_dma_desc *desc;
	int pos = *desc_pos;
	unsigned int ds = 32; /* ehh, 32byte is the best */
	unsigned int next;
	dma_addr_t dma_addr;
	unsigned int dma_len;
	unsigned int head_unalign_size = 0; /* ds = 4byte */
	unsigned int aligned_size = 0;
	unsigned int tail_unalign_size = 0; /* ds = 4byte */

	dma_addr = sg_dma_address(sgentry);
	dma_len = sg_dma_len(sgentry);

	BUG_ON(dma_len % 4); /* we do NOT support transfer size < 4byte */

	ds = best_burst_size(dma_len);
	head_unalign_size = dma_addr & (ds - 1);
	tail_unalign_size = (dma_addr + dma_len) & (ds - 1);
	aligned_size = dma_len - head_unalign_size - tail_unalign_size;

	BUG_ON(head_unalign_size % 4);
	BUG_ON(tail_unalign_size % 4);

	if (head_unalign_size) {
		desc = host->dma_desc + pos;
		next = (host->dma_desc_phys_addr + (pos + 1) * (sizeof(jz_dma_desc))) >> 4;
		desc->dcmd = DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32 | DMAC_DCMD_DS_32BIT | DMAC_DCMD_RDIL_IGN |
#ifndef CONFIG_SOC_JZ4760
			DMAC_DCMD_DES_V | DMAC_DCMD_DES_VM | DMAC_DCMD_DES_VIE |
#endif
			DMAC_DCMD_LINK;
		if (DMA_MODE_WRITE == mode) {
			desc->dcmd |= DMAC_DCMD_SAI;
			desc->dsadr = (unsigned int)dma_addr;      /* DMA source address */
			desc->dtadr = CPHYSADDR(MSC_TXFIFO(host->pdev_id));      /* DMA target address */
		} else {
			desc->dcmd |= DMAC_DCMD_DAI;
			desc->dsadr = CPHYSADDR(MSC_RXFIFO(host->pdev_id));
			desc->dtadr = (unsigned int)dma_addr;
		}
		desc->ddadr = (next << 24) | (head_unalign_size >> 2) ;

		pos ++;
	}

	if (aligned_size) {
		desc = host->dma_desc + pos;
		next = (host->dma_desc_phys_addr + (pos + 1) * (sizeof(jz_dma_desc))) >> 4;

		desc->dcmd = DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32 | DMAC_DCMD_RDIL_IGN |
#ifndef CONFIG_SOC_JZ4760
			DMAC_DCMD_DES_V | DMAC_DCMD_DES_VM | DMAC_DCMD_DES_VIE |
#endif
			 DMAC_DCMD_LINK;
		switch (ds) {
#ifdef USE_DMA_BUSRT_64
		case 64:
			desc->dcmd |= DMAC_DCMD_DS_64BYTE;
			break;
#endif

		case 32:
			desc->dcmd |= DMAC_DCMD_DS_32BYTE;
			break;

		case 16:
			desc->dcmd |= DMAC_DCMD_DS_16BYTE;
			break;

		case 4:
			desc->dcmd |= DMAC_DCMD_DS_32BIT;
			break;

		default:
			;
		}

		if (DMA_MODE_WRITE == mode) {
			desc->dcmd |= DMAC_DCMD_SAI;
			desc->dsadr = (unsigned int)(dma_addr + head_unalign_size);      /* DMA source address */
			desc->dtadr = CPHYSADDR(MSC_TXFIFO(host->pdev_id));      /* DMA target address */
		} else {
			desc->dcmd |= DMAC_DCMD_DAI;
			desc->dsadr = CPHYSADDR(MSC_RXFIFO(host->pdev_id));
			desc->dtadr = (unsigned int)(dma_addr + head_unalign_size);;
		}
		desc->ddadr = (next << 24) | (aligned_size  / ds) ;

		pos ++;
	}


	if (tail_unalign_size) {
		desc = host->dma_desc + pos;
		next = (host->dma_desc_phys_addr + (pos + 1) * (sizeof(jz_dma_desc))) >> 4;

		desc->dcmd = DMAC_DCMD_SWDH_32 | DMAC_DCMD_DWDH_32 | DMAC_DCMD_DS_32BIT |   DMAC_DCMD_RDIL_IGN |
#ifndef CONFIG_SOC_JZ4760
			DMAC_DCMD_DES_V | DMAC_DCMD_DES_VM | DMAC_DCMD_DES_VIE |
#endif
			DMAC_DCMD_LINK;
		if (DMA_MODE_WRITE == mode) {
			desc->dcmd |= DMAC_DCMD_SAI;
			desc->dsadr = (unsigned int)(dma_addr + head_unalign_size + aligned_size);      /* DMA source address */
			desc->dtadr = CPHYSADDR(MSC_TXFIFO(host->pdev_id));      /* DMA target address */
		} else {
			desc->dcmd |= DMAC_DCMD_DAI;
			desc->dsadr = CPHYSADDR(MSC_RXFIFO(host->pdev_id));
			desc->dtadr = (unsigned int)(dma_addr + head_unalign_size + aligned_size);;
		}
		desc->ddadr = (next << 24) | (tail_unalign_size >> 2) ;

		pos ++;

	}

	*desc_pos = pos;
}

void jz_mmc_start_scatter_dma(int chan, struct jz_mmc_host *host,
			      struct scatterlist *sg, unsigned int sg_len, int mode) {
	int i = 0;
	int desc_pos = 0;
	struct mmc_data *data = host->curr.data;
	struct scatterlist *sgentry;
	jz_dma_desc *desc;
	unsigned int next;
	unsigned long flags;

	memset(host->dma_desc, 0, PAGE_SIZE);

	desc = host->dma_desc;
	next = (host->dma_desc_phys_addr + (sizeof(jz_dma_desc))) >> 4;

	desc_pos = 0;
	flags = claim_dma_lock();
	for_each_sg(data->sg, sgentry, host->dma.len, i) {
		sg_to_desc(sgentry, host, &desc_pos, mode);
	}

	desc = host->dma_desc + (desc_pos - 1);
	desc->dcmd |= DMAC_DCMD_TIE;
	desc->dcmd &= ~DMAC_DCMD_LINK;
	desc->ddadr &= ~0xff000000;

	dma_cache_wback_inv((unsigned long)desc, desc_pos * (sizeof(jz_dma_desc)));

        /* Setup DMA descriptor address */
	REG_DMAC_DDA(chan) = host->dma_desc_phys_addr;

	/* Setup request source */
	if (DMA_MODE_WRITE == mode) {
		if(host->pdev_id == 0)
			REG_DMAC_DRSR(chan) = DMAC_DRSR_RS_MSC0OUT;
		else if(host->pdev_id == 1)
			REG_DMAC_DRSR(chan) = DMAC_DRSR_RS_MSC1OUT;
		else
#ifdef  DMAC_DRSR_RS_MSC2OUT
			REG_DMAC_DRSR(chan) = DMAC_DRSR_RS_MSC2OUT;
#else
		;
#endif
	} else {
		if(host->pdev_id == 0)
			REG_DMAC_DRSR(chan) = DMAC_DRSR_RS_MSC0IN;
		else if(host->pdev_id == 1)
			REG_DMAC_DRSR(chan) = DMAC_DRSR_RS_MSC1IN;
		else
#ifdef  DMAC_DRSR_RS_MSC2IN
			REG_DMAC_DRSR(chan) = DMAC_DRSR_RS_MSC2IN;
#else
		;
#endif
	}

	/* Setup DMA channel control/status register */
	REG_DMAC_DCCSR(chan) = DMAC_DCCSR_EN;	/* descriptor transfer, clear status, start channel */

	/* Enable DMA */
	REG_DMAC_DMACR(chan / HALF_DMA_NUM) = DMAC_DMACR_DMAE;

	/* DMA doorbell set -- start DMA now ... */
	REG_DMAC_DMADBSR(chan / HALF_DMA_NUM) = 1 << (chan - (chan / HALF_DMA_NUM) * HALF_DMA_NUM) ;

	release_dma_lock(flags);

}
#else
void jz_mmc_start_dma(int chan, unsigned long phyaddr, int count, int mode, int ds)
{
	unsigned long flags;

	flags = claim_dma_lock();
	disable_dma(chan);
	clear_dma_ff(chan);
	jz_set_dma_src_width(chan, 32);
	jz_set_dma_dest_width(chan, 32);
	jz_set_dma_block_size(chan, ds);
	set_dma_mode(chan, mode);
	set_dma_addr(chan, phyaddr);
	set_dma_count(chan, (count + ds - 1));
	enable_dma(chan);
	release_dma_lock(flags);
}
#endif

static irqreturn_t jz_mmc_dma_rx_callback(int irq, void *devid)
{
	struct jz_mmc_host *host = devid;
	int chan = host->dma.rxchannel;

	disable_dma(chan);
	if (__dmac_channel_address_error_detected(chan)) {
		printk("%s: DMAC address error.\n",
		       __FUNCTION__);
		__dmac_channel_clear_address_error(chan);
	}
	if (__dmac_channel_transmit_end_detected(chan)) {
		__dmac_channel_clear_transmit_end(chan);
	}

	return IRQ_HANDLED;
}

static irqreturn_t jz_mmc_dma_tx_callback(int irq, void *devid)
{
	struct jz_mmc_host *host = devid;
	unsigned int chan = host->dma.txchannel;

	disable_dma(chan);
	if (__dmac_channel_address_error_detected(chan)) {
		printk("%s: DMAC address error.\n",
		       __FUNCTION__);
		__dmac_channel_clear_address_error(chan);
	}
	if (__dmac_channel_transmit_end_detected(chan)) {
		__dmac_channel_clear_transmit_end(chan);
	}

	return IRQ_HANDLED;
}

static int jz_mmc_init_dma(struct jz_mmc_host *host)
{
	host->dma.rxchannel = -1;
	host->dma.txchannel = -1;

	if (!host->dmares)
		return -ENODEV;

	host->dma.rxchannel = jz_request_dma(host->dmares->start, "dma-rx", jz_mmc_dma_rx_callback,
			       0, host);
	if (host->dma.rxchannel < 0) {
		printk(KERN_ERR "jz_request_dma failed for MMC Rx\n");
		goto err1;
	}

	if (host->dma.rxchannel < 6)
		REG_DMAC_DMACR(0) |= DMAC_DMACR_FMSC;
	else
		REG_DMAC_DMACR(1) |= DMAC_DMACR_FMSC;

	/* Request MMC Tx DMA channel */
	host->dma.txchannel = jz_request_dma(host->dmares->end, "dma-tx", jz_mmc_dma_tx_callback,
			       0, host);
	if (host->dma.txchannel < 0) {
		printk(KERN_ERR "jz_request_dma failed for MMC Tx\n");
		goto err2;
	}

	if (host->dma.txchannel < 6)
		REG_DMAC_DMACR(0) |= DMAC_DMACR_FMSC;
	else
		REG_DMAC_DMACR(1) |= DMAC_DMACR_FMSC;

#ifdef USE_DMA_DESC
	host->dma_desc = (jz_dma_desc *)__get_free_pages(GFP_KERNEL, 0); /* 4096 / 4 = 1024 descriptor max */
	host->dma_desc_phys_addr = CPHYSADDR((unsigned long)host->dma_desc);
#endif

	return 0;

err2:
	jz_free_dma(host->dma.rxchannel);
err1:
	return -ENODEV;
}

static void jz_mmc_deinit_dma(struct jz_mmc_host *host)
{
	jz_free_dma(host->dma.rxchannel);
	jz_free_dma(host->dma.txchannel);
}

int jz_mmc_dma_register(struct jz_mmc_dma *dma)
{
	if(dma == NULL)
		return -ENOMEM;

	dma->init = jz_mmc_init_dma;
	dma->deinit = jz_mmc_deinit_dma;

	return 0;
}
