PRODUCT := $(ABS_PRODUCT)/ant/app

.PHONY: all clear prepare

all: prepare $(PRODUCT)/ant

$(PRODUCT)/ant: $(PRODUCT)/main.o $(PRODUCT)/args.o
	$(CC) \
    -lcurl \
    -ljsoncpp \
    -ljsonrpccpp-client \
    -ljsonrpccpp-common \
    -lboost_program_options \
    -o $@ $^


$(PRODUCT)/main.o: main.cpp $(ABS_PRODUCT)/map/rpc/client.h
	$(CC) -c \
    -o $@ $<

$(PRODUCT)/args.o: args.cpp args.h
	$(CC) -c \
    -o $@ $<

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

