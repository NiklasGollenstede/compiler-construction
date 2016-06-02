CC = g++
CCFLAGS = -g
FLEX = flex
BISON = bison
LATEX = latex
DVIPS = dvips

all: Testcpp cpp.ps

clean:
	rm -f *.o cpp.dvi cpp.aux cpp.log cpp.ps Testcpp

distclean:
	 rm -f *.o Absyn.C Absyn.H Test.C Parser.C Parser.H Lexer.C Skeleton.C Skeleton.H Printer.C Printer.H cpp.l cpp.y cpp.tex cpp.dvi cpp.aux cpp.log cpp.ps Testcpp Makefile

Testcpp: Absyn.o Lexer.o Parser.o Printer.o Test.o
	@echo "Linking Testcpp..."
	${CC} ${CCFLAGS} *.o -o Testcpp
        
Absyn.o: Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Lexer.C: cpp.l
	${FLEX} -oLexer.C cpp.l

Parser.C: cpp.y
	${BISON} cpp.y -o Parser.C

Lexer.o: Lexer.C Parser.H
	${CC} ${CCFLAGS} -c Lexer.C 

Parser.o: Parser.C Absyn.H
	${CC} ${CCFLAGS} -c Parser.C

Printer.o: Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Skeleton.o: Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -c Skeleton.C

Test.o: Test.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Test.C

cpp.dvi: cpp.tex
	${LATEX} cpp.tex

cpp.ps: cpp.dvi
	${DVIPS} cpp.dvi -o cpp.ps

