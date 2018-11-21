TARGET = test-g++
CXX = g++

test-g++ : LL.dll main.o
	$(CXX) -o test-g++ LL.dll main.cpp

LL.dll : FileReader.o Grammar.o Parser.o ParsingTable.o Production.o Symbol.o Tokenizer.o
	$(CXX) -shared FileReader.o Grammar.o Parser.o ParsingTable.o Production.o Symbol.o Tokenizer.o -o LL.dll

Symbol.o : Symbol.cpp
	$(CXX) -c -o Symbol.o Symbol.cpp

Production.o : Production.cpp
	$(CXX) -c -o Production.o Production.cpp

FileReader.o : FileReader.cpp
	$(CXX) -c -o FileReader.o FileReader.cpp

Tokenizer.o : Tokenizer.cpp
	$(CXX) -c -o Tokenizer.o Tokenizer.cpp

Grammar.o : Grammar.cpp
	$(CXX) -c -o Grammar.o Grammar.cpp

ParsingTable.o : ParsingTable.cpp
	$(CXX) -c -o ParsingTable.o ParsingTable.cpp

Parser.o : Parser.cpp
	$(CXX) -c -o Parser.o Parser.cpp

clean:
	-rm -f *.o
	-rm -f LL.dll
	-rm -f test-g++*