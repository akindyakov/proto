PRODUCT = $(ABS_PRODUCT)/ant/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libant.a

$(PRODUCT)/libant.a: $(PRODUCT)/worker.o
	ar rcs $@ $^

$(PRODUCT)/worker.o: worker.h worker.cpp
	$(CC) -c -o $@ \
    worker.cpp

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

