CFLAGS:=-O2 -Wall -Werror -std=c99
CFILES:=$(wildcard src/*.c)
OBJS:=$(patsubst src/%.c,objs/%.o,$(CFILES))
HEADERS:=$(wildcard src/*.h)
LIBS:=$(wildcard libs/*.a $(OS)/*.a)
LINK_FLAGS:=

OS:=$(shell uname)
ifeq ($(OS),Darwin)
	LINK_FLAGS:=$(LINK_FLAGS) -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices
endif

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
