PRODUCT = $(ABS_PRODUCT)/ant/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/libant.a ut

ut: $(PRODUCT)/location_ut $(PRODUCT)/task_ut

$(PRODUCT)/libant.a: $(PRODUCT)/ant.o $(PRODUCT)/task.o
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

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

