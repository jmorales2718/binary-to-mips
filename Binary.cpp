#include "BinaryParser.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	BinaryParser* bParser;

	if(argc < 2)
	{
		cerr << "Please specify an Machine Language file to translate." << endl;
		exit(1);
	}

	bParser = new BinaryParser(argv[1]);

	if(bParser->isFormatCorrect() == false)
	{
		cerr << "Format of input file is not correct." << endl;
		exit(1);
	}

	Instruction i;

	i = bParser->getNextInstruction();
	while(i.getOpcode() != UNDEFINED)
	{
		cout << i.getDecoding() << endl;
		i = bParser->getNextInstruction();
	}

	delete bParser;
}
