PRODUCT := $(ABS_PRODUCT)/map/app

.PHONY: all clear prepare

all: prepare $(PRODUCT)/map

# for client

$(PRODUCT)/map: $(PRODUCT)/main.o $(ABS_PRODUCT)/map/lib/libmap.a
	$(CC) \
    -ljsoncpp \
    -ljsonrpccpp-common \
    -ljsonrpccpp-server \
    -lmicrohttpd \
    $(ABS_PRODUCT)/map/lib/libmap.a \
    -o $@ $^

$(PRODUCT)/main.o: main.cpp $(ABS_PRODUCT)/map/rpc/server.h
	$(CC) -c \
    -o $@ $<

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

