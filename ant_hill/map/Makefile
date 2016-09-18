PRODUCT := "$(ABS_PRODUCT)/map"

TARGETS := lib rpc app

.PHONY: all prepare $(TARGETS)

all: prepare $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

