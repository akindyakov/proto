PRODUCT = $(ABS_PRODUCT)/map/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libmap.a ut

$(PRODUCT)/libmap.a: $(PRODUCT)/2d_field.o $(PRODUCT)/world.o $(PRODUCT)/map_symbols.o $(PRODUCT)/transaction.o $(PRODUCT)/snake.o
	ar rcs $@ $^

$(PRODUCT)/2d_field.o: 2d_field.h 2d_field.cpp
	$(CC) -c -o $(PRODUCT)/2d_field.o \
    2d_field.cpp

$(PRODUCT)/map_symbols.o: map_symbols.h map_symbols.cpp
	$(CC) -c -o $(PRODUCT)/map_symbols.o \
    map_symbols.cpp

$(PRODUCT)/transaction.o: transaction.cpp transaction.h $(PRODUCT)/2d_field.o 2d_field.h
	$(CC) -c -o $@ $<

$(PRODUCT)/world.o: world.cpp world.h transaction.h map_symbols.h 2d_field.h
	$(CC) -c -o $@ $<

$(PRODUCT)/snake.o: snake.cpp snake.h world.h chain.h
	$(CC) -c -o $@ $<

ut: $(PRODUCT)/2d_field_ut $(PRODUCT)/transaction_ut $(PRODUCT)/world_ut $(PRODUCT)/snake_ut

$(PRODUCT)/2d_field_ut.o: 2d_field_ut.cpp
	$(CC) -c -o $@ $^

$(PRODUCT)/transaction_ut.o: transaction_ut.cpp
	$(CC) -c -o $@ $<

$(PRODUCT)/world_ut.o: world_ut.cpp
	$(CC) -c -o $@ $<

$(PRODUCT)/snake_ut.o: snake_ut.cpp
	$(CC) -c -o $@ $<

$(PRODUCT)/2d_field_ut: $(PRODUCT)/2d_field_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/transaction_ut: $(PRODUCT)/transaction_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/world_ut: $(PRODUCT)/world_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/snake_ut: $(PRODUCT)/snake_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

