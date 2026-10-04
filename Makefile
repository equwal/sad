VERSION = 0.0

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CFLAGS = -I/usr/local/include
LDFLAGS = -L /usr/local/lib
# sound backends: must match SOUNDSYS in config.h
# OSS (Cygwin, *BSD): OUTOBJ = oss.o, OUTLIBS =
OUTOBJ = alsa.o sndio.o
OUTLIBS = -lsndio -lasound

LDLIBS = -lsndfile -lmpg123 -lvorbisfile -lsoxr $(OUTLIBS)

OBJ =\
	$(OUTOBJ)\
	cmd.o\
	decoder.o\
	fifo.o\
	mp3.o\
	notify.o\
	output.o\
	pcm.o\
	playlist.o\
	sad.o\
	vorbis.o\
	wav.o

BIN = sad

# non-OpenBSD
OBJ += compat/reallocarray.o
OBJ += compat/strlcat.o
OBJ += compat/strlcpy.o
OBJ += compat/strtonum.o
CFLAGS += -DCOMPAT

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

$(OBJ): arg.h compat.h config.h queue.h sad.h

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp -f sad.1 $(DESTDIR)$(MANPREFIX)/man1

install-cygwin:
	sed 's/SOUNDSYS ALSA/SOUNDSYS OSS/' config.def.h > config.h
	$(MAKE) install OUTOBJ=oss.o OUTLIBS=
	cp -f cygwin/sacc $(DESTDIR)$(PREFIX)/bin
	cp -f sacc.1 $(DESTDIR)$(MANPREFIX)/man1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN) $(DESTDIR)$(PREFIX)/bin/sacc
	rm -f $(DESTDIR)$(MANPREFIX)/man1/sad.1 $(DESTDIR)$(MANPREFIX)/man1/sacc.1

clean:
	rm -f $(BIN) $(OBJ)

.SUFFIXES: .def.h

.def.h.h:
	cp $< $@

.PHONY:
	all install install-cygwin uninstall clean
