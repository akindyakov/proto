PRODUCT = $(ABS_PRODUCT)/ant/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libant.a

$(PRODUCT)/libant.a: $(PRODUCT)/ant.o
	ar rcs $@ $^

$(PRODUCT)/worker.o: worker.h worker.cpp
	$(CC) -c -o $@ \
    worker.cpp

$(PRODUCT)/ant.o: ant.cpp ant.h
	$(CC) -c -o $@ $<

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

