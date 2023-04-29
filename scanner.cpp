#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scanner.h"
using namespace std;
/*
//Character class declarations. These are used to determine type of the character as well as special cases like <~ and >~ and EOF.
#define LETTER 0
#define DIGIT 97
#define UNKNOWN 98
#define IDENTIFIER 99
#define END 100
#define GREATER 101
#define LESS 102
//Token Codes
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
*/
//Global vars
int charClass;
string lexeme;
char nextChar;
int lexLen;
int token;
int nextToken; 
fstream inputFile;
int lineCount = 1;

//Vector of keywords to check potential matches against
vector <string> keywords{"close", "open", "iter", "void", "data", "exit", "read", "print", "main", "iff", "then", "let", "func"};

//Prototypes
void addChar();
void getNextChar();
void getNonSpace();
Token scanner();
int lookup(char currentChar);

/*
int main(int argc, char *argv[]){
	
	string input;
	string fileName;

	//If there was a command line arg try to use it as file with implicit extension .sp23
	if(argc == 2){
		fileName = argv[1];
		fileName.append(".sp23");
	}else{
		cout << "Please enter input\n";
		getline(cin, input);
		fileName = "input.sp23";
	}
	
	//If there was no command line argument then take the input from the keyboard send it to the file and use the file the same as we otherwise would.
	if(argc != 2){
		inputFile.open(fileName, ios::out);

		if(inputFile){
			input.append(" ");
			inputFile << input;
			inputFile.close();
		}else{
			cout << "Error opening file\n";
			exit(1);
		}
	}
	
	inputFile.open(fileName, ios::in);

	if(!inputFile){
		cout << "Error opening file\n";
		exit(1);
	}
	
	getNextChar();

//Looping for each token and outputting it.		
	do{	
		Token returnedToken = scanner();

		//Need if here to check if last token is blank.
		if(returnedToken.tokenInstance != ""){
			cout << "Token code is: " << returnedToken.tokenID;
			cout << " token is: " << returnedToken.tokenInstance;
			cout << " line number: " << returnedToken.lineNum << endl;
		}

	}while(nextToken != END);

	inputFile.close();
}
*/

//Adds character to the token if necessary.

void setUpScanner(){
	getNextChar();	

	string input;

	/*	
	while(inputFile >> input){
		cout << "printing from setup\n";
		cout << input;
	}*/
}



void addChar(){
	if(lexLen <= 8){
		lexeme += nextChar;
		lexLen++;
	}else{
		cout << "Error: lexeme is too long, program will terminate\n";
		exit(1);
	}
}

//Get next char from file and determine its character class.
void getNextChar(){

	if(!inputFile.eof()){
		//Count lines if we encounter newline.
		if(nextChar == '\n'){
	                ++lineCount;
        	}
	
		inputFile.get(nextChar);	

		if(isalpha(nextChar)){
			charClass = LETTER;
		}else if(isdigit(nextChar)){
			charClass = DIGIT;
		}else if(nextChar == '_'){ //If we get _ we know it is ID.
			charClass = IDENTIFIER;
//Here we will check if we got a @, if so we want to read until the next @ discarding anything in between. Additional case in lookup() to handle ending a line with @.
		}else if(nextChar == '@'){
			
			inputFile.get(nextChar);
			
			while(nextChar != '@' && !inputFile.eof()){
				inputFile.get(nextChar);
			}

			if(inputFile.eof()){
				cout << "LEXICAL ERROR: Comment was not closed\n";
				exit(1);
			}
			inputFile.get(nextChar);
		}else if(nextChar == '<'){ //This and > will be handled in the scanner similar to the way I am handling keywords or integers.
			charClass = LESS;
		}else if (nextChar == '>'){
			charClass = GREATER;
		}else
			charClass = UNKNOWN;
	}else{
		charClass = END;
	}
}

//Get rid of spaces in input stream.
void getNonSpace(){
	//Keep reading out the spaces using the .get() in getNextChar()
	while(isspace(nextChar) && !inputFile.eof()){
		getNextChar();
	}
}

//If the value read isn't an alphabetical or digit then look it up in special characters.
int lookup(char currentChar){
	
	//Switch case for recognizing special characters in the language.
	switch(currentChar){
		
		case  '(':
      			addChar();
      			nextToken = LEFT_PAREN;
      			break;

    		case  ')':
    			addChar();
      			nextToken = RIGHT_PAREN;
      			break;

    		case  '+':
      			addChar();
      			nextToken = ADD_OP;
      			break;

    		case  '-':
      			addChar();
      			nextToken = SUB_OP;
      			break; 

    		case  '*':
      			addChar();
      			nextToken = MULT_OP;
      			break;

    		case  '/':
      			addChar();
      			nextToken = DIV_OP;
      			break;		
		
		case ';':
			addChar();
			nextToken = SEMI_COL;
			break;

		case ',':
                        addChar();
                        nextToken = COMMA;
                        break;

		case '=':
                        addChar();
                        nextToken = ASSIGN_OP;
                        break;		

		case '[':
                        addChar();
                        nextToken = LEFT_BRACKET;
                        break;

		case ']':
                        addChar();
                        nextToken = RIGHT_BRACKET;
                        break;

		case '{':
                        addChar();
                        nextToken = LEFT_CURLY_BRACKET;
                        break;

		case '}':
                        addChar();
                        nextToken = RIGHT_CURLY_BRACKET;
                        break;

		case '.':
                        addChar();
                        nextToken = DOT_OP;
                        break;


		case '%':
                        addChar();
                        nextToken = MODULUS_OP;
                        break;
		
		case ':':
                        addChar();
                        nextToken = COLON;
                        break;
		
		case '~':
                        addChar();
                        nextToken = TILDE;
                        break;
		
		case '@': //Case needed for this when a comment tag is the last thing that is in the file.
			getNonSpace();
			break;

		default:
			cout << "LEXICAL ERROR: unknown token on line number " << lineCount << ", program will terminate\n";
			exit(1);
	
	}	

	return nextToken;
}

//Function representing the actual scanner.
Token scanner(){
	
	Token currentToken;
	lexLen = 0; 
	int match = 0;

	getNonSpace();

	switch(charClass){
	
		case IDENTIFIER:
			
			addChar();
			getNextChar();
			
			while(charClass == LETTER || charClass == DIGIT){
				addChar();
				getNextChar();				
			}

			nextToken = IDENT;
			break;
			
		case LETTER:
			
			addChar();
			getNextChar();
	
			while(charClass == LETTER){
				addChar();
				getNextChar();
			}
						
			//Loop through vector of keywords to see if we get a match, set flags appropriately.	
			for(string kw : keywords){			
				if(kw.compare(lexeme) == 0){ 
					nextToken = KEYWORD;
					match = 0;
					break;
				}else{
					match = 1; //If no match set flag.
				}
			}
			//If this flag is set we had an error reading kw.
			if(match == 1){
				cout << "LEXICAL ERROR: Error in matching keyword, line number: " << lineCount << ", program will terminate\n";
				exit(1);
			}
							
			break;
	
		case DIGIT:

			addChar();
			getNextChar();
			
			while(charClass == DIGIT){
				addChar();
				getNextChar();
			}		
			
			nextToken = INT_LIT;	
			
			break;

		case UNKNOWN:
			
			if(!inputFile.eof()){
				lookup(nextChar);
				getNextChar();
			}else
				charClass = END;
			break;

		case LESS: 
			
			addChar();
			getNextChar();
			
			if(nextChar == '~'){
				addChar();
				getNextChar();
				nextToken = LTHN_TILDE; 
				break;
			}else{
				nextToken = LESS_THAN;
				break;
			}

		case GREATER:
		
			addChar();
                        getNextChar();

                        if(nextChar == '~'){
                                addChar();
                                getNextChar();
                                nextToken = GRTRTHN_TILDE;
                                break;
                        }else{
                                nextToken = GREATER_THAN;
                                break;
                        }
			
		case END:
			nextToken = END;
			lexeme = "EOF";
			break;	
				
	}//End switch	

	currentToken.tokenID = nextToken;
	currentToken.tokenInstance = lexeme;
	currentToken.lineNum = lineCount;
	
	lexeme = ""; //Reset after each token

		
	return currentToken;

}
