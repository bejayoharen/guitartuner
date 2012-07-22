CFLAGS:=-02 -Wall -Werror -std=c99
CFILES:=$(wildcard src/*.c)
OBJS:=$(patsubst src/%.c,objs/%.o,$(CFILES))
HEADERS:=$(wildcard src/*.h)
OS:=$(shell uname)
LIBS:=$(wildcard $(OS)/*.a)

LINK_FLAGS=-framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices

all: tuner

tuner: $(HEADERS) $(OBJS) $(LIBS)
	$(CC) $(LINK_FLAGS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

objs:
	-rm objs
	mkdir objs

objs/%.o: src/%.c $(HEADERS) objs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm tuner
	-rm -r objs
