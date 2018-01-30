//John Morales
//CS301
//BinaryParser.cpp
//Purpose: Checks a machine language file for correct syntax and and creates a list of instructions


#include "BinaryParser.h"
const string BinaryParser::emptyReg = "00000";
const string BinaryParser::emptyOpcode = "000000";

//BinaryParser Constructor parses file argument check for correct syntax of
//the machine languagae file. Then creates a list of the correct instructions.

BinaryParser::BinaryParser(string filename)
{
	Instruction i;
	myFormatCorrect = true;
	lBits = 0;

	ifstream infile;
	infile.open(filename.c_str());
	if(infile.bad())
	{
		myFormatCorrect = false;
	}
	else
	{
		string line;
		while( getline(infile, line))
		{
			string opcode("");				
			string operand[80];
			string funct_field(""); // will store function field for R-type Instructions
			int operand_count = 0;

			getOpcodeBin(line, opcode);	
			getFunctionField(line, funct_field);

			if(!checkFor32Consec(line))
			{	
				myFormatCorrect = false;
				break;
			}
			Opcode o = opcodes.getOpcode(opcode, funct_field);
			if(o == UNDEFINED)
			{
				myFormatCorrect = false;
				break;
			}

			InstType rij = opcodes.getInstType(o);
			if(rij == RTYPE)
			{
				getTokensR(line, operand, operand_count);	
			}
			else if(rij == ITYPE)
			{	
				//handles the special case of load and store instructions with an
				//immeadiate before their second register.
				if(opcodes.numOperands(o) == 3 && opcodes.IMMposition(o) == 1)
				{
					getTokImmPos1(line, operand, operand_count);
				}
				else
				{
					getTokensI(line, operand, operand_count);
				}	
			}
			else
			{
				getTokensJ(line, operand, operand_count);
			}

			bool success = getOperands(i, o, operand, operand_count);
			if(!success)
			{ 
				myFormatCorrect = false;
				break;	
			}
			
			string decoding = decode(i);
			i.setDecoding(decoding);
			
			myInstructions.push_back(i);		
		}
	}
	myIndex = 0;
}


// Iterator which returns the next Instruction in the list of Instructions.
Instruction BinaryParser::getNextInstruction()
{
	if(myIndex < (int)(myInstructions.size()))
	{
		myIndex++;
		return myInstructions[myIndex - 1];
	}
	Instruction i;
	return i;
}


//Gets the opcode field of the current instruction, this will be used to find out
//the instruction type.
void BinaryParser::getOpcodeBin(string line, string &opcode)
{
	int len = line.length();
	int p = 0;		//Position in line 			
	int lBits = 0;		

	while(p < len && isWhitespace(line.at(p)))
	{
		p++;
	}
	while(p < len && !isWhitespace(line.at(p)) && lBits < opBitLen )
	{
		opcode = opcode + line.at(p);
		lBits++;
		p++;
	}
}


//Gets the function field of the instruction so that RType instructions can
//be parsed properly
void BinaryParser::getFunctionField(string line, string &funct_field)
{
	int len = line.length();
	int p = 0;		//Position in line 			
	int lBits = 0;		

	while(p < len && isWhitespace(line.at(p)))
	{
	 p++;
	}

	while(p < len && !isWhitespace(line.at(p)) && lBits < opBitLen )
	{
		lBits++;
		p++;
	}
	//moves past operands to get to function field
	for (int i = 0; i < rOperandCnt; i++)
	{	
		lBits = 0;
		while(p < len && !isWhitespace(line.at(p)) && lBits < rBitLen)
		{ 
			lBits++;
			p++;
 		}
 	}

 	lBits = 0;
 	while(p < len && !isWhitespace(line.at(p)) && lBits < opBitLen)
 	{
 		funct_field = funct_field + line.at(p);
 		lBits++;
 		p++;
 	}

}


//Decomposes a line of machine language code into operands and counting the number of
//operands. Specifically this handles the Rtype instruction.
void BinaryParser::getTokensR(string line, string *operand, int &num_operands)
{
	int p = 0; 					//position in line
	int len = line.length();

	while (p < len && isWhitespace(line.at(p)))
	{
		p++;
	}
	
	lBits = 0;
	while (p < len && !isWhitespace(line.at(p)) && lBits < opBitLen)
	{	
		lBits++;
		p++;
	}

	int counter = 0;
	for (int i = 0; i < rOperandCnt; i++)
	{
		
		lBits = 0;
		bool flag = false;  //will be used to determine whether numOperands will be increased
		while(p < len && !isWhitespace(line.at(p)) && lBits < rBitLen)
		{
			operand[i] = operand[i] + line.at(p); 
			lBits++;
			p++;
 		}
 		if(operand[i] == emptyReg)
 		{
 			flag = false;
 		}
 		else
 		{
 			flag = true;
 		}
		if(flag == true)
		{
			counter++;
			num_operands++;
		}
	}
	
}


//Decomposes a line of machine language code into operands and counting the number of
//operands. Specifically this handles the Itype instruction.
void BinaryParser::getTokensI(string line, string *operand, int &num_operands)
{
	int len = line.length();
	int p = 0;

	while (p < len && isWhitespace(line.at(p)))
	{
		p++;
	}

	lBits = 0;
	while (p < len && !isWhitespace(line.at(p)) && lBits < opBitLen)
	{	
		lBits++;
		p++;
	}

	int i;
	bool flag = false; //will be used to determine whether numOperands will be increased
	for (i = 0; i < iRegisterCnt; i++)
	{
		lBits = 0;
		while(p < len && !isWhitespace(line.at(p)) && lBits < rBitLen)
		{
			operand[i] = operand[i] + line.at(p);
			lBits++;
			p++;
		}
		//if operand is all 0's don't count it in num operands
		if(operand[i] == emptyReg)
 		{
 			flag = false;
 		}
 		else
 		{
 			flag = true;
 		}
		if(flag == true)
		{
			num_operands++;
		}
	}	
	flag = false;
	lBits = 0;
	while(p < len && !isWhitespace(line.at(p)) && lBits < immBitLen)
	{
		operand[i] = operand[i] + line.at(p);
		lBits++;
		p++;
	
	}
	num_operands++;
}

//Decomposes a line of machine language code into operands and counting the number of
//operands. Specifically this handles the Load word, load byte, etc. It is meant
//to parse an instruction with the immediate before the second register.
void BinaryParser::getTokImmPos1(string line, string *operand, int &num_operands)
{
	int len = line.length();
	int p = 0;  //position in line
	string rsTemp = "";

	while (p < len && isWhitespace(line.at(p)))
	{
		p++;
	}
	lBits = 0;
	while (p < len && !isWhitespace(line.at(p)) && lBits < opBitLen)
	{	
		lBits++;
		p++;
	}
	int i = 0;
	int ctrl = 0; 	    //Will determine end of while 
	bool flag = false;  //will be used to determine whether numOperands will be increased
	while(ctrl < iRegisterCnt)
	{
		lBits = 0;
		while(p < len && !isWhitespace(line.at(p)) && lBits < rBitLen)
		{
			if(i == 1)
			{
				rsTemp = rsTemp + line.at(p);
			}
			else
			{
				operand[i] = operand[i] + line.at(p);
			}
			lBits++;
			p++;
		}
		if(i < iRegisterCnt-1)
		{
			flag = true;
			i++;
		}
		if(flag == true)
		{
			num_operands++;
		}
		ctrl++;
	}
	flag = false;
	lBits = 0;

	while(p < len && !isWhitespace(line.at(p)) && lBits < immBitLen)
	{
		operand[i] = operand[i] + line.at(p);
		if(isBinaryChar(line.at(p)))
		{
			flag = true;
		}
		else
		{
			flag = false;
			break;
		}
		lBits++;
		p++;
	
	}
	if(flag == true)
	{
		num_operands++;
	}
	else
	{
	 	cerr << "immeadiate did not contain all binary characters.";
	 	cerr << "Please input an immeadiate which only contains binary characters." << endl;
	}
	if(i < iRegisterCnt)
	{
		i++;
		operand[i] = rsTemp;
	}

}


//Decomposes a line of machine language code into operands and counting the number of
//operands. Specifically this handles the Jtype instruction.
void BinaryParser::getTokensJ(string line, string *operand, int &num_operands)
{
	int len = line.length();
	int p = 0;
	while (p < len && isWhitespace(line.at(p)))
	{
		p++;
	}
	int lBits = 0;
	while (p < len && !isWhitespace(line.at(p)) && lBits < opBitLen)
	{	
		lBits++;
		p++;
	}
	bool flag = false;
	lBits = 0;
	int i = 0;
	while (p < len && !isWhitespace(line.at(p)) && lBits < jImmBitLen)
	{
		operand[i] = operand[i] + line.at(p);
		if(isBinaryChar(line.at(p)))
		{
			flag = true;
		}
		else
		{
			flag = false;
			break;
		}
		lBits++;
		p++;
	}
	if(flag == true)
	{
		num_operands++;
	}
	else
	{
	 cerr << "immeadiate did not contain all binary characters.";
	 cerr << "Please input an immeadiate which only contains binary characters." << endl;
	}
}	


//Using opcode, string representing operands, and the operand count, this method
//creates an Instruction from those values after checking for correct synatax.
bool BinaryParser::getOperands(Instruction &i, Opcode o, string *operand, int operand_count)
{
	if(operand_count != opcodes.numOperands(o))
	{
		return false;
	}
	
	if(opcodes.getInstType(o) == RTYPE)
	{
		return getOperandsR(i,o,operand,operand_count);
	}

	else if(opcodes.getInstType(o) == ITYPE)
	{
		return getOperandsI(i,o,operand,operand_count);
	}

	else
	{
		return getOperandsJ(i,o,operand,operand_count);
	}	

	return false;
}


//Using opcode, string representing operands, and the operand count, this method
//creates an Instruction from those values after checking for correct synatax.
//Specifically this method, handles the case of R-type instructions.
bool BinaryParser::getOperandsR(Instruction &i, Opcode o, string *operand, int operand_count)
{
	int rs, rt, rd;
	string imm = "";
	rs = rt = rd = NumRegisters;

	int rs_p = opcodes.RSposition(o);
	int rt_p = opcodes.RTposition(o);
	int rd_p = opcodes.RDposition(o);
	int imm_p = opcodes.IMMposition(o);


	int j = 0; //will be used to determine location of each operand field

	//if the binary contains empty operands j will be incremented to skip them
	while(operand[j] == emptyReg)
	{
		j++;
	}
	//rs is the first register in the binary encoding of Rtype so it is acessed first while j is at its lowest
	if(rs_p != -1)
	{

		rs = registers.getNum(operand[j]);
		if(rs == NumRegisters)
		{
			return false;
		}
		j++;
	
	}
	
	if(rt_p != -1)
	{
		rt = registers.getNum(operand[j]);
		if(rt == NumRegisters)
		{
			return false;
		}
		j++;
	}
	if(rd_p != -1)
	{
		rd = registers.getNum(operand[j]);
		if(rd == NumRegisters)
		{
			return false;
		}
		j++;
	}

	if(imm_p != -1)
	{
			imm = operand[j];
			int immLen = imm.length();
			if(immLen != immBitLen && immLen != jImmBitLen && immLen != shiftLen)
			{
				return false;	
			}
	}

	int immDec = binaryToDecimal(imm);
	i.setValues(o, rs, rt, rd, immDec);	
	return true;


}


//Using opcode, string representing operands, and the operand count, this method
//creates an Instruction from those values after checking for correct synatax.
//Specifically this method, handles the case of I-type instructions.
bool BinaryParser::getOperandsI(Instruction &i, Opcode o, string *operand, int operand_count)
{
	int rs, rt, rd;
	int immInt = 0;
	string imm = "";
	rs = rt =  rd = NumRegisters;

	int rs_p = opcodes.RSposition(o);
	int rt_p = opcodes.RTposition(o);;
	int imm_p = opcodes.IMMposition(o);


	int j = 0;
	while(operand[j] == emptyReg)
	{
		j++;
	}
	//handles case of load and store instructions with immeadiate before second register
	if(opcodes.numOperands(o) == 3 && opcodes.IMMposition(o) == 1)
	{ 
		if(rs_p != -1)
		{

			rs = registers.getNum(operand[j]);
			if(rs == NumRegisters)
			{
				return false;
			}
			j++;
		
		}
		// in load and store instructions imm comes before second register so it is handled second here
		if(imm_p != -1)
		{

			imm = operand[j];
			int immLen = imm.length();
			if(immLen != immBitLen && immLen != jImmBitLen && immLen != shiftLen)
			{
				return false;	
			}
			immInt = binaryToDecimal(imm);
			j++;
		}
		
		if(rt_p != -1)
		{
			rt = registers.getNum(operand[j]);
			if(rt == NumRegisters)
			{
				return false;
			}
		}
	}
	else
	{
		if(rs_p != -1)
		{

			rs = registers.getNum(operand[j]);
			if(rs == NumRegisters)
			{
				return false;
			}
			j++;
		
		}
		
		if(rt_p != -1)
		{
			rt = registers.getNum(operand[j]);
			if(rt == NumRegisters)
			{
				return false;
			}
			j++;
		}
		if(imm_p != -1)
		{
				imm = operand[j];
				int immLen = imm.length();
				if(immLen != immBitLen && immLen != jImmBitLen && immLen != shiftLen)
				{
					return false;	
				}
				immInt = binaryToDecimal(imm);

		}
	}
	i.setValues(o, rs, rt, rd, immInt);	
	return true;
}


//Using opcode, string representing operands, and the operand count, this method
//creates an Instruction from those values after checking for correct synatax.
//Specifically this method, handles the case of J-type instructions.
bool BinaryParser::getOperandsJ(Instruction &i, Opcode o, string *operand, int operand_count)
{
	int rs, rt, rd;
	rs=rt=rd=NumRegisters;
	string imm = "";
	int imm_p = opcodes.IMMposition(o); 

	//only contains imm so no registers need to be handled
	if(imm_p != -1)
	{
		imm = operand[imm_p];
	}
	else
	{
		cerr << "Jump instruction must contain binary immeadiate field" << endl;
		return false;
	}
	int immInt = binaryToDecimal(imm);

	i.setValues(o, rs, rt, rd, immInt);
	return true;
}


//Taking an binary value as a parameter, it converts this value to its decimal
//representation.
int BinaryParser::binaryToDecimal(string s)
{
	//handles empty string
	if(s == "")
	{
		return 0;
	}

	int len = s.length();
	int sum = 0;
	int i = 0;
	bool neg = false;    // will be used to determine if a value is negative

	//if first character is 1 then the value is negative so will be converted to postive twos complement number
	if(s.at(i) == '1')
	{
		neg = true;
		while(i < len)
		{
			if(s.at(i) == '1')
			{
				s.at(i) = '0';
			}
			else
			{
				s.at(i) = '1';
			}
			i++;
		}

		i = 0;
		while(s.at(len-(i+1)) != '0')
		{
			s.at(len-(i+1)) = '0';
			i++;
		}
		s.at(len-(i+1)) = '1';
	}

	for(i = 0; i < len; i++)
	{	
		int exponent = len - (i+1);
		int numAt = s.at(i) - '0';
		numAt = numAt*pow(2,exponent);
		sum += numAt;
	}
	//makes value negative again
	if(neg == true)
	{
		sum *= -1;
	}
	return sum;
}


//Taking a binary value as a parameter, it converts this value into its hexadecimal
//representation and returns that represenation as a string.
string BinaryParser::decToHex(int n)
{
	stringstream ss;
	n = n*4;         //equivalent of left shift by two
	ss << hex << n;
	string hexStr = ss.str();
	return hexStr;

}

//Taking an intger as a parameter, it determines the number of digits contained
//in that integer and returns the number of digits.
int BinaryParser::numDigits(int n)
{
	int digits = 0;
	while(n > 0)
	{
		n /= 10;
		digits++;
	}
	return digits;	
}


//Taking an Instruction as a parameter, it decomposes this value and creates a
// string representation of the mips instruction paramter. It then returns this 
// string representation.
string BinaryParser::decode(Instruction i)
{
	Opcode op = i.getOpcode();
	string asmInstruction = opcodes.getName(op) + " "; // will store the instructions ASM representation
	int num_operands = opcodes.numOperands(op);
	int rs_p = opcodes.RSposition(op);
	int rt_p = opcodes.RTposition(op);
	int rd_p = opcodes.RDposition(op);
	int imm_p = opcodes.IMMposition(op);
	
	//puts the values in an array in order so that they can be added in the correct
	//order to the asmInstruction string
	string posArray[4];
	if(rs_p >= 0 && rs_p < 4)
	{
		posArray[rs_p] = "rs";
	}

	if(rt_p >= 0 && rt_p < 4)
	{
		posArray[rt_p] = "rt";
	}

	if(rd_p >= 0 && rd_p < 4)
	{
		posArray[rd_p] = "rd";
	}

	if(imm_p >= 0 && imm_p < 4)
	{
		posArray[imm_p] = "imm";
	}

	for(int j = 0; j < num_operands; j++)
	{
		if(rs_p != -1 && posArray[j] == "rs")
		{
			//handle Load word special case
			if(num_operands == 3 && imm_p == 1)
			{
				asmInstruction = asmInstruction + cvt2MipsReg(i.getRS()) + ")";
			}
			//handles case where register is the last one
			else if(rs_p == num_operands-1)
			{
				asmInstruction = asmInstruction + cvt2MipsReg(i.getRS());
			}	
			else
			{
				asmInstruction = asmInstruction + cvt2MipsReg(i.getRS()) + ", ";
			}
		}
		if(rt_p != -1 && posArray[j] == "rt")
		{
			//handles case where register is the last one
			if(rt_p == num_operands-1)
			{
				asmInstruction = asmInstruction + cvt2MipsReg(i.getRT());
			}
			else
			{
				asmInstruction = asmInstruction + cvt2MipsReg(i.getRT()) + ", ";
			}
		}
		if(rd_p != -1 && posArray[j] == "rd")
		{
			//handles case where register is the last one
			if(rd_p == num_operands-1)
			{
				asmInstruction = asmInstruction + cvt2MipsReg(i.getRD());
			}
			else
			{
				asmInstruction = asmInstruction + cvt2MipsReg(i.getRD()) + ", ";
			}
		}
		if(imm_p != -1 && posArray[j] == "imm")
		{
			//handle Load word special case
			if(num_operands == 3 && imm_p == 1)
			{
				asmInstruction = asmInstruction + intToString(i.getImmediate()) +  "(";
			}
			//handles jump special case
			else if(num_operands == 1 && imm_p == 0)
			{
				int immInt = i.getImmediate();
				string imm = decToHex(immInt);
				asmInstruction = asmInstruction + "0x" + imm;
			}
			//handles branch special case
			else if(num_operands == 3 && opcodes.isIMMLabel(op))
			{
				string immead = "";
				int gotImm = i.getImmediate();
				immead = decToHex(gotImm);
				asmInstruction = asmInstruction + "0x" + immead;
			}
			else
			{	
				asmInstruction = asmInstruction + intToString(i.getImmediate());
			}
		}
	}
	return asmInstruction;

}


// Taking a Register value as a parameter, it converts this value to a string, 
// and prepends a dollar sign to it to follow MIPS convention.
string BinaryParser::cvt2MipsReg(Register reg)
{
	string oneReg = "$";  //will store a single registers ASM representation
	int numDig = numDigits((int)reg);
	int tempInt = (int)reg;
	string regStr = "";

	char digitChar;
	for(int i = 0; i < numDig; i++)
	{
		digitChar = (char)(tempInt%10 + 48);
		regStr = digitChar + regStr;
		tempInt /= 10;
	}
	oneReg = oneReg + regStr;

	return oneReg;
}


//Taking an integer value as a parameter, it converts this value into a string
// and then returns the new string representation.
string BinaryParser::intToString(int n)
{
	string finalStr = "";
	bool neg = false; 	// will determine if the value is a negative or positive integer
	if(n < 0)
	{
		n *= -1;
		neg = true;
	}
	int len = numDigits(n);
	for(int i = 0; i < len; i++)
	{
		char tempC = (char)(n%10 + 48);
		finalStr = tempC + finalStr;
		n /= 10;
	}
	//handles case where number is negative
	if(neg == true)
	{
		finalStr = "-" + finalStr;
	}	
	return finalStr;
}

//Converts a string to its integer representation returning a that integer value
int BinaryParser::stringToInt(string s)
{
	int final = 0;
	int len = s.length();
	int i = 0;
	int exponent = len - (i+1);
	//multiplies each character by its respective power of 10
	for(i = 0; i < len; i++)
	{
		int temp = s.at(i) - '0';
		temp = temp * pow(10, exponent);
		final += temp;
	}
	return final;
}

//Taking a line of machine language code as a parameter, it checks that each bit
//is either a 0 or 1 and makes sure that their are 32 consectutive bits.
bool BinaryParser::checkFor32Consec(string s)
{
	int len = s.length();
	int p = 0;
	int pBefore = 0;
	while(isWhitespace(s.at(p)) && p < len)
	{
		p++;
	}
	pBefore = p;     //will serve as the starting point if whitespace characters came before the machine language
	while(p < len && !isWhitespace(s.at(p)))
	{	
		if(!isBinaryChar(s.at(p)))
		{
			return false;
		}
		p++;
	}
	if(p - pBefore != instLen)
	{
		return false;
	}
	return true;
}


