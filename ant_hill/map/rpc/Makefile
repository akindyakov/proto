PRODUCT := $(ABS_PRODUCT)/map/rpc

.PHONY: all clear prepare

all: prepare $(PRODUCT)/server.h $(PRODUCT)/client.h

$(PRODUCT)/server.h: rpc_spec.json
	jsonrpcstub $< \
    --cpp-server=Map::JsonRPCServer \
    --cpp-server-file=$(PRODUCT)/server.h

$(PRODUCT)/client.h: rpc_spec.json
	jsonrpcstub $< \
    --cpp-client=Map::JsonRPCClient \
    --cpp-client-file=$(PRODUCT)/client.h

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

