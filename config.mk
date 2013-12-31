CC=cc
CFLAGS=-std=c99 -pipe -O2 -ggdb
CPPFLAGS=-DHAVE_LIBGLEW
LDLIBS=-lm -lGL -lGLEW -lX11 -lXrandr -lXi -lXxf86vm -lpthread -lrt -lglfw3
