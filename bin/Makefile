include ../config.mk

VPATH=../src
DISTDIR=../dist

all: main

clean:
	rm *.o main

main: common.o glsl.o matrix.o object.o shader.o texture.o

shader.o: common.o

.PHONY: all clean
