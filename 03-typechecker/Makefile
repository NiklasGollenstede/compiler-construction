CXX        = g++
CXXF       = -c -g -std=gnu++11
LDF        = -std=gnu++11
OBJECTS    = obj/absyn.o \
			 obj/lexer.o \
			 obj/parser.o \
			 obj/scope.o \
			 obj/env.o \
			 obj/type_checker.o \
			 obj/main.o
BNFCDIR    = cpp.build
EXECUTABLE = tccpp
TESTFILE   = test.cpp

$(EXECUTABLE): init $(OBJECTS) 
	$(CXX) ${LDF} -o $@ ${OBJECTS}

run: $(EXECUTABLE)
	./${EXECUTABLE} ${TESTFILE}

clean:
	rm -f obj/scope.o obj/env.o obj/type_checker.o obj/main.o

clean-all:
	rm -r obj 
	rm ${EXECUTABLE}

init:
	mkdir -p obj

obj/absyn.o:
	$(CXX) $(CXXF) ${BNFCDIR}/Absyn.C -o $@

obj/lexer.o:
	$(CXX) $(CXXF) ${BNFCDIR}/Lexer.C -o $@

obj/parser.o:
	$(CXX) $(CXXF) ${BNFCDIR}/Parser.C -o $@

obj/scope.o:
	$(CXX) $(CXXF) scope.cc -o $@

obj/env.o:
	$(CXX) $(CXXF) env.cc -o $@

obj/type_checker.o:
	$(CXX) $(CXXF) type_checker.cc -o $@

obj/main.o:
	$(CXX) $(CXXF) main.cc -o $@