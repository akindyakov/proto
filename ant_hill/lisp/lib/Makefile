PRODUCT = $(ABS_PRODUCT)/lisp/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/liblisp.a ut

$(PRODUCT)/liblisp.a: $(PRODUCT)/cell.o
	ar rcs $@ $^

$(PRODUCT)/cell.o: cell.cpp cell.h
	$(CC) -c -o $@ $<

ut: $(PRODUCT)/cell_ut

$(PRODUCT)/cell_ut.o: cell_ut.cpp
	$(CC) -c -o $@ $^

$(PRODUCT)/cell_ut: $(PRODUCT)/cell_ut.o $(PRODUCT)/liblisp.a
	$(CC) -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

