PRODUCT := $(ABS_PRODUCT)/map/app

.PHONY: all clear prepare

all: prepare $(PRODUCT)/map

$(PRODUCT)/map: $(PRODUCT)/main.o $(PRODUCT)/args.o $(PRODUCT)/map_server.o $(ABS_PRODUCT)/map/lib/libmap.a
	$(CC) \
    -ljsoncpp \
    -ljsonrpccpp-common \
    -ljsonrpccpp-server \
    -lmicrohttpd \
    -lboost_program_options \
    -o $@ $^

$(PRODUCT)/main.o: main.cpp
	$(CC) -c \
    -o $@ $<

$(PRODUCT)/args.o: args.cpp args.h
	$(CC) -c \
    -o $@ $<

$(PRODUCT)/map_server.o: map.cpp map_server.h $(ABS_PRODUCT)/map/rpc/server.h
	$(CC) -c \
    -o $@ $<

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

