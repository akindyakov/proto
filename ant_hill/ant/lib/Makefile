PRODUCT = $(ABS_PRODUCT)/ant/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libant.a ut

ut: $(PRODUCT)/location_ut $(PRODUCT)/task_ut $(PRODUCT)/movements_ut $(PRODUCT)/creative_ut

$(PRODUCT)/libant.a: $(PRODUCT)/location.o $(PRODUCT)/task.o $(PRODUCT)/movements.o $(PRODUCT)/creative.o
	ar rcs $@ $^

$(PRODUCT)/location.o: location.cpp location.h
	$(CC) -c -o $@ $<

$(PRODUCT)/location_ut: $(PRODUCT)/location_ut.o $(PRODUCT)/libant.a
	$(CC) -o $@ $^ $(ABS_PRODUCT)/map/lib/libmap.a

$(PRODUCT)/location_ut.o: location_ut.cpp location.h
	$(CC) -c -o $@ $<

$(PRODUCT)/task.o: task.cpp task.h
	$(CC) -c -o $@ $<

$(PRODUCT)/task_ut.o: task_ut.cpp task.h
	$(CC) -c -o $@ $<

$(PRODUCT)/task_ut: $(PRODUCT)/task_ut.o $(PRODUCT)/libant.a
	$(CC) -o $@ $^ $(ABS_PRODUCT)/map/lib/libmap.a

$(PRODUCT)/movements.o: movements.cpp movements.h
	$(CC) -c -o $@ $<

$(PRODUCT)/movements_ut.o: movements_ut.cpp movements.h
	$(CC) -c -o $@ $<

$(PRODUCT)/movements_ut: $(PRODUCT)/movements_ut.o $(PRODUCT)/libant.a
	$(CC) -o $@ $^ $(ABS_PRODUCT)/map/lib/libmap.a

$(PRODUCT)/creative.o: creative.cpp creative.h
	$(CC) -c -o $@ $<

$(PRODUCT)/creative_ut.o: creative_ut.cpp creative.h
	$(CC) -c -o $@ $<

$(PRODUCT)/creative_ut: $(PRODUCT)/creative_ut.o $(PRODUCT)/libant.a
	$(CC) -o $@ $^ $(ABS_PRODUCT)/map/lib/libmap.a

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

