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
EXECUTABLE = typechecker
TESTFILE   = test.cpp

run: all
	./${EXECUTABLE} ${TESTFILE}

all: $(EXECUTABLE)

clean:
	rm -f obj/* ${EXECUTABLE}
    
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) ${LDF} -o $@ ${OBJECTS}

obj/absyn.o:
	$(CXX) $(CXXF) cpp.build/Absyn.C -o $@

obj/lexer.o:
	$(CXX) $(CXXF) cpp.build/Lexer.C -o $@

obj/parser.o:
	$(CXX) $(CXXF) cpp.build/Parser.C -o $@

obj/scope.o:
	$(CXX) $(CXXF) scope.cc -o $@

obj/env.o:
	$(CXX) $(CXXF) env.cc -o $@

obj/type_checker.o:
	$(CXX) $(CXXF) type_checker.cc -o $@

obj/main.o:
	$(CXX) $(CXXF) main.cc -o $@