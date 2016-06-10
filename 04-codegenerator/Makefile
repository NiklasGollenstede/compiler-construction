CLANGCC    = clang++-3.7
CLANGCCF   = -c -g `llvm-config-3.7 --cxxflags` -std=c++14 -fexceptions
CLANGLDF   = `llvm-config-3.7 --cxxflags --ldflags --system-libs --libs core`
GNUCCF     = -c -g -std=gnu++11
OBJECTS    = obj/absyn.o \
			 obj/lexer.o \
			 obj/parser.o \
			 obj/scope.o \
			 obj/env.o \
			 obj/type_checker.o \
			 obj/llvm_ir_gen.o \
			 obj/main.o
BNFCDIR    = cpp.build
EXECUTABLE = codegen
TESTFILE   = test.cpp

$(EXECUTABLE): init $(OBJECTS) 
	$(CLANGCC) -o $@ ${OBJECTS} ${CLANGLDF}

run: $(EXECUTABLE)
	./${EXECUTABLE} ${TESTFILE}

clean:
	rm -f obj/scope.o obj/env.o obj/type_checker.o obj/llvm_ir_gen.o obj/main.o

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

obj/scope.o:
	$(CLANGCC) $(CLANGCCF) scope.cc -o $@

obj/env.o:
	$(CLANGCC) $(CLANGCCF) env.cc -o $@

obj/type_checker.o:
	$(CLANGCC) $(CLANGCCF) type_checker.cc -o $@

obj/llvm_ir_gen.o:
	$(CLANGCC) $(CLANGCCF) llvm_ir_gen.cc -o $@

obj/main.o:
	$(CLANGCC) $(CLANGCCF) main.cc -o $@