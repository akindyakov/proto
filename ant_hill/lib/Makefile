PRODUCT := "$(ABS_PRODUCT)/lib"

TARGETS := traits str

.PHONY: all prepare $(TARGETS)

all: prepare $(TARGETS) ut

ut: $(PRODUCT)/validate_ut

$(TARGETS):
	$(MAKE) --directory=$@

$(PRODUCT)/validate_ut.o: validate_ut.cpp validate.h
	$(CC) -c -o $@ $<

$(PRODUCT)/validate_ut: $(PRODUCT)/validate_ut.o
	$(CC) -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

