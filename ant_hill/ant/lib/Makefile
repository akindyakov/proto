PRODUCT = $(ABS_PRODUCT)/ant/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libant.a $(PRODUCT)/ant_ut

$(PRODUCT)/libant.a: $(PRODUCT)/ant.o
	ar rcs $@ $^

$(PRODUCT)/worker.o: worker.h worker.cpp
	$(CC) -c -o $@ \
    worker.cpp

$(PRODUCT)/ant.o: ant.cpp ant.h
	$(CC) -c -o $@ $<

$(PRODUCT)/ant_ut: $(PRODUCT)/ant_ut.o $(PRODUCT)/libant.a
	$(CC) -o $@ $^ $(ABS_PRODUCT)/map/lib/libmap.a

$(PRODUCT)/ant_ut.o: ant_ut.cpp
	$(CC) -c -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

