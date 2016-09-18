PRODUCT := $(ABS_PRODUCT)/map/rpc

.PHONY: all clear prepare

all: prepare $(PRODUCT)/server.h $(PRODUCT)/client.h

$(PRODUCT)/server.h $(PRODUCT)/client.h: rpc_spec.json
	jsonrpcstub $^ \
    --cpp-server=NMap::NJsonRPC::TServer \
    --cpp-client=NMap::NJsonRPC::TClient \
    --cpp-server-file=$(PRODUCT)/server.h \
    --cpp-client-file=$(PRODUCT)/client.h

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

