PRODUCT := $(ABS_PRODUCT)/doc

.PHONY: all clear prepare

all: prepare $(PRODUCT)/net.pdf

$(PRODUCT)/net.pdf: net.dot
	dot \
    -Tpdf \
    -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

