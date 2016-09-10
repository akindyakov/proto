PRODUCT := $(ABS_PRODUCT)/app/map

.PHONY: all clear prepare

all: prepare $(PRODUCT)/map

$(PRODUCT)/map: $(PRODUCT)/map.o
	$(CC) -o $@ $^ \
	$(ABS_PRODUCT)/lib/map/libmap.a

$(PRODUCT)/map.o: map.cpp
	$(CC) -c -o $(PRODUCT)/map.o \
	map.cpp

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

