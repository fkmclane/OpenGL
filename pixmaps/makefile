include ../config.mk

VPATH=../$(TEXDIR)

BMPSRC!=find $(VPATH) -name "*.bmp" -printf "%f "
PNGSRC!=find $(VPATH) -name "*.png" -printf "%f "

%.ppm: %.bmp
	mogrify -flip -strip -format ppm -write $@ $<

%.ppm: %.png
	mogrify -flip -strip -format ppm -write $@ $<

all: $(BMPSRC:.bpm=.ppm) $(PNGSRC:.png=.ppm)

clean:
	rm -f *.ppm

.PHONY: all clean
