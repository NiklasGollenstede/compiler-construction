CLANGCC    = clang++
CLANGCCF   = -c -g -std=c++11 `llvm-config --cflags`
CLANGLDF   = `llvm-config --cxxflags --ldflags --libs core executionengine jit interpreter analysis native bitwriter --system-libs`
GNUCC      = g++
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
	$(CLANGCC) ${CLANGLDF} -o $@ ${OBJECTS}

run: $(EXECUTABLE)
	./${EXECUTABLE} ${TESTFILE}

clean:
	rm -f obj/scope.o obj/env.o obj/type_checker.o obj/main.o

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