include config.mk

DIRS=bin objects textures

all:
	@for dir in $(DIRS); do \
		$(MAKE) -C $$dir; \
	done
clean:
	@for dir in $(DIRS); do \
		$(MAKE) -C $$dir clean; \
	done

dist: all
	mkdir -p $(DISTDIR)
	cp bin/main $(DISTDIR)/$(PROJECT)
	cp glsl/*.glsl $(DISTDIR)/
	cp objects/*.mod $(DISTDIR)/
	cp textures/*.ppm $(DISTDIR)/

release: dist clean

.PHONY: all clean dist release
