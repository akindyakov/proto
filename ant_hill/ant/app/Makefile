PRODUCT := $(ABS_PRODUCT)/ant/app

.PHONY: all clear prepare

all: prepare $(PRODUCT)/ant

$(PRODUCT)/ant: $(PRODUCT)/main.o
	$(CC) \
    -lcurl \
    -ljsoncpp \
    -ljsonrpccpp-client \
    -ljsonrpccpp-common \
    -o $@ $^


$(PRODUCT)/main.o: main.cpp $(ABS_PRODUCT)/map/rpc/client.h
	$(CC) -c \
    -o $@ $<

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

