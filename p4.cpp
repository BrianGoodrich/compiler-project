#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "BinaryTree.h"
#include "scanner.h"
#include "parser.h"
#include <vector>

using namespace std;

//Global vector for global scoped variables.
vector<string> globalScope;
vector<string> stack;
vector<int> varCounts;

void staticSemanticTraverse(TreeNode* node, int depth);
int push(string tk);
void pop();
int find(string tk);



int main(int argc, char** argv){

	string input;
	string fileName;

	if(argc == 2){
		fileName = argv[1];
		fileName.append(".sp23");
	}else{
		cout << "Please enter input\n";
		getline(cin, input);
		fileName = "input.sp23";
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


	setUpScanner();

	
	TreeNode* root = new TreeNode;


	root = parser();

	staticSemanticTraverse(root, 0);	
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
					
					if(tempX.compare(tempY) == 0 && x != y){

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

	return 1;
}

void pop(){

	if(!stack.empty())
		stack.pop_back();	
}

int find(string tk){

	int distance = 0;
	//Since we are pushing into the back of the vector to iterate from top down we start at the back.
	for(auto it = stack.rbegin(); it != stack.rend(); it++){
		if(tk.compare(*it) == 0){
			return distance;
		}

		distance++;
	}

	return -1;
}
