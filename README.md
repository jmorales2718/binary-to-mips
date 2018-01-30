# Binary To Mips Assembly Project - C++

## Overview

This program reads in a Machine Language file specified as a command line argument. It checks that the file is correctly formatted and only contains supported Machine language instructions. If true, the program prints each instruction’s MIPS assembly equivalent to stdout.

This program contains four classes: BinaryParser, Instruction, OpcodeTable, and RegisterTable. It also contains a file ASM.cpp which uses all the classes to print the instructions in MIPS Assembly.

## Description of Classes:

**OpcodeTable:** This class contains information about the format of specific instructions. Specifically it contains information about the Mips instruction fields: opcode, register, immediate, and function. It also contains this information for all supported Machine language instructions.

**RegisterTable:** This class stores information about the valid register names in Machine language. The name is just the binary representation of the number. If the binary representation was converted to decimal that would be the register 
(ex. 01000 would be $8). It also stores an integer containing the number of each register.

**Instruction:** This class holds information about a single Machine language instruction. It stores values for the different fields of an instruction such as: opcode, registers, and immediate fields as well as the MIPS assembly encoding of the instruction.

**BinaryParser:** This class reads in a Machine language file, checks for syntax errors. If none are found. This class retains a list of Instructions, taken from the file.

