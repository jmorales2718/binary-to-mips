
all: clean RegisterTable.o OpcodeTable.o Instruction.o BinaryParser.o Binary

Binary: Binary.o Instruction.o OpcodeTable.o RegisterTable.o BinaryParser.o
	g++ -o Binary Binary.o OpcodeTable.o BinaryParser.o RegisterTable.o Instruction.o

BinaryParser.o: BinaryParser.cpp OpcodeTable.cpp RegisterTable.h Instruction.h 
	g++ -Wall -c BinaryParser.cpp

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h 
	g++ -Wall -c Instruction.cpp

OpcodeTable.o: OpcodeTable.h 
	g++ -Wall -c OpcodeTable.cpp
	

RegisterTable.o: RegisterTable.h  
	g++ -Wall -c RegisterTable.cpp
clean:
	/bin/rm -f Binary *.o