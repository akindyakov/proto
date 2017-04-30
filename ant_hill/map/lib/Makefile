PRODUCT = $(ABS_PRODUCT)/map/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libmap.a ut

#ut: $(PRODUCT)/2d_field_ut $(PRODUCT)/transaction_ut
ut: $(PRODUCT)/2d_field_ut

#$(PRODUCT)/libmap.a: $(PRODUCT)/2d_field.o $(PRODUCT)/map_symbols.o $(PRODUCT)/transaction.o
$(PRODUCT)/libmap.a: $(PRODUCT)/2d_field.o $(PRODUCT)/map_symbols.o
	ar rcs $@ $^

$(PRODUCT)/2d_field.o: 2d_field.h 2d_field.cpp
	$(CC) -c -o $(PRODUCT)/2d_field.o \
    2d_field.cpp

$(PRODUCT)/map_symbols.o: map_symbols.h map_symbols.cpp
	$(CC) -c -o $(PRODUCT)/map_symbols.o \
    map_symbols.cpp

$(PRODUCT)/transaction.o: transaction.cpp transaction.h $(PRODUCT)/2d_field.o
	$(CC) -c -o $@ $<

$(PRODUCT)/2d_field_ut: $(PRODUCT)/2d_field_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/2d_field_ut.o: 2d_field_ut.cpp
	$(CC) -c -o $@ $^

$(PRODUCT)/transaction_ut: $(PRODUCT)/transaction_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/transaction_ut.o: transaction_ut.cpp
	$(CC) -c -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

