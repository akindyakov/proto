export ABS_PRODUCT := $(abspath product)
export SRC_PATH := $(abspath .)

export CC_FLAGS := -std=c++14 -Wall -I $(SRC_PATH) -I $(ABS_PRODUCT)
export CC := clang++ $(CC_FLAGS)

LOCAL_PRODUCT = $(PRODUCT)

TARGETS := map ant

.PHONY: all $(TARGETS)

all: $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(ABS_PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

