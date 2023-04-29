#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
#include <fstream>


using namespace std;
//Char classes
#define LETTER 0
#define DIGIT 97
#define UNKNOWN 98
#define IDENTIFIER 99
#define END 100
#define GREATER 101
#define LESS 102
//Token codes
#define INT_LIT 1
#define IDENT 2
#define ASSIGN_OP 3
#define ADD_OP 4
#define SUB_OP 5
#define MULT_OP 6
#define DIV_OP 7
#define LEFT_PAREN 8
#define RIGHT_PAREN 9
#define SEMI_COL 10
#define COMMA 11
#define LEFT_BRACKET 12
#define RIGHT_BRACKET 13
#define LEFT_CURLY_BRACKET 14
#define RIGHT_CURLY_BRACKET 15
#define DOT_OP 16
#define MODULUS_OP 17
#define COLON 18
#define GREATER_THAN 19
#define LESS_THAN 20
#define TILDE 21
#define GRTRTHN_TILDE 22
#define LTHN_TILDE 23
#define KEYWORD 24

extern fstream inputFile;

struct Token{
	int tokenID;
	string tokenInstance;
	int lineNum;
};

Token scanner();

void setUpScanner();

#endif
