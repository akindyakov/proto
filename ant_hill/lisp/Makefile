PRODUCT := "$(ABS_PRODUCT)/lisp"

TARGETS := lib app

.PHONY: all prepare $(TARGETS)

all: prepare $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

