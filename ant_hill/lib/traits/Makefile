PRODUCT = $(ABS_PRODUCT)/lib/traits

.PHONY: all ut clear prepare

all: prepare ut

ut: $(PRODUCT)/is_printable_ut

$(PRODUCT)/is_printable_ut.o: is_printable_ut.cpp is_printable.h
	$(CC) -c -o $@ $<

$(PRODUCT)/is_printable_ut: $(PRODUCT)/is_printable_ut.o
	$(CC) -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)
