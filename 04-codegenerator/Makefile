CLANGCC    = clang++-3.7
CLANGCCF   = -c -g `llvm-config-3.7 --cxxflags` -std=c++14 -fexceptions
CLANGLDF   = `llvm-config-3.7 --cxxflags --ldflags --system-libs --libs core`
GNUCC      = g++
GNUCCF     = -c -g -std=gnu++11
OBJECTS    = obj/absyn.o \
			 obj/lexer.o \
			 obj/parser.o \
			 obj/variable.o \
			 obj/function.o \
			 obj/scope.o \
			 obj/env.o \
			 obj/type_checker.o \
			 obj/code_generator.o \
			 obj/main.o
BNFCDIR    = cpp.build
EXECUTABLE = ccpp
TESTFILE   = test.cpp

$(EXECUTABLE): init $(OBJECTS)
	$(CLANGCC) -o $@ ${OBJECTS} ${CLANGLDF}

clean:
	rm -f obj/type_checker.o obj/code_generator.o obj/main.o

clean-all:
	rm -r obj
	rm -f ${EXECUTABLE}

init:
	mkdir -p obj

obj/absyn.o:
	$(GNUCC) $(GNUCCF) ${BNFCDIR}/Absyn.C -o $@

obj/lexer.o:
	$(GNUCC) $(GNUCCF) ${BNFCDIR}/Lexer.C -o $@

obj/parser.o:
	$(GNUCC) $(GNUCCF) ${BNFCDIR}/Parser.C -o $@

obj/variable.o:
	$(CLANGCC) $(CLANGCCF) variable.cc -o $@

obj/function.o:
	$(CLANGCC) $(CLANGCCF) function.cc -o $@

obj/scope.o:
	$(CLANGCC) $(CLANGCCF) scope.cc -o $@

obj/env.o:
	$(CLANGCC) $(CLANGCCF) env.cc -o $@

obj/type_checker.o:
	$(CLANGCC) $(CLANGCCF) type_checker.cc -o $@

obj/code_generator.o:
	$(CLANGCC) $(CLANGCCF) code_generator.cc -o $@

obj/main.o:
	$(CLANGCC) $(CLANGCCF) main.cc -o $@
