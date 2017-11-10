include config.mk

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
	cp $(DIST) $(DISTDIR)/

distclean:
	rm -rf $(DISTDIR)

release: dist clean

run: dist
	cd $(DISTDIR); ./$(PROJECT)

.PHONY: all clean dist distclean release run
