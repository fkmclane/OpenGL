PROJECT=OpenGL
VERSION=0.1

SRC=common.c glsl.c matrix.c object.c opengl.c shader.c texture.c

SRCDIR=src
DISTDIR=dist

CC=cc
CFLAGS=-std=c99 -pipe -O2 -ggdb
CPPFLAGS=-DHAVE_LIBGLEW -DVERSION=$(VERSION)
LDLIBS=-lm -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lrt -lglfw3
