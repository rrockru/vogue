cmd_shell/hush.o := mipsel-linux-gcc -Wp,-MD,shell/.hush.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.23.0.git)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wno-format-security -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-unwind-tables -fno-asynchronous-unwind-tables -Os     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(hush)"  -D"KBUILD_MODNAME=KBUILD_STR(hush)" -c -o shell/hush.o shell/hush.c

deps_shell/hush.o := \
  shell/hush.c \
    $(wildcard include/config/hush/case.h) \
    $(wildcard include/config/hush/random/support.h) \
    $(wildcard include/config/hush.h) \
    $(wildcard include/config/msh.h) \
    $(wildcard include/config/feature/sh/is/hush.h) \
    $(wildcard include/config/feature/bash/is/hush.h) \
    $(wildcard include/config/hush/fast.h) \
    $(wildcard include/config/hush/dollar/ops.h) \
    $(wildcard include/config/feature/sh/standalone.h) \
    $(wildcard include/config/hush/interactive.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/fancy/prompt.h) \
    $(wildcard include/config/feature/editing/save/on/exit.h) \
    $(wildcard include/config/hush/if.h) \
    $(wildcard include/config/hush/loops.h) \
    $(wildcard include/config/has/keywords.h) \
    $(wildcard include/config/has/no/keywords.h) \
    $(wildcard include/config/hush/bash/compat.h) \
    $(wildcard include/config/hush/functions.h) \
    $(wildcard include/config/hush/job.h) \
    $(wildcard include/config/hush/local.h) \
    $(wildcard include/config/hush/mode/x.h) \
    $(wildcard include/config/feature/editing/max/len.h) \
    $(wildcard include/config/hush/help.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/feature/clean/up.h) \
    $(wildcard include/config/feature/check/unicode/in/env.h) \
    $(wildcard include/config/unicode/using/locale.h) \
    $(wildcard include/config/hush/brace/expansion.h) \
    $(wildcard include/config/hush/tick.h) \
    $(wildcard include/config/sh/math/support.h) \
    $(wildcard include/config/feature/sh/nofork.h) \
    $(wildcard include/config/hush/savehistory.h) \
    $(wildcard include/config/feature/sh/histfilesize.h) \
    $(wildcard include/config/feature/sh/extra/quiet.h) \
    $(wildcard include/config/hush/export/n.h) \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/malloc.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/features.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/cdefs.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/gnu/stubs.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/stddef.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/glob.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/fnmatch.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/utsname.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/utsname.h \
  include/busybox.h \
    $(wildcard include/config/feature/suid.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/feature/installer.h) \
    $(wildcard include/config/build/libbusybox.h) \
    $(wildcard include/config/feature/shared/busybox.h) \
  include/libbb.h \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/verbose.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/seamless/xz.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/long/opts.h) \
    $(wildcard include/config/feature/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/xxx.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/pmap.h) \
    $(wildcard include/config/feature/show/threads.h) \
    $(wildcard include/config/feature/ps/additional/columns.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/unicode/support.h) \
    $(wildcard include/config/feature/mtab/support.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/limits.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/syslimits.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/limits.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/posix1_lim.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/local_lim.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/linux/limits.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/posix2_lim.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/xopen_lim.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/stdio_lim.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/byteswap.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/byteswap.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/endian.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/endian.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/stdint.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/wchar.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/wordsize.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/stdbool.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/unistd.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/posix_opt.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/environments.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/types.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/typesizes.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/confname.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/getopt.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/ctype.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/xlocale.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/dirent.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/dirent.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/errno.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/errno.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/linux/errno.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/asm/errno.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/asm-generic/errno-base.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/fcntl.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/fcntl.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sgidefs.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/types.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/time.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/select.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/select.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sigset.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/time.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/sysmacros.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/pthreadtypes.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sched.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/stat.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/stat.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/inttypes.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/netdb.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/netinet/in.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/socket.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/uio.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/uio.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/socket.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sockaddr.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/asm/socket.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/asm/sockios.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/asm/ioctl.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/in.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/rpc/netdb.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/siginfo.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/netdb.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/setjmp.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/setjmp.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/signal.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/signum.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sigaction.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sigcontext.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sigstack.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/ucontext.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sigthread.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/stdio.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/libio.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/_G_config.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/wchar.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/gconv.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/stdarg.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/sys_errlist.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/stdlib.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/waitflags.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/waitstatus.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/alloca.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/string.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/libgen.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/poll.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/poll.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/poll.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/ioctl.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/ioctls.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/asm/ioctls.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/ioctl-types.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/ttydefaults.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/mman.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/mman.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/time.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/wait.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/resource.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/resource.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/termios.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/termios.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/param.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/linux/param.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/pwd.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/grp.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/mntent.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/paths.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/statfs.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/statfs.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/utmp.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/utmp.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/arpa/inet.h \
  include/pwd_.h \
  include/grp_.h \
  include/shadow_.h \
  include/xatonum.h \
  include/applet_metadata.h \
    $(wildcard include/config/install/no/usr.h) \
  include/unicode.h \
    $(wildcard include/config/last/supported/wchar.h) \
    $(wildcard include/config/unicode/combining/wchars.h) \
    $(wildcard include/config/unicode/wide/wchars.h) \
    $(wildcard include/config/unicode/bidi/support.h) \
    $(wildcard include/config/unicode/neutral/table.h) \
  shell/shell_common.h \
  shell/math.h \
    $(wildcard include/config/sh/math/support/64.h) \
  shell/match.h \
  shell/random.h \
  include/NUM_APPLETS.h \

shell/hush.o: $(deps_shell/hush.o)

$(deps_shell/hush.o):
