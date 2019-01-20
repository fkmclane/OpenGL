PROJECT=gl-fooster
VERSION=0.1

DIRS=build pixmaps
DIST=glsl/*.glsl objects/*.obj pixmaps/*.ppm

SRCDIR=src
TEXDIR=textures
DISTDIR=dist

CC?=cc

#CPPFLAGS+=-DVERSION=$(VERSION)
#CFLAGS+=-std=c11 `pkg-config --cflags glfw3` -pipe -ggdb -Wall -Wpedantic -Wextra -Werror
#LDLIBS+=-lm -lGL `pkg-config --libs glfw3`

CPPFLAGS+=-DHAVE_LIBGLEW -DVERSION=$(VERSION)
CFLAGS+=-std=c11 -iframework OpenGL `pkg-config --cflags glfw3` -pipe -ggdb -Wall -Wpedantic -Wextra -Werror -fsanitize=address
LDLIBS+=-framework OpenGL -lm -lGLEW  `pkg-config --libs glfw3` -fsanitize=address
