PRODUCT := product
CXX := g++ -std=c++14

.PHONY: all clear prepare

all: prepare $(PRODUCT)/a_ld $(PRODUCT)/o_ld

$(PRODUCT)/base.o: base.cpp base.h
	$(CXX) -c -o $@ $<

$(PRODUCT)/first.o: first.cpp first.h
	$(CXX) -c -o $@ $<

$(PRODUCT)/second.o: second.cpp second.h
	$(CXX) -c -o $@ $<

$(PRODUCT)/libsecond.a: $(PRODUCT)/second.o $(PRODUCT)/base.o
	ar rcs $@ $^

$(PRODUCT)/libfirst.a: $(PRODUCT)/first.o $(PRODUCT)/base.o
	ar rcs $@ $^

$(PRODUCT)/main.o: main.cpp
	$(CXX) -c -o $@ $^

$(PRODUCT)/a_ld: $(PRODUCT)/main.o $(PRODUCT)/libfirst.a $(PRODUCT)/libsecond.a
	$(CXX) -o $@ $^

$(PRODUCT)/o_ld: $(PRODUCT)/main.o $(PRODUCT)/first.o $(PRODUCT)/second.o $(PRODUCT)/base.o
	$(CXX) -o $@ $^

clear:
	rm --recursive $(PRODUCT)

prepare:
	mkdir --parents $(PRODUCT)

