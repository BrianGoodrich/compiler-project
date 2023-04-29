#include <iostream>
#include "BinaryTree.h"
#include <vector>
#include <fstream>

using namespace std;


void BinaryTree :: add_node(TreeNode* node){	

	TreeNode* newNode = new TreeNode;

	insert(node, newNode);
}

void BinaryTree :: insert(TreeNode*& nodePtr, TreeNode*& newNode){
	if (nodePtr == NULL){
		nodePtr = newNode;
	}else if(nodePtr->node1 == NULL){
		insert(nodePtr->node1, newNode);	
	}else if(nodePtr->node2 == NULL){
		insert(nodePtr->node2, newNode);
	}else if(nodePtr->node3 == NULL){
                insert(nodePtr->node3, newNode);
        }else if(nodePtr->node4 == NULL){
                insert(nodePtr->node4, newNode);
        }

}

void BinaryTree :: print_inorder(){
	displayInOrder(root);
}

void BinaryTree :: displayInOrder(TreeNode* nodePtr){

	string space;

	if (nodePtr != NULL){

		displayInOrder(nodePtr->node1);
		
		//cout << nodePtr->	
	
		for(string val : nodePtr->tkVector){
			cout << "tkVector\n";
		}

	}else
		return;
}


void BinaryTree::destroyTree(TreeNode* nodePtr)
{
	if (nodePtr)
	{
		if (nodePtr->node1)
			destroyTree(nodePtr->node1);
		else if (nodePtr->node2)
			destroyTree(nodePtr->node2);
		else if(nodePtr->node3)
			destroyTree(nodePtr->node3);
		else if(nodePtr->node4)
			destroyTree(nodePtr->node4);
		delete nodePtr;

	}
}
