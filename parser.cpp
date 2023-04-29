#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "BinaryTree.h"

using namespace std;

//Global token to be used by all functions
Token tk;

TreeNode* parser();
TreeNode* program();
TreeNode* block();
TreeNode* vars();
TreeNode* stats();
TreeNode* stat();
TreeNode* mStat();
TreeNode* varList();
TreeNode* in();
TreeNode* out();
TreeNode* iff();
TreeNode* loop();
TreeNode* assign();
TreeNode* exp();
TreeNode* RO();
TreeNode* M();
TreeNode* N();
TreeNode* R();
void printTree(TreeNode* node, int depth);
/*
int main(int argc, char* argv[]){

	string input;
	string fileName;

	//If there is command line arg append .sp23 and use it as filename 
	if(argc == 2){
		fileName = argv[1];
		fileName.append(".sp23");
	}else{
		cout << "Please enter input\n";
		getline(cin, input);
		fileName = "input.sp23";
	}

	//If no command line arg take input from keyboard.
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

	printTree(root, 0);	
}
*/

/*
void printTree(TreeNode* node, int depth){

	if(node == NULL){
		return;
	}	

	for(int x = 0; x < depth; x++){
		cout << " ";
	}

	cout << node->label << " ";
	
	for(string tk : node->tkVector){
		cout << tk << " ";
	}

	cout << endl;

	
	if(node->node1 != NULL){
		printTree(node->node1, depth+2);
	}

	if(node->node2 != NULL){
		printTree(node->node2, depth+2);
	}
	
	if(node->node3 != NULL){
		printTree(node->node3, depth+2);
	}

	if(node->node4 != NULL){
		printTree(node->node4, depth+2);
	}
}
*/

TreeNode* parser(){
	
	TreeNode* root = new TreeNode();//Allocate new node
	tk = scanner();
	root = program();

	if(tk.tokenInstance.compare("") == 0){
		tk = scanner();
		if(tk.tokenID == END){
			return root;
		}else{
			cout << "Parser Error: token expected \"EOF\" token received " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
			exit(1);
		}
	}else{
			cout << "Parser Error: token expected \"EOF\" token received " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
	}

	return root;
}

TreeNode* program(){

	TreeNode* program = new TreeNode(); //Allocate node for function
	program->label = "program";	

	program->node1 = block();
	
	return program;
}

TreeNode* block(){
	//Set up node
	TreeNode* block = new TreeNode;
	block->label = "block";
	
	if(tk.tokenInstance.compare("open") == 0){
		
		tk = scanner(); //Consume tk don't need open
		
		block->node1 = vars();

		//Wont need new tk before stats vars will refresh
		block->node2 = stats();
 
		if(tk.tokenInstance.compare("close") == 0){
			tk = scanner(); //Consume no need to store
			return block;
		}else{
			cout << "Block Error: token expected: close token received: " << tk.tokenInstance << " line number: " << tk.lineNum << endl;
			exit(1);
		}
		

	}else{
		cout << "Block Error: token expected: \"open\" token received: " << tk.tokenInstance << " line number: " << tk.lineNum << endl;
		exit(1);
	}
	return block;
}

TreeNode* vars(){

	//Set up node
	TreeNode* vars = new TreeNode;
        vars->label = "vars";

	if(tk.tokenInstance.compare("data") == 0){
	
		tk = scanner(); //consume data don't need
		
		vars->node1 = varList(); //Set child

		return vars; //Return child

	}else{
		return NULL;
	}

	return vars;
}

TreeNode* varList(){
	//Set up node
	TreeNode* vList = new TreeNode;
	vList->label = "varList";
	int flag = 1;

while(flag){
	if(tk.tokenID == IDENT){
		//Store terminal in token vector of node
		vList->tkVector.push_back(tk);
		tk = scanner();
		if(tk.tokenID == COLON){
			tk = scanner();//Consume don't need
			if(tk.tokenID == INT_LIT){
				vList->tkVector.push_back(tk);
				tk = scanner();
			
				if(tk.tokenID == SEMI_COL){
					tk = scanner();//Consume
					return vList;

				}else{
					//tk = scanner();
					//vList->node1 = varList();
					//return vList;
					//tk = scanner();
					continue;
				}

			}else{
				cout << "varList Error: token expeceted: Integer token received: " << tk.tokenInstance << " line number: " << tk.lineNum << endl;                exit(1);
			}	

		}else{
			cout << "varList Error: token expeceted: \":\" token received: " << tk.tokenInstance << " line number: " << tk.lineNum << endl;                exit(1);

		}
	}else{
		cout << "varList Error: token expeceted: Identifier token received: " << tk.tokenInstance << " line number: " << tk.lineNum << endl;
		exit(1);		
	}
}
	return vList;
}

TreeNode* stats(){
	//New tk not needed here one is generated right before call.
	//Set up node
	TreeNode* stats = new TreeNode;
	stats->label = "stats";
	
	stats->node1 = stat();
	stats->node2 = mStat();

	return stats;
}

TreeNode* mStat(){
	
	//Set up node
	TreeNode* moreStat = new TreeNode;
	moreStat->label = "mStat";


	//If the next token seems to be a statement then call stats, if not then it is empty so return.	
	if(tk.tokenInstance.compare("read") == 0
|| tk.tokenInstance.compare("print") == 0 
|| tk.tokenInstance.compare("iff") == 0
|| tk.tokenInstance.compare("open") == 0 
|| tk.tokenInstance.compare("iter") == 0
|| tk.tokenID == IDENT)
	{
		moreStat->node1 = stat();		
		moreStat->node2 = mStat();
	}else
		return moreStat;

	return moreStat;
}


TreeNode* stat(){
	
	//Set up node
	TreeNode* stat = new TreeNode;
	stat->label = "stat";

	if(tk.tokenInstance.compare("read") == 0){
		stat->node1 = in();
		return stat;
	}else if(tk.tokenInstance.compare("print") == 0){
		stat->node1 = out();
		return stat;
	}else if(tk.tokenInstance.compare("iff") == 0){
		stat->node1 = iff();
		return stat;
	}else if(tk.tokenInstance.compare("open") == 0){
		stat->node1 = block();
		return stat;
	}else if(tk.tokenInstance.compare("iter") == 0){
		stat->node1 = loop();
		return stat;
	}else if(tk.tokenID == IDENT){
		stat->node1 = assign();
		return stat;
	}else{
		cout << "stat Error: statement expected received token: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
		exit(1);
	}	
	return stat;

}

TreeNode* in(){
	
	//Set up node
	TreeNode* in = new TreeNode;
	in->label = "in";

	if(tk.tokenInstance.compare("read") == 0){
		tk = scanner(); //consume

		if(tk.tokenInstance.compare(">") == 0){
			tk = scanner();	//consume
			
			if(tk.tokenID == IDENT){
				//store then consume
				in->tkVector.push_back(tk);
				tk = scanner();
				
				if(tk.tokenID == SEMI_COL){
					tk = scanner();	
					return in;
				}else{
					cout << "in Error: token expected \";\" received token: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
					exit(1);
				}
				
			}else{
				cout << "in Error: token expected: Identifier received token: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
				exit(1);
			}

		}else{
			cout << "in Error: token expected: \">\" received token: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
			exit(1);
		}

	}else{
		cout << "in Error: token expected: \"read\" received token: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
		exit(1);
	}

	return in;

}

TreeNode* out(){

	//Set up node
	TreeNode* out = new TreeNode;
	out->label = "out";

	if(tk.tokenInstance.compare("print") == 0){
		tk = scanner();
			
		if(tk.tokenID == LESS_THAN){
			tk = scanner();
			out->node1 = exp();

			if(tk.tokenID == SEMI_COL){
				tk = scanner();
				return out;
			}else{
				cout << "out Error: token expected \";\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;						
				exit(1);
			}
		}else{
			cout << "out Error: token expected \"<\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
			exit(1);
		}
	}else{
		cout << "out Error: token expected \"print\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
		exit(1);
	}

	return out;
}

TreeNode* iff(){
	
	//Set up node
	TreeNode* iff = new TreeNode;
	iff->label = "iff";

	if(tk.tokenInstance.compare("iff") == 0){
		tk = scanner();			

		if(tk.tokenID == LEFT_BRACKET){
			tk = scanner();
						
			iff->node1 = exp();
			iff->node2 = RO();
			iff->node3 = exp();

			if(tk.tokenID == RIGHT_BRACKET){
				tk = scanner();
			
				iff->node4 = stat();
				return iff;
			}else{
				cout << "iff Error: token expected \"]\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                exit(1);
			}

		}else{
			cout << "Error: token expected \"[\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                exit(1);

		}

	}else{
		cout << "Error: keyword expected \"iff\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
		exit(1);
	
	}
	return iff;
}

TreeNode* loop(){

	//Set up node
	TreeNode* loop = new TreeNode;
	loop->label = "loop";

	if(tk.tokenInstance.compare("iter") == 0){
		tk = scanner();
		
		if(tk.tokenID == LEFT_BRACKET){
			tk = scanner();
			
			loop->node1 = exp();
			loop->node2 = RO();
			loop->node3 = exp();

			if(tk.tokenID == RIGHT_BRACKET){
				tk = scanner();

				loop->node4 = stat();
				return loop;
			}else{
				cout << "loop Error: token expected \"]\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                		exit(1);
			}

		}else{
			cout << "loop Error: token expected \"[\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                	exit(1);
		}
		
	}else{
		cout << "loop Error: keyword expected \"iter\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                exit(1);
	}
	
	return loop;

}

TreeNode* assign(){

	//Set up node
	TreeNode* assign = new TreeNode;
	assign->label = "assign";

	if(tk.tokenID == IDENT){
		assign->tkVector.push_back(tk);
		tk = scanner();

		if(tk.tokenID == ASSIGN_OP){
			tk = scanner();
			assign->node1 = exp();
			if(tk.tokenID == SEMI_COL){
				tk = scanner();
				return assign;
			}else{
				 cout << "assign Error: token expected \";\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                        exit(1);
			}
		}else{
			cout << "assign Error: token expected \"=\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                        exit(1);
		}
	}else{
		cout << "assign Error: token expected Identifier token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
                        exit(1);
	}

	return assign;	

}
TreeNode* exp(){

	//Set up node
	TreeNode* expr = new TreeNode;
	expr->label = "exp";
	
	expr->node1 = M();
		
	if(tk.tokenID == DIV_OP || tk.tokenID == MULT_OP){
		expr->tkVector.push_back(tk);
		tk = scanner();

		expr->node2 = exp();
		return expr;
	}else{ //Else is just a return because we are already calling M above, in the case it isn't followed by / or * its just M.
		//M();
		return expr;
	}

}

TreeNode* RO(){
	
	TreeNode* ro = new TreeNode;
	ro->label = "RO";

	if(tk.tokenID == LTHN_TILDE){
		ro->tkVector.push_back(tk);
		tk = scanner();
		return ro;
	}else if(tk.tokenID == GRTRTHN_TILDE){
		ro->tkVector.push_back(tk);
		tk = scanner();
		return ro;
	}else if(tk.tokenID == LESS_THAN){
		ro->tkVector.push_back(tk);
		tk = scanner();
		return ro;
	}else if(tk.tokenID == GREATER_THAN){
		ro->tkVector.push_back(tk);
		tk = scanner();
		return ro;
	}else if(tk.tokenID == ASSIGN_OP){
		ro->tkVector.push_back(tk);
		tk = scanner();
		return ro;
	}else if(tk.tokenID == TILDE){
		ro->tkVector.push_back(tk);
		tk = scanner();
		return ro;
	}else{
		cout << "RO Error: relational operator expected token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
	}

	return ro;

}

TreeNode* M(){
	
	//Set up node
	TreeNode* m = new TreeNode;
	m->label = "M";
	
	m->node1 = N();
		
	if(tk.tokenID == ADD_OP){
		m->tkVector.push_back(tk);
		tk = scanner();
		m->node2 = M();
		return m;
	}else{
		//N();
		return m;
	}
	
	return m;

}

TreeNode* N(){

	//Set up node
	TreeNode* n = new TreeNode;
	n->label = "N";

	if(tk.tokenID == TILDE){
		n->tkVector.push_back(tk);
		tk = scanner();
		n->node1 = N();
		return n;
	}
	
	//Only call R if we know it needs to be called
	if(tk.tokenID == LEFT_PAREN || tk.tokenID == IDENT || tk.tokenID == INT_LIT){
		n->node1 = R();
	}
	
	if(tk.tokenID == SUB_OP){
		n->tkVector.push_back(tk);
		tk = scanner();
		n->node2 = N();
		return n;
	}else if(tk.tokenID == LEFT_PAREN || tk.tokenID == IDENT || tk.tokenID == INT_LIT){
		n->node2 = R();
		return n;
	}else
		return n;
}

TreeNode* R(){

	//Set up node
	TreeNode* r = new TreeNode;
	r->label = "R";

	if(tk.tokenID == LEFT_PAREN){
		tk = scanner();
		r->node1 = exp();

		if(tk.tokenID == RIGHT_PAREN){
			tk = scanner();		
			return r;
		}else{
			cout << "R Error: token expected \")\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
			exit(1);
		}

	}else if(tk.tokenID == IDENT){
		r->tkVector.push_back(tk);
		tk = scanner();		
		return r;

	}else if(tk.tokenID == INT_LIT){
		r->tkVector.push_back(tk);
		tk = scanner();
		return r;
	}else{
		cout << "R Error: token expected \"(\" token received: " << tk.tokenInstance << " line num: " << tk.lineNum << endl;
		exit(1);
	}

	return r;

}









