//John Morales
//CS301
//BinaryParser.h
//Purpose: Checks a machine language file for correct syntax and and creates a list of instructions


#ifndef BINARYPARSER_H
#define BINARYPARSER_H

using namespace std;

#include <iostream>
#include "Instruction.h"
#include "OpcodeTable.h"
#include "RegisterTable.h"
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <bitset>
#include <iomanip>

class BinaryParser
{
	public:
		//BinaryParser Constructor parses file argument check for correct syntax of
		//the machine languagae file. Then creates a list of the correct instructions.
		BinaryParser(string filename);


		//Returns true if the file specified had correct syntax, if not it returns false.
	    bool isFormatCorrect() {return myFormatCorrect; };


	    // Iterator which returns the next Instruction in the list of Instructions.
		Instruction getNextInstruction();


	private:
		vector<Instruction> myInstructions;
		bool myFormatCorrect;
		RegisterTable registers;
		OpcodeTable opcodes;
		int myLabelAddress;
		int myIndex;						//iterator index
		int lBits;							//used to count the non-whitespace bits of a  line
		const static int rBitLen = 5;  		//number of bits for a register	
		const static int opBitLen = 6; 		//number of bits in opcode field
		const static int immBitLen = 16; 	//number of bits in immeadiate field	
		const static int jImmBitLen = 26;  	// Number of bits in jump immeadiate
		const static int instLen = 32;		//Number of bits in instruction
		const static int shiftLen = 5; 		// Number of bits in shift
		const static int rOperandCnt = 4;	// Number of Operands in rInstruction
		const static int iRegisterCnt = 2;	//Number of Registers in I instruction
		const static int jOperandCnt = 1;	// Amount of Operands in Jump Instruction
		const static string emptyReg;  		// Represents an empty register (ei."00000")
		const static string emptyOpcode;	// Represents an empty Opcode field (ei."000000")

		
		//Gets the opcode field of the current instruction, this will be used to find out
		//the instruction type.
		void getOpcodeBin(string line, string &opcode);


		//Gets the function field of the instruction so that RType instructions can
		//be parsed properly
		void getFunctionField(string line, string &funct_field);


		//Decomposes a line of machine language code into operands and counting the number of
		//operands. Specifically this handles the Rtype instruction.
		void getTokensR(string line, string *operand, int &num_operands);


		//Decomposes a line of machine language code into operands and counting the number of
		//operands. Specifically this handles the Itype instruction.
		void getTokensI(string line, string *operand, int &num_operands);


		//Decomposes a line of machine language code into operands and counting the number of
		//operands. Specifically this handles the Load word, load byte, etc. It is meant
		//to parse an instruction with the immediate before the second register.
		void getTokImmPos1(string line, string *operand, int &num_operands);


		//Decomposes a line of machine language code into operands and counting the number of
		//operands. Specifically this handles the Jtype instruction.
		void getTokensJ(string line, string *operand, int &num_operands);


		//Using opcode, string representing operands, and the operand count, this method
		//creates an Instruction from those values after checking for correct synatax.
		bool getOperands(Instruction &i, Opcode o, string *operand, int operand_count);
		

		//Using opcode, string representing operands, and the operand count, this method
		//creates an Instruction from those values after checking for correct synatax.
		//Specifically this method, handles the case of R-type instructions.
		bool getOperandsR(Instruction &i, Opcode o, string *operand, int operand_count);
		

		//Using opcode, string representing operands, and the operand count, this method
		//creates an Instruction from those values after checking for correct synatax.
		//Specifically this method, handles the case of I-type instructions.
		bool getOperandsI(Instruction &i, Opcode o, string *operand, int operand_count);
		

		//Using opcode, string representing operands, and the operand count, this method
		//creates an Instruction from those values after checking for correct synatax.
		//Specifically this method, handles the case of J-type instructions.
		bool getOperandsJ(Instruction &i, Opcode o, string *operand, int operand_count);
		

		//Taking an binary value as a parameter, it converts this value to its decimal
		//representation.
		int  binaryToDecimal(string s);
		

		//Taking a binary value as a parameter, it converts this value into its hexadecimal
		//representation and returns that represenation as a string.
		string decToHex(int n);
		

		//Taking an intger as a parameter, it determines the number of digits contained
		//in that integer and returns the number of digits.
		int numDigits(int n);


		//given a single character, checks that the character is a 0 or 1 returning true
		//if it is and false otherwise.
		bool isBinaryChar(char c){return (c== '0' || c == '1'); };
		

		//given a single character, reuturns true if the character is a space or tab
		//character, returns false otherwise.
		bool isWhitespace(char c) { return(c== ' ' || c == '\t'); };
		

		//Taking an Instruction as a parameter, it decomposes this value and creates a
		// string representation of the mips instruction paramter. It then returns this 
		// string representation.
		string decode(Instruction i);
		

		// Taking a Register value as a parameter, it converts this value to a string, 
		// and prepends a dollar sign to it to follow MIPS convention
		string cvt2MipsReg(Register reg);
		

		//Taking an integer value as a parameter, it converts this value into a string
		// and then returns the new string representation.
		string intToString(int n);


		//Converts a string to its integer representation returning a that integer value
		int stringToInt(string s);
		

		//Taking a line of machine language code as a parameter, it checks that each bit
		//is either a 0 or 1 and makes sure that their are 32 consectutive bits.
		bool checkFor32Consec(string s);
};
#endif