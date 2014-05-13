all:
	@for file in *.png; do \
		if [ -e "$$file" ]; then \
			mogrify -flip -strip -format ppm "$$file"; \
		fi; \
	done

clean:
	rm -f *.ppm

.PHONY: all clean
