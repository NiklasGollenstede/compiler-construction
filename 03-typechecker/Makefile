CXX        = g++
CXXF       = -c -g -std=gnu++11
LDF        = -std=gnu++11
OBJECTS    = absyn.o lexer.o parser.o scope.o env.o type_checker.o main.o
EXECUTABLE = typechecker
TESTFILE   = test.cpp

run: all
	./${EXECUTABLE} ${TESTFILE}

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -f obj/*.o ${EXECUTABLE}
    
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) ${LDF} -o $@ $(patsubst %,obj/%,$(OBJECTS))

absyn.o:
	$(CXX) $(CXXF) cpp.build/Absyn.C -o obj/$@

lexer.o:
	$(CXX) $(CXXF) cpp.build/Lexer.C -o obj/$@

parser.o:
	$(CXX) $(CXXF) cpp.build/Parser.C -o obj/$@

scope.o:
	$(CXX) $(CXXF) scope.cc -o obj/$@

env.o:
	$(CXX) $(CXXF) env.cc -o obj/$@

type_checker.o:
	$(CXX) $(CXXF) type_checker.cc -o obj/$@

main.o:
	$(CXX) $(CXXF) main.cc -o obj/$@