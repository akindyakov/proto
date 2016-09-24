PRODUCT := $(ABS_PRODUCT)/map/app

.PHONY: all clear prepare

all: prepare $(PRODUCT)/map

$(PRODUCT)/map: $(PRODUCT)/main.o $(ABS_PRODUCT)/map/lib/libmap.a $(PRODUCT)/args.o
	$(CC) \
    -ljsoncpp \
    -ljsonrpccpp-common \
    -ljsonrpccpp-server \
    -lmicrohttpd \
    -lboost_program_options \
    $(ABS_PRODUCT)/map/lib/libmap.a \
    -o $@ $^

$(PRODUCT)/main.o: main.cpp $(ABS_PRODUCT)/map/rpc/server.h
	$(CC) -c \
    -o $@ $<

$(PRODUCT)/args.o: args.cpp args.h
	$(CC) -c \
    -o $@ $<

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

