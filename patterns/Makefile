PRODUCT=product

CC=g++
LIB=
FLAGS=-std=c++11 -Wall

decorator:
	${CC} ${FLAGS} ${LIB} -o ${PRODUCT}/decorator \
	decorator.cpp

doc:
	latex -output-directory=${PRODUCT} -output-format=pdf article.tex

clear:
	rm product -r

prepare:
	mkdir product

