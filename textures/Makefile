all:
	@for file in *.png; do \
		if [ -e "$$file" ]; then \
			mogrify -flip -strip -format ppm "$$file"; \
		fi; \
	done

clean:
	rm *.ppm

.PHONY: all clean
