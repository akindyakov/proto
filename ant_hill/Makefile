PRODUCT=product

CC=g++
LIB=
FLAGS=-std=c++11 -Wall

ut:
	${CC} ${FLAGS} ${LIB} -o ${PRODUCT}/ut \
	ut.cpp

tdoc:
	latex -output-directory=${PRODUCT} -output-format=pdf article.tex

mdoc:
	pandoc --output ${PRODUCT}/mdoc.rst --to rst doc/mdoc.md

clear:
	rm product -r

prepare:
	mkdir product

