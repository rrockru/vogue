cmd_libbb/perror_nomsg.o := mipsel-linux-gcc -Wp,-MD,libbb/.perror_nomsg.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.23.0.git)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wno-format-security -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-unwind-tables -fno-asynchronous-unwind-tables -Os     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(perror_nomsg)"  -D"KBUILD_MODNAME=KBUILD_STR(perror_nomsg)" -c -o libbb/perror_nomsg.o libbb/perror_nomsg.c

deps_libbb/perror_nomsg.o := \
  libbb/perror_nomsg.c \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/limits.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/syslimits.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/limits.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/features.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/sys/cdefs.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/gnu/stubs.h \
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
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/include/stddef.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/typesizes.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/bits/confname.h \
  /opt/mipseltools-gcc412-lnx26/lib/gcc/mipsel-linux/4.1.2/../../../../mipsel-linux/include/getopt.h \

libbb/perror_nomsg.o: $(deps_libbb/perror_nomsg.o)

$(deps_libbb/perror_nomsg.o):
