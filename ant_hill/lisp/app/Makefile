PRODUCT := $(ABS_PRODUCT)/lisp/app

.PHONY: all clear prepare

all: prepare $(PRODUCT)/atlant

$(PRODUCT)/atlant: $(PRODUCT)/main.o $(PRODUCT)/args.o $(ABS_PRODUCT)/lisp/lib/liblisp.a
	$(CC) \
    -lboost_program_options \
    -o $@ $^

$(PRODUCT)/main.o: main.cpp
	$(CC) -c \
    -o $@ $<

$(PRODUCT)/args.o: args.cpp args.h
	$(CC) -c \
    -o $@ $<

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)
