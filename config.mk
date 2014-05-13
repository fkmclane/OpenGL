PROJECT=OpenGL
VERSION=0.1

SRC=common.c glsl.c matrix.c object.c opengl.c shader.c texture.c

SRCDIR=src
DISTDIR=dist

DIRS=bin textures
DIST=glsl/*.glsl objects/*.obj textures/*.ppm

CC?=cc
CFLAGS+=-std=c99 -pipe -O2 -ggdb
CPPFLAGS+=-DHAVE_LIBGLEW -DVERSION=$(VERSION)
LDLIBS+=-lm -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lXcursor -lpthread -lrt -lglfw3
