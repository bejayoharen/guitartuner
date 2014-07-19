CFILES := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,objs/%.o,$(CFILES))
HEADERS := $(wildcard src/*.h)

CFLAGS += -O2 -Wall -Werror -std=c99 -D_XOPEN_SOURCE
CFLAGS += $(shell pkg-config portaudio-2.0 --cflags)
LIBS += $(shell pkg-config portaudio-2.0 --libs-only-l)
LDFLAGS += $(shell pkg-config portaudio-2.0 --libs-only-L --libs-only-other)

all: tuner

tuner: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

objs:
	mkdir objs

objs/%.o: src/%.c $(HEADERS) | objs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm tuner
	-rm -r objs
