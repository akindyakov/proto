PRODUCT = $(ABS_PRODUCT)/lib/str

.PHONY: all ut clear prepare

all: prepare ut

ut: $(PRODUCT)/to_string_ut

$(PRODUCT)/to_string_ut.o: to_string_ut.cpp to_string.h
	$(CC) -c -o $@ $<

$(PRODUCT)/to_string_ut: $(PRODUCT)/to_string_ut.o
	$(CC) -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)
