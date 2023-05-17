#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "BinaryTree.h"
#include "scanner.h"
#include "parser.h"
#include <vector>

using namespace std;

vector<string> stack;
vector<string> varList; //Permanent "stack" for storage allocation of regular vars
vector<int> varCounts;
vector<string> tempVars; //Treated as stack for temp vars 

fstream outFile;
int tempVarCount;
int tempLabelCount;

string newTemp(string type);
void staticSemanticTraverse(TreeNode* node, int depth);
int push(string tk);
void pop();
int find(string tk);
void codeGenerator(TreeNode* node);
void storageAllocation();

int main(int argc, char** argv){

	string input;
	string fileName;
	string outputFile;

	if(argc == 2){
		fileName = argv[1];
		outputFile = argv[1];
		fileName.append(".sp23");
		outputFile.append(".asm");
	}else{
		cout << "Please enter input\n";
		getline(cin, input);
		fileName = "input.sp23";
		outputFile = "a.asm";
	}

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
	
	outFile.open(outputFile, ios::out);

	if(!outFile){
		cout << "Error opening file\n";
		exit(1);

	}

	setUpScanner();

	
	TreeNode* root = new TreeNode;

	//Get root of the tree built by parser
	root = parser();
	
	//Do static semantics traversal.
	staticSemanticTraverse(root, 0);	

	//Generate code to output file.
	codeGenerator(root);

	outFile << "STOP\n";

	storageAllocation();

	inputFile.close();
	outFile.close();
}

void codeGenerator(TreeNode* node){
	
	//Return on null
	if(node == NULL)
		return;

	//Handle block node
	if(node->label.compare("block") == 0){
		//Call vars
		codeGenerator(node->node1);
		//Call stats
		codeGenerator(node->node2);
		return;
	}
	
	//Handle program node
	if(node->label.compare("program") == 0){
		//Call block	
		codeGenerator(node->node1);
		return;
	}	

	//Handle in node
	if(node->label.compare("in") == 0){
		outFile << "READ " << node->tkVector[0].tokenInstance << "\n";
		return;
	}
	
	//Handle vars node
	if(node->label.compare("vars") == 0){
		//Call varlist
		codeGenerator(node->node1);			
		return;
	}

	//Handle varList
	if(node->label.compare("varList") == 0){


			for(auto tk = node->tkVector.rbegin(); tk != node->tkVector.rend(); tk++){       			
				if(tk->tokenID == INT_LIT){
                                        outFile << "LOAD " << tk->tokenInstance << "\n";
                                }
	
				if(tk->tokenID == IDENT){
					outFile << "STORE " << tk->tokenInstance << "\n";
				}
			}
		
			return;
	}

	//Handle exp
	if(node->label.compare("exp") == 0){
		//If just the M node then call it and return
		if(node->node2 == NULL){
			codeGenerator(node->node1);
			return;
		}
		
		//If division
		if(node->tkVector[0].tokenInstance.compare("/") == 0){	
			codeGenerator(node->node2); //Get second val in acc first
			
			string temp = newTemp("var");
			
			outFile << "STORE " << temp << "\n"; //Store our second val.
			
			codeGenerator(node->node1); //Get first val in acc

			outFile << "DIV " << temp << "\n";
			
			return;
		}
		
		//If mult
		if(node->tkVector[0].tokenInstance.compare("*") == 0){
			codeGenerator(node->node2);

			string temp = newTemp("var");
			
			outFile << "STORE " << temp << "\n";

			codeGenerator(node->node1);
		
			outFile << "MULT " << temp << "\n";
			
			return;
		}
		
	}

	//Handle M
	if(node->label.compare("M") == 0){
		if(node->node2 == NULL){
			codeGenerator(node->node1);
			return;
		}else{
			//Call left first for N node
			codeGenerator(node->node1);
			//Now store the left side value in temp
			string temp = newTemp("var");
			
			//Write store left node val to file, value will already be in ACC
			outFile << "STORE " << temp << "\n";
			
			//Call second child, will put right value in ACC
			codeGenerator(node->node2);
			
			//Right value in ACC now, add temp back to it.
			outFile << "ADD " << temp << "\n";
			
			return;
		}
	}

	//Handle N
	if(node->label.compare("N") == 0){
		if(node->node2 == NULL){
			
			codeGenerator(node->node1); //Get the value in the acc

			//If there is negation multiply by -1
			if(node->tkVector.size() > 0){
				if(node->tkVector[0].tokenID == TILDE){
					outFile << "MULT -1\n";//Negation 

				}
			}
			
			return;
			
		}else{
			codeGenerator(node->node2); //Load right value into acc

			string temp = newTemp("var");
		
			outFile << "STORE " << temp << "\n"; //Store right val in temp

			codeGenerator(node->node1); //load left value in acc

			outFile << "SUB " << temp << "\n"; //sub right from left

			return;
		}
	}	

	//Handle R
	if(node->label.compare("R") == 0){
		
		if(node->node1 != NULL){
			codeGenerator(node->node1);
			return;
		}else{
			outFile << "LOAD " << node->tkVector[0].tokenInstance << "\n";
			return;
		}
	}

	//Handle stats
	if(node->label.compare("stats") == 0){
		codeGenerator(node->node1);
                codeGenerator(node->node2);
		return;
	}

	//Handle mStat
	if(node->label.compare("mStat") == 0){
		codeGenerator(node->node1);
		codeGenerator(node->node2);
		return;
	}

	//Handle stat
	if(node->label.compare("stat") == 0){
		codeGenerator(node->node1);
		return;
	}

	//Handle out
	if(node->label.compare("out") == 0){
		
		string temp = newTemp("var");	

	
		codeGenerator(node->node1); //get value of exp in acc

		outFile << "STORE " << temp << "\n"; //store the val in acc in a temp
		outFile << "WRITE " << temp << "\n";
		return;
	}
		
	//Handle assign
	if(node->label.compare("assign") == 0){
		codeGenerator(node->node1);
		outFile << "STORE " << node->tkVector[0].tokenInstance << "\n"; 
		return;
	}
	
	//handle if
	if(node->label.compare("iff") == 0){
		codeGenerator(node->node3); //Right expr

		//Right expr will be in acc need to store it
			
		string tempVar = newTemp("var");

		outFile << "STORE " << tempVar << "\n";

		codeGenerator(node->node1);//left expr

		outFile << "SUB " << tempVar << "\n";//expr left - expr right
	
		string tempLabel = newTemp("label");
		
		//Handle == case	
		if(node->node2->tkVector[0].tokenInstance.compare("=") == 0){ 
			outFile << "BRNEG " << tempLabel << "\n";
			outFile << "BRPOS " << tempLabel << "\n";
		}
		
		//Handle > case
		if(node->node2->tkVector[0].tokenInstance.compare(">") == 0){
			outFile << "BRZNEG " << tempLabel << "\n";

		}

		//Handle < case
		if(node->node2->tkVector[0].tokenInstance.compare("<") == 0){
			outFile << "BRZPOS " << tempLabel << "\n"; 

		}
		
		//Handle >~ case
		if(node->node2->tkVector[0].tokenInstance.compare(">~") == 0){
			outFile << "BRNEG " << tempLabel << "\n";

		}

		//Handle <~ case
		if(node->node2->tkVector[0].tokenInstance.compare("<~") == 0){
			outFile << "BRPOS " << tempLabel << "\n";

		}
		
		if(node->node2->tkVector[0].tokenInstance.compare("~") == 0){
			outFile << "BRZERO " << tempLabel << "\n";

		}

		codeGenerator(node->node4); //Dependent statements

		outFile << tempLabel << ": NOOP\n";
		
		return;
	}


	//Handle iter
	if(node->label.compare("loop") == 0){

		string tempLabel = newTemp("label");//Label for not true branch
		string tempLabel2 = newTemp("label");//Label for looping

		outFile << tempLabel2 << ": NOOP\n"; //Set label at the top of the looping struct so that we can jump back up to it after statements are executed.
		
		codeGenerator(node->node3); //Right exp

		string tempVar = newTemp("var");
	
		outFile << "STORE " << tempVar << "\n";

		codeGenerator(node->node1); //Left exp

		outFile << "SUB " << tempVar << "\n"; //Evaluate expr

		//Handle == case
		if(node->node2->tkVector[0].tokenInstance.compare("=") == 0){
                        outFile << "BRNEG " << tempLabel << "\n";
			outFile << "BRPOS " << tempLabel << "\n";
                }

		//Handle > case
		if(node->node2->tkVector[0].tokenInstance.compare(">") == 0){
                        outFile << "BRZNEG " << tempLabel << "\n";
                }

		//Handle < case
		if(node->node2->tkVector[0].tokenInstance.compare("<") == 0){
                        outFile << "BRZPOS " << tempLabel << "\n";
                }

		//Handle >~
		if(node->node2->tkVector[0].tokenInstance.compare(">~") == 0){
                        outFile << "BRNEG " << tempLabel << "\n";
                }

		//Handle <~
		if(node->node2->tkVector[0].tokenInstance.compare("<~") == 0){
                        outFile << "BRPOS " << tempLabel << "\n";
                }

		//Handle ~
		if(node->node2->tkVector[0].tokenInstance.compare("~") == 0){
			outFile << "BRZERO " << tempLabel << "\n";

		}

		codeGenerator(node->node4); //Dependent statements

		//Branch back to the top. If statement wasn't true we would branch past this.
		outFile << "BR " << tempLabel2 << "\n";

		outFile << tempLabel << ": NOOP\n"; //Branching for if the statement is not true. 
		
	}
}


void storageAllocation(){
	
	//Write all regular vars to the end. Required by assembly langauge.
	for(string var : varList){
		outFile << var << " 0\n";
	}
	
	//Write temp vars to allocation.
	for(string var : tempVars){
		outFile << var << " 0\n";
	}
}

//Function to generate temp var names and labels to be printed to output.
string newTemp(string type){
	
	string newName;

	//Generate a new string var to print into output
	if(type.compare("var") == 0){
		newName = "T";
		newName += to_string(tempVarCount);
		tempVarCount++;			

	}else{
		newName = "L";
		newName += to_string(tempLabelCount);
		tempLabelCount++;
	}
	
	if(type.compare("var") == 0){
		tempVars.push_back(newName);	
	}

	return newName;
}


void staticSemanticTraverse(TreeNode* node, int depth){
	
	if(node == NULL){
                return;
        }
	int block = 1;
	int varCount = 0;

//--------------Local Scoping----------------
	

	//If we are outside of the global scoped vars
	if(block != 0){
		if(node->label.compare("varList") == 0){


			//Nested loops to ensure there are no duplicate declarations in the same scope.
			for(unsigned int x = 0; x < node->tkVector.size(); x++){
				string tempX = node->tkVector[x].tokenInstance;
				for(unsigned int y = 0; y < node->tkVector.size(); y++){
					string tempY = node->tkVector[y].tokenInstance;
					
					if(tempX.compare(tempY) == 0 && x != y && node->tkVector[x].tokenID == IDENT){

						cout << "Error: variable " << node->tkVector[x].tokenInstance << " has multiple declarations on line " << node->tkVector[x].lineNum << endl;
						exit(1);
					}
				}
			}
			
			//Push each var onto stack.
			for(Token token : node->tkVector){
         	               if(token.tokenID == IDENT){
					varCount++;
					push(token.tokenInstance);
                        	}			
			}
			
			//Place the number of vars in a vector/list so we can pop them off later.
			varCounts.push_back(varCount);
			varCount = 0;
		}
	}
	

	//Nodes where a var use could be
	if(node->label.compare("in") == 0
        || node->label.compare("assign") == 0
        || node->label.compare("R") == 0)
        {
                int result;
		
		//Loop through the tokens in the node looking for vars
		for(Token token : node->tkVector){
                        if(token.tokenID == IDENT){
				result = find(token.tokenInstance);
				if(result == -1){
					cout << "Error: variable " << token.tokenInstance << " on line " << token.lineNum <<  " not defined before use.\n";
				}        	
			}		
		}
        }
	


	//If we see an empty mStat we know we are about to leave the scope of the previously added vars.	
	if(node->label.compare("mStat") == 0){
		if(node->node1 == NULL && node->node2 == NULL && node->node3 == NULL && node->node4 == NULL){
			int tempVarCount;
			if(!varCounts.empty()){
				tempVarCount = varCounts.back();
			}
			

			for(int x = 0; x < tempVarCount -1; x++){
				pop();
			}
	
			tempVarCount = 0;
		}
		
	}
	
	
	//Traversal
	if(node->node1 != NULL){
		staticSemanticTraverse(node->node1, depth+2);
	}

	if(node->node2 != NULL){
		staticSemanticTraverse(node->node2, depth+2);
	}
	
	if(node->node3 != NULL){
		staticSemanticTraverse(node->node3, depth+2);
	}

	if(node->node4 != NULL){
		staticSemanticTraverse(node->node4, depth+2);
	}
}


int push(string tk){

	stack.push_back(tk);
	varList.push_back(tk);
	return 1;
}

void pop(){

	if(!stack.empty())
		stack.pop_back();	
}

int find(string tk){

	int distance = 0;
	//Since we are pushing into the back of the vector to iterate from top down we start at the back.
	for(auto iter = stack.rbegin(); iter != stack.rend(); iter++){
		if(tk.compare(*iter) == 0){
			return distance;
		}

		distance++;
	}

	return -1;
}
