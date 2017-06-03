PRODUCT = $(ABS_PRODUCT)/map/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libmap.a ut

$(PRODUCT)/libmap.a: $(PRODUCT)/2d_field.o $(PRODUCT)/world.o $(PRODUCT)/map_symbols.o $(PRODUCT)/direction.o $(PRODUCT)/snake.o $(PRODUCT)/spiral.o $(PRODUCT)/find_a_way.o
	ar rcs $@ $^

ut: $(PRODUCT)/2d_field_ut $(PRODUCT)/direction_ut $(PRODUCT)/world_ut $(PRODUCT)/snake_ut $(PRODUCT)/spiral_ut $(PRODUCT)/find_a_way_ut

$(PRODUCT)/2d_field.o: 2d_field.h 2d_field.cpp
	$(CC) -c -o $(PRODUCT)/2d_field.o \
    2d_field.cpp

$(PRODUCT)/map_symbols.o: map_symbols.h map_symbols.cpp
	$(CC) -c -o $(PRODUCT)/map_symbols.o \
    map_symbols.cpp

$(PRODUCT)/direction.o: direction.cpp direction.h $(PRODUCT)/2d_field.o 2d_field.h
	$(CC) -c -o $@ $<

$(PRODUCT)/world.o: world.cpp world.h direction.h map_symbols.h 2d_field.h
	$(CC) -c -o $@ $<

$(PRODUCT)/snake.o: snake.cpp snake.h world.h chain.h
	$(CC) -c -o $@ $<

$(PRODUCT)/2d_field_ut.o: 2d_field_ut.cpp
	$(CC) -c -o $@ $^

$(PRODUCT)/direction_ut.o: direction_ut.cpp
	$(CC) -c -o $@ $<

$(PRODUCT)/world_ut.o: world_ut.cpp
	$(CC) -c -o $@ $<

$(PRODUCT)/snake_ut.o: snake_ut.cpp
	$(CC) -c -o $@ $<

$(PRODUCT)/2d_field_ut: $(PRODUCT)/2d_field_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/direction_ut: $(PRODUCT)/direction_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/world_ut: $(PRODUCT)/world_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

$(PRODUCT)/snake_ut: $(PRODUCT)/snake_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

# spiral
$(PRODUCT)/spiral.o: spiral.cpp spiral.h 2d_field.h
	$(CC) -c -o $@ $<

$(PRODUCT)/spiral_ut.o: spiral_ut.cpp spiral.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/spiral_ut: $(PRODUCT)/spiral_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

# find a way
$(PRODUCT)/find_a_way.o: find_a_way.cpp find_a_way.h find_a_way_impl.h 2d_field.h direction.h
	$(CC) -c -o $@ $<

$(PRODUCT)/find_a_way_ut.o: find_a_way_ut.cpp find_a_way.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/find_a_way_ut: $(PRODUCT)/find_a_way_ut.o $(PRODUCT)/libmap.a
	$(CC) -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

