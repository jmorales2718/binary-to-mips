#include "RegisterTable.h"
#include <iostream>

RegisterTable::RegisterTable()
// Constructs and initializes the valid register names and numbers
{
  int i = 0;

  myRegisters[i].name = "00000";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "00001";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "00010";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "00011";  myRegisters[i].number = i; i++;

  myRegisters[i].name = "00100";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "00101";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "00110";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "00111";  myRegisters[i].number = i; i++;

  myRegisters[i].name = "01000";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "01001";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "01010";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "01011";  myRegisters[i].number = i; i++;

  myRegisters[i].name = "01100";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "01101";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "01110";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "01111";  myRegisters[i].number = i; i++;

  myRegisters[i].name = "10000";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "10001";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "10010";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "10011";  myRegisters[i].number = i; i++;

  myRegisters[i].name = "10100";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "10101";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "10110";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "10111";  myRegisters[i].number = i; i++;

  myRegisters[i].name = "11000";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "11001";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "11010";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "11011";  myRegisters[i].number = i; i++;

  myRegisters[i].name = "11100";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "11101";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "11110";  myRegisters[i].number = i; i++;
  myRegisters[i].name = "11111";  myRegisters[i].number = i; i++;
}

Register RegisterTable::getNum(string reg)
  // Given a string representing a MIPS register operand, returns the number associated
  // with that register.  If string is not a valid register, returns NumRegisters.
{
  for(int i = 0; i < NumRegisters; i++)
  {
    if(myRegisters[i].name == reg)
    {
       
      return myRegisters[i].number;
    }
  }
  return NumRegisters;

}
