#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include "LexerJS.h"
#include "Variable.h"
#include "Function.h"

using namespace std;

std::string replace(std::string& source, char oldChar, char newChar) {
	std::string edited = source;
	for (size_t i = 0; i < edited.length(); i++)
	{
		if (edited[i] == oldChar)
			edited[i] = newChar;
	}
	return edited;
}

const std::string lx = "WeHaJS translator Ver - 1.0\n\n"; // Lang Version

void Parser(); // Main Parser

// Other Parsers
void ParseVarOut(std::string& code);
void ParsePrint(std::string& code);
void ParseStartFunc(std::string& code);
void ParseMV(std::string& Code);
void ParseInput(std::string& code);
bool ParseFunction(std::string& code);
void ParseAssignVar(std::string& code);
void ParseNewVariable(std::string& code);