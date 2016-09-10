export ABS_PRODUCT := $(abspath product)
export SRC_PATH := $(abspath .)

export CC_FLAGS := -std=c++14 -Wall -I $(SRC_PATH)
export CC := clang++ $(CC_FLAGS)

LOCAL_PRODUCT = $(PRODUCT)

.PHONY: all lib app

TARGETS := lib app

all: $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(ABS_PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

