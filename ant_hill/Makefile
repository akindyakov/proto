PRODUCT=product

#CC=g++
CC=clang++
LIB=
FLAGS=-std=c++14 -Wall

app:
	${CC} ${FLAGS} ${LIB} -o ${PRODUCT}/app \
	2d_field.cpp \
	map_symbols.cpp \
	app.cpp \

ut:
	${CC} ${FLAGS} ${LIB} -o ${PRODUCT}/ut \
	ut.cpp

2d_field_ut:
	${CC} ${FLAGS} ${LIB} -o ${PRODUCT}/2d_field_ut \
	2d_field.cpp \
	map_symbols.cpp \
	2d_field_ut.cpp

tdoc:
	latex -output-directory=${PRODUCT} -output-format=pdf article.tex

mdoc:
	pandoc --output ${PRODUCT}/mdoc.rst --to rst doc/mdoc.md

clear:
	rm product -r

prepare:
	mkdir product

