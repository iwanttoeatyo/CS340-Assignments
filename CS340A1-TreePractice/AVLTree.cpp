#include "AVLTree.h"
#include <iostream>
#include <iomanip>
#include <cassert>

AVLTree::AVLTree() :root(0), totalDepth(0)
{}

AVLTree::~AVLTree()
{
	deleteTree();
}

int AVLTree::getTotalDepth() const
{
	return totalDepth;
}

bool AVLTree::insert(int key)
{
	//create root if tree is empty
	if (root == 0)
	{
		root = new TreeNode(key);
		return true;
	}
	else
	{
		//recursive insert
		return insert(key, root, 0);
	}
}

void AVLTree::deleteTree()
{
	if (root)
	{
		//recursive delete
		deleteTree(root);
		delete root;
		root = 0;
	}
}
void AVLTree::verboseDeleteTree()
{
	if (root)
	{
		//recursive delete
		verboseDeleteTree(root);
		std::cout << "\nRoot node with key " << root->getKey();
		std::cout << " has been deleted.\n\n";
		delete root;
		root = 0;
		printPostOrder();
		std::cout << "This AVL Tree has deleted every node.\n";
	}
}



void AVLTree::printPostOrder() const
{
	if (root == 0)
	{
		std::cout << "AVL Tree is empty.\n\n";
	}
	else
	{
		//recursive print
		postOrder(root, 0);
	}
}


bool AVLTree::insert(int key, TreeNode * &tree, int depth)
{
	//increment depth because depth increases for every
	//recursive leaf node
	depth++;

	bool inserted = false;

	//If key is smaller than current node, insert on left side
	if (key < tree->getKey())
	{
		//add to left child if empty
		if (tree->getLeft() == 0)
		{
			tree->setLeft(new TreeNode(key));
			totalDepth += depth;
			inserted = true;
		}
		else
		{
			//try to insert to this nodes left child
			inserted = insert(key, tree->getLeftA(), depth);
		}

		if (inserted)
		{
			assert(tree->getLeft());

			//Calculate height difference
			//Rotate if left is deeper than right
			if (getHeightDifference(tree->getLeft(), tree->getRight()) == 2)
			{
				//If rotation occurs tree will lose 1 depth
				totalDepth--;
				if (key < tree->getLeft()->getKey())
				{
					llRotation(tree);
				}
				else
				{
					lrRotation(tree);
				}
			}
		}

	}
	else if (key > tree->getKey())
	{
		//add to right child if empty
		if (tree->getRight() == 0)
		{
			tree->setRight(new TreeNode(key));
			totalDepth += depth;
			inserted = true;
		}
		else
		{
			//try to insert to this nodes right child
			inserted = insert(key, tree->getRightA(), depth);
		}
		if (inserted)
		{
			assert(tree->getRight());
			//Calculate height difference
			//Rotate if right is deeper than left
			if (getHeightDifference(tree->getLeft(), tree->getRight()) == -2)
			{
				//If rotation occurs tree will lose 1 depth
				totalDepth--;
				if (key > tree->getRight()->getKey())
				{
					rrRotation(tree);
				}
				else
				{
					rlRotation(tree);
				}
			}
		}

	}
	else
	{
		//Duplicate key exists in tree
		inserted = false;
	}
	//recalculate this nodes height
	if (inserted)
	{
		fixHeight(tree);
	}

	return inserted;

}

//Hilderman's standard AVL rotation algorithm
//http://www2.cs.uregina.ca/~hilder/cs210/Algorithms/avlLL.txt
void AVLTree::llRotation(TreeNode * &tree)
{
	TreeNode *leftSubTree = tree->getLeft();
	tree->setLeft(leftSubTree->getRight());
	leftSubTree->setRight(tree);

	fixHeight(tree);
	fixHeight(leftSubTree);
	tree = leftSubTree;
}

//Hilderman's standard AVL rotation algorithm
//http://www2.cs.uregina.ca/~hilder/cs210/Algorithms/avlRR.txt
void AVLTree::rrRotation(TreeNode * &tree)
{
	TreeNode *rightSubTree = tree->getRight();
	tree->setRight(rightSubTree->getLeft());
	rightSubTree->setLeft(tree);

	fixHeight(tree);
	fixHeight(rightSubTree);
	tree = rightSubTree;
}

//Hilderman's standard AVL rotation algorithm
//http://www2.cs.uregina.ca/~hilder/cs210/Algorithms/avlLR.txt
void AVLTree::lrRotation(TreeNode * &tree)
{
	rrRotation(tree->getLeftA());
	llRotation(tree);
}

//Hilderman's standard AVL rotation algorithm
//http://www2.cs.uregina.ca/~hilder/cs210/Algorithms/avlRL.txt
void AVLTree::rlRotation(TreeNode * &tree)
{
	llRotation(tree->getRightA());
	rrRotation(tree);
}



void AVLTree::postOrder(TreeNode * tree, int indent) const
{
	if (tree)
	{
		if (tree->getRight())
		{
			//prints right of tree at the top of output
			//farthest right child node printed first
			//and farthest indented
			postOrder(tree->getRight(), indent + 4);
		}
		if (indent)
		{
			std::cout << std::setw(indent) << ' ';
		}
		if (tree->getRight()) std::cout << " /\n" << std::setw(indent) << ' ';
		//prints this node
		std::cout << tree->getKey() << "\n ";
		if (tree->getLeft())
		{
			//prints down the left side of the tree
			//farthest left child is printed last
			std::cout << std::setw(indent) << ' ' << " \\\n";
			postOrder(tree->getLeft(), indent + 4);
		}
	}
}

void AVLTree::verboseDeleteTree(TreeNode * tree)
{
	assert(tree);
	if (tree->getLeft())
	{
		verboseDeleteTree(tree->getLeft());
		std::cout << "\nNode with key " << tree->getLeft()->getKey();
		std::cout << " has been deleted.\n\n";
		delete tree->getLeft();
		//Set deleted subtrees back to NULL
		tree->setLeft(0);
		printPostOrder();
	}
	if (tree->getRight())
	{
		verboseDeleteTree(tree->getRight());
		std::cout << "\nNode with key " << tree->getRight()->getKey();
		std::cout << " has been deleted.\n\n";
		delete tree->getRight();
		//Set deleted subtrees back to NULL
		tree->setRight(0);
		printPostOrder();
	}
}

void AVLTree::deleteTree(TreeNode * tree)
{
	assert(tree);
	if (tree->getLeft())
	{
		deleteTree(tree->getLeft());
		delete tree->getLeft();
		//Set deleted subtrees back to NULL
		tree->setLeft(0);
	}
	if (tree->getRight())
	{
		deleteTree(tree->getRight());
		delete tree->getRight();
		//Set deleted subtrees back to NULL
		tree->setRight(0);
	}
}

void AVLTree::fixHeight(TreeNode* tree)
{
	//Ternary to get height of child nodes if not NULL. NULL has -1 height
	int leftTreeHeight = tree->getLeft() ? tree->getLeft()->getHeight(): -1;
	int rightTreeHeight = tree->getRight() ? tree->getRight()->getHeight(): -1;

	//Get max height of left and right sub trees
	int tempMaxHeight = max(leftTreeHeight, rightTreeHeight);
	//Add 1 to height because current node has 1 more height
	//than its tallest child
	tempMaxHeight += 1;
	tree->setHeight(tempMaxHeight);
}

int AVLTree::getHeightDifference(TreeNode * left, TreeNode * right) const
{
	//Ternary to get TreeNode height, NULL TreeNode has -1 height
	int leftHeight = left ? left->getHeight() : -1;
	int rightHeight = right ? right->getHeight() : -1;
	return leftHeight - rightHeight;
}


int AVLTree::max(int a, int b) const
{
	return (a > b ? a : b);
}