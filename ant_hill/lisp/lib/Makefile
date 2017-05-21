PRODUCT = $(ABS_PRODUCT)/lisp/lib

.PHONY: all ut clear prepare

all: prepare $(PRODUCT)/liblisp.a ut

$(PRODUCT)/liblisp.a: $(PRODUCT)/cell.o $(PRODUCT)/math_functions.o $(PRODUCT)/basic_functions.o $(PRODUCT)/parser.o $(PRODUCT)/context.o $(PRODUCT)/environment.o
	ar rcs $@ $^

# all ut
ut: $(PRODUCT)/cell_ut $(PRODUCT)/math_functions_ut $(PRODUCT)/basic_functions_ut $(PRODUCT)/parser_ut $(PRODUCT)/environment_ut $(PRODUCT)/context_ut

# cell
$(PRODUCT)/cell.o: cell.cpp cell.h
	$(CC) -c -o $@ $<

ut: $(PRODUCT)/cell_ut

$(PRODUCT)/cell_ut.o: cell_ut.cpp cell.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/cell_ut: $(PRODUCT)/cell_ut.o $(PRODUCT)/liblisp.a
	$(CC) -o $@ $^

# math_functions
$(PRODUCT)/math_functions.o: math_functions.cpp math_functions.h cell.h
	$(CC) -c -o $@ $<

$(PRODUCT)/math_functions_ut.o: math_functions_ut.cpp math_functions.h cell.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/math_functions_ut: $(PRODUCT)/math_functions_ut.o $(PRODUCT)/liblisp.a
	$(CC) -o $@ $^

# basic_functions
$(PRODUCT)/basic_functions.o: basic_functions.cpp basic_functions.h cell.h
	$(CC) -c -o $@ $<

$(PRODUCT)/basic_functions_ut.o: basic_functions_ut.cpp basic_functions.h cell.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/basic_functions_ut: $(PRODUCT)/basic_functions_ut.o $(PRODUCT)/liblisp.a
	$(CC) -o $@ $^

# parser
$(PRODUCT)/parser.o: parser.cpp parser.h cell.h
	$(CC) -c -o $@ $<

$(PRODUCT)/parser_ut.o: parser_ut.cpp parser.h cell.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/parser_ut: $(PRODUCT)/parser_ut.o $(PRODUCT)/liblisp.a
	$(CC) -o $@ $^

# environment
$(PRODUCT)/environment.o: environment.cpp environment.h cell.h
	$(CC) -c -o $@ $<

$(PRODUCT)/environment_ut.o: environment_ut.cpp environment.h cell.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/environment_ut: $(PRODUCT)/environment_ut.o $(PRODUCT)/liblisp.a
	$(CC) -o $@ $^

# context
$(PRODUCT)/context.o: context.cpp context.h cell.h parser.h
	$(CC) -c -o $@ $<

$(PRODUCT)/context_ut.o: context_ut.cpp context.h $(SRC_PATH)/tools/tests/ut.h
	$(CC) -c -o $@ $<

$(PRODUCT)/context_ut: $(PRODUCT)/context_ut.o $(PRODUCT)/liblisp.a
	$(CC) -o $@ $^


# prepare/clear
clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

