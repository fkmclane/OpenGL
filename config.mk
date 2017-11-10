PROJECT=gl-fooster
VERSION=0.1

SRC=common.c matrix.c object.c world.c scene.c shader.c texture.c

DIRS=bin textures
DIST=glsl/*.glsl objects/*.obj textures/*.ppm

SRCDIR=src
DISTDIR=dist

CC?=cc
CPPFLAGS+=-DVERSION=$(VERSION)
CFLAGS+=-std=c11 `pkg-config --cflags glfw3` -pipe -O2 -ggdb -Wall -Wpedantic -Wextra -Werror
LDLIBS+=-lm -lGL `pkg-config --libs glfw3`

#CPPFLAGS+=-DHAVE_LIBGLEW -DVERSION=$(VERSION)
#CFLAGS+=-std=c11 `pkg-config --cflags glfw3` -pipe -O2 -ggdb -Wall -Wpedantic -Wextra -Werror
#LDLIBS+=-lm -lGL -lGLEW  `pkg-config --libs glfw3`
