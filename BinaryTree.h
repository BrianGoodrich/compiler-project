#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <iostream>
#include <vector>
#include <fstream>
#include "scanner.h"

using namespace std;

struct TreeNode{
	string label;
        vector<struct Token> tkVector;
        TreeNode* node1;
        TreeNode* node2;
        TreeNode* node3;
        TreeNode* node4;
};

class BinaryTree
{

public:
        void add_node(TreeNode*);

        void print_inorder();

        BinaryTree()
        {
                root = NULL;
        }

        ~BinaryTree()
        {
                destroyTree(root);
        }



private: 

	TreeNode* root;

        void insert(TreeNode*&, TreeNode*&);

        void destroyTree(TreeNode*);

        void displayInOrder(TreeNode*);

};

#endif
