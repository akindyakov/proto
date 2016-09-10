PRODUCT := $(ABS_PRODUCT)/app/map

.PHONY: all app clear prepare

all: prepare app

app: $(PRODUCT)/map.o
	$(CC) -o $(PRODUCT)/map \
	$(PRODUCT)/map.o

$(PRODUCT)/map.o: map.cpp
	$(CC) -c -o $(PRODUCT)/map.o \
	map.cpp

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

