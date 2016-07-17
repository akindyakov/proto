PRODUCT=product

CC=clang++
LIB=
FLAGS=-std=c++11 -Wall

test: prepare
	${CC} ${FLAGS} ${LIB} -o ${PRODUCT}/test \
	test.cpp

ut: prepare
	${CC} ${FLAGS} ${LIB} -o ${PRODUCT}/ut \
	ut.cpp

doc:
	latex -output-directory=${PRODUCT} -output-format=pdf article.tex

clear:
	rm product -r

prepare:
	mkdir --parents product

