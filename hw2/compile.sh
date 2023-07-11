#!/usr/bin/csh -x

bison -d parser.ypp
flex scanner.lex
g++ lex.yy.c parser.tab.cpp output.cpp
