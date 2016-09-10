PRODUCT := $(ABS_PRODUCT)/map/app

.PHONY: all clear prepare

all: prepare $(PRODUCT)/map

$(PRODUCT)/map: $(PRODUCT)/map.o
	$(CC) -o $@ $^ \
	$(ABS_PRODUCT)/map/lib/libmap.a

$(PRODUCT)/map.o: map.cpp
	$(CC) -c -o $(PRODUCT)/map.o \
	map.cpp

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

