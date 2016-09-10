PRODUCT := "$(ABS_PRODUCT)/lib"

TARGETS := map

.PHONY: all prepare $(TARGETS)

all: prepare $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

