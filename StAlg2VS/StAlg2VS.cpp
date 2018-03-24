//used libraries
#include "stdafx.h"
#include <fstream>//file stream
#include <iostream>//input, output stream
#include <conio.h>//getch
#include <iomanip>//string comparison
#include <windows.h>//localization
#include <stdlib.h>//random
#include <time.h>//time
#include <string>//strings


class Data {//class of tree nodes' data
private:
	int key;
	int value;

public:
	Data(int value) {//constructor
		this->value = value;
		this->key = value;
	}

	int getKey() {//key getter
		return key;
	}

	int getValue() {//value getter
		return value;
	}

	void setKey(int key) {//key setter
		this->key = key;
	}

	void setValue(int value) {//value setter
		this->value = value;
	}
};

class Node {//class of tree nodes
public:
	Data *data;
	Node *left;
	Node *right;

	Node(Data *data) {//constructor
		this->data = data;
		this->left = NULL;
		this->right = NULL;
	}

	void print() {//node data output
		std::cout << "this = " << this << " Key = " << data->getKey() << " Value = " << data->getValue()
			<< " Left = " << left << " Right = " << right << std::endl;
	}

	void printDOT() {//node data output in DOT format
		std::cout << data->getValue() << " -> ";
		if (left != NULL)
			std::cout << left->data->getValue() << ";" << std::endl;
		else
			std::cout << "NULL;" << std::endl;
		std::cout << data->getValue() << " -> ";
		if (right != NULL)
			std::cout << right->data->getValue() << ";" << std::endl;
		else
			std::cout << "NULL;" << std::endl;
	}

	~Node() {//node destructor
		delete data;
	}
};

class BinaryTree {//binary tree class
private:
	Node * root;

	void inOrderPrint(Node* root) {//in-order data output
		if (isEmpty(root))
			return;
		inOrderPrint(root->left);
		root->print();
		inOrderPrint(root->right);
	}

	void inOrderPrintDOT(Node* root) {//in-order data output in DOT format
		if (isEmpty(root))
			return;
		inOrderPrintDOT(root->left);
		root->printDOT();
		inOrderPrintDOT(root->right);
	}

	void rotateLeft(Node* preRoot, Node* currentRoot) {//left rotation
		if (currentRoot == root) {//case of root rotation
			Node *temp = currentRoot->right->left;
			this->root = currentRoot->right;
			currentRoot->right->left = currentRoot;
			currentRoot->right = temp;
			temp = NULL;
		}
		else {//case of not-root rotation
			Node *temp = currentRoot->right->left;
			if (preRoot->left == currentRoot) {//detect if static node child is left or right
				preRoot->left = currentRoot->right;
			}
			else {
				preRoot->right = currentRoot->right;
			}
			currentRoot->right->left = currentRoot;
			currentRoot->right = temp;
			temp = NULL;
		}
	}

	void rotateRight(Node* preRoot, Node* currentRoot) {//right rotation
		if (currentRoot == root) {//case of root rotation
			Node *temp = currentRoot->left->right;
			this->root = currentRoot->left;
			currentRoot->left->right = currentRoot;
			currentRoot->left = temp;
			temp = NULL;
		}
		else {//case of non-root rotation
			Node *temp = currentRoot->left->right;
			if (preRoot->right == currentRoot) {//detect if static node child is left or right
				preRoot->right = currentRoot->left;
			}
			else {
				preRoot->left = currentRoot->left;
			}
			currentRoot->left->right = currentRoot;
			currentRoot->left = temp;
			temp = NULL;
		}

	}

	void convertToVine(Node *currentNode) {//conversion of unbalanced tree to left-oriented vine
		if (!isEmpty(root->right)) {//if root still has right sub-tree
			while (!isEmpty(root->right))//rotate until no right sub-tree
			{
				rotateLeft(root, root);
			}
			convertToVine(root);//call this function again to proceed to other nodes
		}
		else {//if root has not right sub-tree
			if (!isEmpty(currentNode->left)) {//if there are nodes to proceed
				while (!isEmpty(currentNode->left->right))//rotate until no right sub-tree
				{
					rotateLeft(currentNode, currentNode->left);
				}
				convertToVine(currentNode->left);//call this function again to next node
			}
		}
		return;
	}

	void convertToTree(Node *currentNode) {//conversion of left-oriented vine to balanced tree
		int quantity = 0, k;
		Node *preRoot = NULL, *currentRoot = root;
		countElements(root, quantity);//count elements in tree
		k = (int)log2(quantity + 1);//count tree height
		for (int i = 1; i < k; i++) {//iterate through conversion levels
			rotateRight(root, root);//root rotation
			for (int j = 1; j < pow(2, k - i) - 1; j++) {//iterate through rotations on one level
				preRoot = NULL;//stable and unstable nodes reset
				currentRoot = root;
				for (int k = 1; k <= j; k++) {//searching current nodes to perform rotation
					preRoot = currentRoot;
					currentRoot = currentRoot->left;
				}
				rotateRight(preRoot, currentRoot);//not-root rotation
			}
		}
	}

	void countElements(Node* root, int &quantity) {//tree elements' counter
		if (isEmpty(root))
			return;
		countElements(root->left, quantity);
		quantity++;
		countElements(root->right, quantity);
	}

	void insert(Node *root, Data *data) {//node insertion
		if (root->data->getKey() > data->getKey()) {//if insert to the left
			if (isEmpty(root->left)) {//if empty place has been found
				root->left = new Node(data);
			}
			else {//if needed to search further
				insert(root->left, data);
			}
		}
		else {//if insert to the right or to this place
			if (root->data->getKey() < data->getKey()) {//if insert to the right
				if (isEmpty(root->right)) {//if empty place has been found
					root->right = new Node(data);
				}
				else {//if needed to search further
					insert(root->right, data);
				}
			}
			else {//if insert to this place
				root->data->setValue(data->getValue());//replacement of node value
				delete data;
			}
		}
	};

	bool remove(Node *root, Data *data) {//node removal
		if (!isEmpty(root)) {//if tree exists
			if (root->data->getKey() > data->getKey()) {//if removal from left sub-tree
				if (!isEmpty(root->left)) {//if left sub-tree exists
					if (root->left->data->getKey() == data->getKey()) {//if element has been found
						if (root->left->left == NULL && root->left->right == NULL) {//if element to be removed has no children
							Node *temp = root->left;
							root->left = NULL;
							delete temp;
							delete data;
						}
						else {
							if (root->left->left == NULL || root->left->right == NULL) {//if element to be removed has one child
								if (root->left->left == NULL) {//if element to be removed has right child
									Node *temp = root->left;
									root->left = root->left->right;
									delete temp;
									delete data;
								}
								else {//if element to be removed has left child
									Node *temp = root->left;
									root->left = root->left->left;
									delete temp;
									delete data;
								}
							}
							else {
								if (root->left->left != NULL && root->left->right != NULL) {//if element to be removed has both children
									Node* currentRoot = root->left->right;
									Node* previousRoot = root->left;
									while (currentRoot->left != NULL) {//search for the smallest value in right sub-tree
										previousRoot = currentRoot;
										currentRoot = currentRoot->left;
									}
									Node* temp = root->left;
									root->left = currentRoot;
									root->left->left = temp->left;
									if (currentRoot->right != NULL) {//fixes after moving the smallest element
										previousRoot->left = currentRoot->right;
									}
									else
										previousRoot->left = NULL;
									if (previousRoot == temp) {//in case of the smallest element been the direct child of element to be removed
										root->left->right = temp->right->right;
									}
									else
										root->left->right = temp->right;
									temp = NULL;
									currentRoot = NULL;
									previousRoot = NULL;
									delete temp;
									delete currentRoot;
									delete previousRoot;
									delete data;
								}
							}
						}
						return true;//element found
					}
					else {//if needed to search further
						remove(root->left, data);
					}
				}
				else//element not found
					return false;
			}
			else {//if removal from right sub-tree or root deletion
				if (root->data->getKey() < data->getKey()) {//removal from right sub-tree
					if (!isEmpty(root->right)) {//if right sub-tree exists
						if (root->right->data->getKey() == data->getKey()) {//if element has been found
							if (root->right->left == NULL && root->right->right == NULL) {//if element to be removed has no children
								Node *temp = root->right;
								root->right = NULL;
								delete temp;
								delete data;
							}
							else {
								if (root->right->left == NULL || root->right->right == NULL) {//if element to be removed has one child
									if (root->right->left == NULL) {//if element to be removed has right child
										Node *temp = root->right;
										root->right = root->right->right;
										delete temp;
										delete data;
									}
									else {//if element to be removed has left child
										Node *temp = root->right;
										root->right = root->right->left;
										delete temp;
										delete data;
									}
								}
								else {
									if (root->right->left != NULL && root->right->right != NULL) {//if element to be removed has both children
										Node* currentRoot = root->right->right;
										Node* previousRoot = root->right;
										while (currentRoot->left != NULL) {//search for the smallest value in right sub-tree
											previousRoot = currentRoot;
											currentRoot = currentRoot->left;
										}
										Node* temp = root->right;
										root->right = currentRoot;
										root->right->left = temp->left;
										if (currentRoot->right != NULL) {//fixes after moving founded the smallest element
											previousRoot->left = currentRoot->right;
										}
										else
											previousRoot->left = NULL;
										if (previousRoot == temp) {//in case of the smallest element been the direct child of element to be removed
											root->right->right = temp->right->right;
										}
										else
											root->right->right = temp->right;
										temp = NULL;
										currentRoot = NULL;
										previousRoot = NULL;
										delete temp;
										delete currentRoot;
										delete previousRoot;
										delete data;
									}
								}
							}
							return true;
						}
						else {//if needed to search further
							remove(root->right, data);
						}
					}
					else
						return false;
				}
				else {// root deletion
					if (!isEmpty(root)) {//if tree exists
						if (root->left == NULL && root->right == NULL) {//if element to be removed has no children
							Node *temp = root;
							root = NULL;
							this->root = NULL;
							delete temp;
							delete data;
						}
						else {
							if (root->left == NULL || root->right == NULL) {//if element to be removed has one child
								if (root->left == NULL) {//if element to be removed has right child
									Node *temp = root;
									root = root->right;
									this->root = root;
									delete temp;
									delete data;
								}
								else {//if element to be removed has left child
									Node *temp = root;
									root = root->left;
									this->root = root;
									delete temp;
									delete data;
								}
							}
							else {
								if (root->left != NULL && root->right != NULL) {//if element to be removed has both children
									Node* currentRoot = root->right;
									Node* previousRoot = root;
									while (currentRoot->left != NULL) {//search for the smallest value in right sub-tree
										previousRoot = currentRoot;
										currentRoot = currentRoot->left;
									}
									Node* temp = root;
									root = currentRoot;
									root->left = temp->left;
									if (currentRoot->right != NULL) {//fixes after moving founded the smallest element
										previousRoot->left = currentRoot->right;
									}
									else
										previousRoot->left = NULL;
									if (previousRoot == temp) {//in case of the smallest element been the direct child of element to be removed
										root->right = temp->right->right;
									}
									else
										root->right = temp->right;
									this->root = currentRoot;
									temp = NULL;
									currentRoot = NULL;
									previousRoot = NULL;
									delete temp;
									delete currentRoot;
									delete previousRoot;
									delete data;
								}
							}
						} //element was found
						return true;
					}
					else//element was not found
						return false;
				}
			}
		}
	}

	bool searchByValue(Node *root, Data *data) {//node search
		if (root->data->getKey() > data->getKey()) {//search in left sub-tree
			if (!isEmpty(root->left)) {//if not leaf
				searchByValue(root->left, data);
			}
			else//element not found
				return false;
		}
		else {//search in right sub-tree or element found
			if (root->data->getKey() < data->getKey()) {//search in right sub-tree
				if (!isEmpty(root->right)) {//if not leaf
					searchByValue(root->right, data);
				}
				else//element not found
					return false;
			}
			else {//element found
				delete data;
				return true;
			}
		}
	};

	void inOrderDelete(Node* root) {//in-order data deletion
		if (isEmpty(root))
			return;
		inOrderDelete(root->left);
		inOrderDelete(root->right);
		delete root;
	}

public:
	BinaryTree() {//tree constructor
		root = NULL;
	}

	void insert(int value) {//public data insertion
		Data *data = new Data(value);
		if (isEmpty(root)) {//if tree is empty
			root = new Node(data);
		}
		else {
			insert(root, data);
		}
	};

	bool remove(int value) {//public data removal
		Data *data = new Data(value);
		return remove(root, data);
	};

	bool searchByValue(int value) {//public data search
		Data *data = new Data(value);
		return searchByValue(root, data);
	};

	bool isEmpty(Node *root) {//check if node is empty
		if (root == NULL) {
			return true;
		}
		else {
			return false;
		}
	}

	bool balanceTree() {
		int quantity = 0;
		countElements(root, quantity);//count elements in tree
		if ((quantity + 1) != 0 && ((quantity + 1) & (quantity)) == 0) {//if tree contains 2^k-1 elements
			convertToVine(root);
			convertToTree(root);
			return true;
		}
		else//balancing can not be done
			return false;
	}

	void inOrderPrint() {//tree printout
		inOrderPrint(root);
		std::cout << std::endl;
		inOrderPrintDOT(root);
	}

	~BinaryTree() {
		//int quantity = 0;
		//countElements(root, quantity);//count elements in tree
		//for (int i = 1; i < quantity; i++) {
		//	remove(root, root->data);
		//}
		inOrderDelete(root);
		root = NULL;
	}
};

//functions prototypes
void outputMainMenu();//menu and info
short treeTypeSelect(short treeType);//tree type selection
void outputTreeTypeMenu(short treeType);//tree type setup
void aboutProgramme();//about programme
bool unitTesting(int testNumber);//unit testing

const int TESTS_NUMBER = 33;//number of units tests

int main()//main function
{
	short operationCode = 0, passedTests = 0, treeType = 1;
	BinaryTree *binaryTree = new BinaryTree;
	int value;
	Node *current;
	srand(time(NULL));//initialization of random seed
	SetConsoleOutputCP(1251);//console localization
	SetConsoleCP(1251);
	outputMainMenu();//show menu of available commands
	while (operationCode != 48)//repeat until exit
	{
		operationCode = _getch();//get command
		system("cls");//clear console screen
		switch (operationCode)//command choice
		{
		case 49://insert to tree
			std::cout << "INSERT TO TREE\n";
			std::cout << "Enter value to insert:\n";//value input
			std::cin >> value;
			binaryTree->insert(value);//node insertion
			std::cout << "Done!\n";
			break;
		case 50://remove from tree
			std::cout << "REMOVE FROM TREE\n";
			std::cout << "Enter value to remove:\n";//index input
			std::cin >> value;
			if (binaryTree->remove(value)) {//node removal
				std::cout<<"Element \""<<value<<"\" was successfully removed\n";
			}
			else//fallback error
				std::cout<<"Tree does not contain element with value "<<value<<std::endl;
			std::cout << "Done!\n";
			break;
		case 51://search data
			std::cout << "SEARCH IN TREE\n";
			std::cout << "Enter value to search:\n";//value input
			std::cin >> value;
			if (binaryTree->searchByValue(value)) {//node search
				std::cout << "Element \"" << value << "\" was found\n";
			}
			else//fallback error
				std::cout << "Element \"" << value << "\" was not found\n";
			std::cout << "Done!\n";
			break;
		case 52://tree rebalancing
			std::cout << "TREE REBALANCING\n";
			if (binaryTree->balanceTree()) {//tree rebalancing
				std::cout << "Tree was successfully balanced!\n";
			}
			else//fallback error
				std::cout << "Tree doesn't have 2^k+1 elements and can not be balanced\n";
			std::cout << "Done!\n";
			break;
		case 53://tree output
			std::cout << "TREE PRINTOUT\n";
			binaryTree->inOrderPrint();//tree output
			std::cout << "Done!\n";
			break;
		case 54://tree destruction
			std::cout << "TREE DESTRUÑTION\n";
			delete binaryTree;
			binaryTree = new BinaryTree();
			std::cout << "Done!\n";
			break;
		case 55://unit testing
			std::cout << "UNIT TESTING\n";
			//for (int i = 0; i < TESTS_NUMBER; ) {//go through all tests
			//	i++;//go to next test
			//	std::cout << "Test " << std::setw(2) << i << ": ";//print test number
			//	if (unitTesting(i)) {//if current test passed
			//		std::cout << "Passed\n";
			//		passedTests++;//increase quantity of passed tests
			//	}
			//	else {//if current test failed
			//		std::cout << "Failed\n";
			//	}
			//}
			//std::cout << "\nPassed tests: " << passedTests << "/" << TESTS_NUMBER << "\n";//print summary
			//passedTests = 0;//reset passed tests quantity
			std::cout << "Unit testing is not available in free version of programme\n";
			std::cout << "Done!\n";
			break;
		case 56:
			binaryTree->createFakeTree();
			binaryTree->inOrderPrint();
			treeType = treeTypeSelect(treeType);
			std::cout << "Tree type choosing is not available in free version of programme, reverting changes...\n";
			std::cout << "Current tree type is binary tree without self-balancing\n";

			break;
		case 57://show info about programme
			aboutProgramme();
			break;
		case 48://programme shut down
				//delete myCircularDoublyLinkedList;//delete list
			exit;
			break;
		default://case of pressing button not assigned to commands
			outputMainMenu();//show menu of available commands
		}
	}
	return 0;
}

bool unitTesting(int testNumber) {//unit testing
		BinaryTree *unitBinaryTree = new BinaryTree;
		int quantity = 0;
		unitBinaryTree->insert(5);
		unitBinaryTree->insert(15);
		unitBinaryTree->insert(12);
		unitBinaryTree->insert(20);
		unitBinaryTree->insert(4);
		unitBinaryTree->insert(8);
		unitBinaryTree->insert(16);
		unitBinaryTree->insert(23);
		unitBinaryTree->insert(42);
		unitBinaryTree->insert(108);
		unitBinaryTree->insert(23);
		unitBinaryTree->insert(5);
		unitBinaryTree->insert(10);
		unitBinaryTree->insert(3);
		unitBinaryTree->insert(7);
		unitBinaryTree->insert(9);
		unitBinaryTree->insert(41);
		unitBinaryTree->balanceTree();
	return true;
}

short treeTypeSelect(short treeType)//sorting data
{
	short OperationCode = 0;//defining variables
	outputTreeTypeMenu(treeType);//show menu of available commands
	while (OperationCode < 48 || OperationCode>57)//repeat until exit
	{
		OperationCode = _getch();//get command
	}
	system("cls");//clear console screen
	outputTreeTypeMenu(OperationCode - 48);//show menu of available commands
	return OperationCode - 48;
}

void outputMainMenu()//show menu of available commands in main menu
{
	std::cout << "MAIN MENU\n";
	std::cout << "Choose operation:\n";
	std::cout << "Press 1 to insert data into tree\n";
	std::cout << "Press 2 to delete data from tree\n";
	std::cout << "Press 3 to search data in tree\n";
	std::cout << "Press 4 to balance tree\n";
	std::cout << "Press 5 to print tree\n";
	std::cout << "Press 6 to clear tree\n";
	std::cout << "Press 7 to perform unit testing\n";
	std::cout << "Press 8 to change tree type\n";
	std::cout << "Press 9 to get info about the programme\n";
	std::cout << "Press 0 to exit";
}

void outputTreeTypeMenu(short treeType)//show menu of available commands in main menu
{
	std::cout << "TREE TYPE CHOICE\n";
	std::cout << "Choose tree type:\n";
	std::cout << "Press 1 to select binary tree without self-balancing\n";
	std::cout << "Press 2 to select AVL tree\n";
	std::cout << "Press 3 to select Red-Black tree\n";
	std::cout << "Press 4 to select splay tree\n";
	std::cout << "Press 5 to select B-tree\n";
	std::cout << "Press 6 to select R-tree\n";
	std::cout << "Press 7 to select R*-tree\n";
	std::cout << "Press 8 to select M-tree\n";
	std::cout << "Press 9 to select AA-tree\n";
	std::cout << "Press 0 to select 2-3 tree\n";
	std::cout << "Current tree type is ";
	switch (treeType)//command choice
	{
	case 1://sort by code
		std::cout << "binary tree without self-balancing" << std::endl;
		break;
	case 2://sort by last name
		std::cout << "AVL tree" << std::endl;
		break;
	case 3://sort by first name
		std::cout << "Red-black tree" << std::endl;
		break;
	case 4://sort by middle name
		std::cout << "splay tree" << std::endl;
		break;
	case 5://sort by group
		std::cout << "B-tree" << std::endl;
		break;
	case 6://sort by grades
		std::cout << "R-tree" << std::endl;
		break;
	case 7://exit submenu
		std::cout << "R*-tree" << std::endl;
		break;
	case 8:
		std::cout << "M-tree" << std::endl;
		break;
	case 9:
		std::cout << "AA-tree" << std::endl;
		break;
	case 0:
		std::cout << "2-3 tree" << std::endl;
		break;
	default://case of pressing button not assigned to commands
		std::cout << " UNKNOWN tree" << std::endl;//show menu of available commands
	}
}

void aboutProgramme()//show info about the programme
{
	std::cout << "ABOUT PROGRAMME\n\n";
	std::cout << "Algorithms and data structures\n";
	std::cout << "Practical assignment #2\n";
	std::cout << "Binary tree without self-balancing\n\n";
	std::cout << "Version 0.1.0.0 build 20180324042000\n\n";
	std::cout << "Development and testing: Yaskovich Dmitry (ISBO-05-15)\n\n";
	std::cout << "Dimini Inc, 2018";
}