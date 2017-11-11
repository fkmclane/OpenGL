include ../config.mk

VPATH=../$(SRCDIR)

SRC!=find $(VPATH) -name "*.c" -printf "%f "

all: main

clean:
	rm -f *.o main

main: $(SRC:.c=.o)

.PHONY: all clean
