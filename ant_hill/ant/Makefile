PRODUCT := "$(ABS_PRODUCT)/ant"

TARGETS := lib

.PHONY: all clear prepare $(TARGETS)

all: prepare $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

