PRODUCT := "$(ABS_PRODUCT)/tools"

TARGETS := traits

.PHONY: all prepare $(TARGETS)

all: prepare $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

