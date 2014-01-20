include ../config.mk

VPATH=../$(SRCDIR)
DISTDIR=../$(DISTDIR)

all: main

clean:
	rm *.o main

main: common.o glsl.o matrix.o object.o opengl.o shader.o texture.o

shader.o: common.o

.PHONY: all clean
