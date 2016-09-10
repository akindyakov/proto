PRODUCT = $(ABS_PRODUCT)/lib/map

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libmap.a ut

ut: $(PRODUCT)/2d_field_ut

$(PRODUCT)/libmap.a: $(PRODUCT)/2d_field.o $(PRODUCT)/map_symbols.o
	ar rcs $@ $^

$(PRODUCT)/2d_field.o: 2d_field.h 2d_field.cpp
	$(CC) -c -o $(PRODUCT)/2d_field.o \
    2d_field.cpp

$(PRODUCT)/map_symbols.o: map_symbols.h map_symbols.cpp
	$(CC) -c -o $(PRODUCT)/map_symbols.o \
    map_symbols.cpp

$(PRODUCT)/2d_field_ut: $(PRODUCT)/2d_field_ut.o $(PRODUCT)/libmap.a
	$(CC) -v -o $@ $^

$(PRODUCT)/2d_field_ut.o: 2d_field_ut.cpp
	$(CC) -c -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

