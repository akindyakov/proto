#PRODUCT=product
PRODUCT := "$(ABS_PRODUCT)/app"

#CC=g++
#export CC_FLAGS := -std=c++14 -Wall
#export CC = clang++ CC_FLAGS

TARGETS := map

.PHONY: all clear prepare $(TARGETS)

all: prepare $(TARGETS)

$(TARGETS):
	$(MAKE) --directory=$@

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

