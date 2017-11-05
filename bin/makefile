include ../config.mk

VPATH=../$(SRCDIR)
DISTDIR=../$(DISTDIR)

all: main

clean:
	rm -f *.o main

main: $(SRC:.c=.o)

.PHONY: all clean
