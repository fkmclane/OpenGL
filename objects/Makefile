all: modconvert
	@for file in *.obj; do \
		if [ -e "$$file" ]; then \
			./modconvert "$$file"; \
		fi; \
	done

clean:
	rm *.mod

.PHONY: all clean
