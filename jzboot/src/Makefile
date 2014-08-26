READLINE ?= 0


ifneq (${READLINE},0)
	LIBS += -lreadline
	CPPFLAGS += -DWITH_READLINE
endif

CC      = gcc
TARGET  = ../jzboot
SOURCES = debug.c devmgr.c ingenic.c main.c shell_lex.c usbdev.c shell.c shell_builtins.c config.c spl_cmdset.c usbboot_cmdset.c elfldr.c
CFLAGS  = --std=gnu99 -Wall -I../include -O2 $(shell pkg-config libusb-1.0 --cflags)
LIBS    += $(shell pkg-config libusb-1.0 --libs)

OBJECTS = ${SOURCES:.c=.o}

all: ${TARGET}

${TARGET}: ${OBJECTS}
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS}

clean:
	rm -f ${TARGET} ${OBJECTS} ${SOURCES:.c=.d}

%.o: %.c
	${CC} ${CPPFLAGS} ${CFLAGS} -o $@ -MD -c $<

%.c: %.l
	flex -o $@ $<

-include ${SOURCES:.c=.d}
