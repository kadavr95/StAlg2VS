//used libraries
#include <fstream>//file stream
#include <iostream>//input, output stream
#include <conio.h>//getch
#include <iomanip>//string comparison
#include <windows.h>//localization
#include <stdlib.h>//random
#include <time.h>//time
#include <string>//strings

class Data{//class of tree nodes' data
private:
	int key;
	int value;

public:
	Data(int value){//constructor
		this->value = value;
		//value = (char)(key + '0');//just another magic value generator
		this->key = value;
	}

	int getKey(){//key getter
		return key;
	}

	int getValue(){//value getter
		return value;
	}

	int setKey(int key){//key setter
		this->key=key;
	}

	int setValue(int value){//value setter
		this->value=value;
	}
};

class Node{
public:
	//int value;
	Data *data;
	Node *left;
	Node *right;

	Node(Data *data){
		this->data = data;
		this->left = NULL;
		this->right = NULL;
    }

	void print(){
		std::cout << "this = " << this << "Key = " << data->getKey() << " Value = " << data->getValue()
			<< " Left = " << left << " Right " << right << std::endl;
	}

	void printDOT(){
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

	~Node(){
		delete data;
	}
};

//class Node{//class of list node
//
//public:
//	Data *data;
//	Node *next;
//	Node *previous;
//
//	Node(Data *data){//constructor
//		this->data = data;
//		this->next = NULL;
//		this->previous = NULL;
//	}
//
//	~Node(){//destructor
//		delete data;
//	}
//};

class BinaryTree{
private:
	Node * root;

	void inOrderPrint(Node* root){
		if (isEmpty(root))
			return;
		inOrderPrint(root->left);
		root->print();
		inOrderPrint(root->right);
	}
	void inOrderPrintDOT(Node* root){
		if (isEmpty(root))
			return;
		inOrderPrintDOT(root->left);
		root->printDOT();
		inOrderPrintDOT(root->right);
	}
public:
	BinaryTree(){
		root = NULL;
	}

	void insert(int value){
		Data *data = new Data(value);
		if (isEmpty(root)) {
			root = new Node(data);
			//root->data->value = value;
		}
		else{
			insert(root, data);
		}

	};

	void insert(Node *root, Data *data){
		if (root->data->getKey()>data->getKey()) {
			if (isEmpty(root->left)) {
				root->left=new Node(data);
			}
			else{
				insert(root->left, data);
			}
		}
		else{
			if (root->data->getKey()<data->getKey()) {
				if (isEmpty(root->right)) {
					root->right=new Node(data);
				}
				else{
					insert(root->right, data);
				}
			}
			else{
				root->data->setValue(data->getValue());
				delete data;
			}
		}

	};

	bool remove(Node *root, Data *data){
		if (root->data->getKey()>data->getKey()) {
			if (!isEmpty(root->left)) {
				if (root->left->data->getKey()==data->getKey()) {
					if (root->left->left==NULL && root->left->right==NULL) {
						Node *temp = root->left;
						root->left=NULL;
						delete temp;
						delete data;
					}
					else{
						if (root->left->left==NULL || root->left->right==NULL) {
							if (root->left->left==NULL) {
								Node *temp = root->left;
								root->left=root->left->right;
								delete temp;
								delete data;
							}
							else{
								Node *temp = root->right;
								root->right=root->right->left;
								delete temp;
								delete data;
							}
						}
						else{
							if (root->left->left!=NULL && root->left->right!=NULL) {
								//detect subtrees height and work with higher tree(or maybe with more elements)
								//find lowest in the left sub-tree (or biggest in right) and make it new parent. deal with its child(it can be only one(right for right sub-tree) as usual
								Node* currentRoot=root->left->right;
								Node* previousRoot=root->left;
								while (currentRoot->left!=NULL){
									previousRoot=currentRoot;
									currentRoot=currentRoot->left;
								}
								Node* temp=root->left;
								root->left=currentRoot;
								root->left->left=temp->left;
								if (currentRoot->right!=NULL) {
									previousRoot->left=currentRoot->right;
								}
								root->left->right=temp->right;

								delete temp;
								delete currentRoot;
								delete previousRoot;
								delete data;


							}
						}
					}
					return true;
				}
				else{


					remove(root->left, data);
				}
			}
			else
				return false;
		}
		else{
			if (root->data->getKey()<data->getKey()) {
				if (!isEmpty(root->right)) {
					if (root->right->data->getKey()==data->getKey()) {
						if (root->right->left==NULL && root->right->right==NULL) {
							Node *temp = root->right;
							root->right=NULL;
							delete temp;
							delete data;
						}
						else{
							if (root->right->left==NULL || root->right->right==NULL) {
								if (root->right->left==NULL) {
									Node *temp = root->right;
									root->right=root->right->right;
									delete temp;
									delete data;
								}
								else{
									Node *temp = root->right;
									root->right=root->right->left;
									delete temp;
									delete data;
								}
							}
							else{
								if (root->right->left!=NULL && root->right->right!=NULL) {

								}
							}
						}
						return true;
					}
					else{


						remove(root->right, data);
					}
				}
				else
					return false;
			}
			else{// root deletion
				delete data;

				return true;
			}
		}
	}

	bool remove(int value){
        Data *data = new Data(value);
		return remove(root, data);

	};

	bool searchByValue(Node *root, Data *data){
		if (root->data->getKey()>data->getKey()) {
			if (!isEmpty(root->left)) {
				searchByValue(root->left, data);
			}
			else
				return false;
		}
		else{
			if (root->data->getKey()<data->getKey()) {
				if (!isEmpty(root->right)) {
					searchByValue(root->right, data);
				}
				else
					return false;
			}
			else{
				delete data;
				return true;
			}
		}
	};

	bool searchByValue(int value){
		Data *data = new Data(value);
		searchByValue(root, data);
	};

	bool isEmpty(Node *root){
		if (root==NULL) {
			return true;
		}
		else{
			return false;
		}
	}

	void createFakeTree(){
		root = new Node(new Data(10));
		//root->value = 10;
		insert(5);
		insert(15);
		insert(12);
		insert(20);
		insert(4);
		insert(8);
		insert(16);
		insert(23);
		insert(42);
		insert(108);
		insert(23);
		insert(5);
		insert(10);
		insert(3);
		insert(7);
		insert(9);
//		remove(4);
//		remove(42);
		remove(5);

//		std::cout<<searchByValue(23)<<std::endl;
//		std::cout<<searchByValue(0)<<std::endl;
//		std::cout<<searchByValue(8)<<std::endl;
//		std::cout<<searchByValue(18)<<std::endl;
//		std::cout<<searchByValue(10)<<std::endl;


		//root->left = new Node(new Data(5));
		//root->left->value = 5;

		//Node *n = new Node(new Data(15));
		//n->value = 15;
		//root->right = n;
	}

	void inOrderPrint(){
		inOrderPrint(root);
		std::cout<<std::endl;
		inOrderPrintDOT(root);
	}
};

//class CircularDoublyLinkedList{//class of circular doubly linked list
//	Node *head;
//
//public:
//	CircularDoublyLinkedList(){//constructor
//		head = NULL;//setting head to default NULL value
//	}
//
//	Node* getHead(){//head address getter
//		return head;
//	}
//
//	void print(Node* current){//list printout
//		if (!CircularDoublyLinkedList::isEmpty()) {//if list is not empty
//			std::cout << current->data->value << "\n";//printing value
//			if (!(current->next == head)) {//moving to next element
//				print(current->next);
//			}
//		}
//		else//if list is empty
//			std::cout << "\n";
//	}
//
//	void push(Data *newData, char nextData){//addition to list
//		Data *data = newData;
//		if (CircularDoublyLinkedList::isEmpty()) {//if list is empty
//			Node *newNode = new Node(data);//node creation
//			head = newNode;//head and neighbours links setting
//			newNode->next = head;
//			newNode->previous = head;
//		}
//		else{//if list is not empty
//			Node *current = CircularDoublyLinkedList::searchByKey(nextData);//search presence of element to insert before it
//			if (current!=NULL) {//if addition before element
//				Node *newNode = new Node(data);//new node creation
//				current->previous->next = newNode;//new node and neighbours nodes links rerouting
//				newNode->next = current;
//				newNode->previous=current->previous;
//				current->previous = newNode;
//				if (current == head) {//if insert to the first place
//					head=newNode;//head adjustment
//				}
//			}
//			else{//if addition to the tail
//				Node *newNode = new Node(data);//new node creation
//				head->previous->next = newNode;//new node and neighbours nodes links rerouting
//				newNode->next = head;
//				newNode->previous=head->previous;
//				head->previous = newNode;
//			}
//		}
//	}
//
//	Node* pop(int index){//deletion from list
//		if (head!=NULL) {//if list is not empty
//			Node* current = head;
//			if (index<=0) {//deny indeces lesser than one
//				return NULL;
//			}
//			while (index>1) {//jumping to element we want to delete
//				index--;
//				current=current->next;
//				if (current==head) {//deny indeces greater than quantity of list elements
//					return NULL;
//				}
//			}
//			current->previous->next=current->next;//rerouting neighbours nodes
//			current->next->previous=current->previous;
//			if (head==current) {//if element to delete is head
//				head=current->next;//head adjustment
//			}
//			if (current==current->next) {//if element is the only one
//				head=NULL;//head reset
//			}
//			Node *currentTemporary= current;//new temporary node to return
//			Node *nullPointer = NULL;//new pointer to erase old data
//			delete current;//element and pointer destruction
//			current = nullPointer;
//			return currentTemporary;
//		}
//		return NULL;//if list is empty
//	}
//
//	bool isEmpty(){//check if list is empty
//		if (head == NULL)//if empty
//			return true;
//		else//if not empty
//			return false;
//	}
//
//	Node* searchByKey(char key){//search element by value
//		Node *current = head;//set head as current
//		if (head != NULL) {//if list is not empty
//			while (true)//unlimited recursion
//			{
//				if (current->data->value ==  key)//if element is found
//					return current;
//				if (current->next == head) {//if all list was checked
//					return NULL;
//				}
//				current=current->next;//jump to next element
//			}
//		}
//		return NULL;//if list is empty;
//	}
//
//	~CircularDoublyLinkedList(){//destructor
//		if (!CircularDoublyLinkedList::isEmpty()) {//if list is not empty
//			Node *last = head->previous;//set last and penultimate element
//			Node *nextLast = NULL;
//			while (last!=head) {//while element has more than one element
//				nextLast=last->previous;//save link to penultimate element
//				delete last;//delete last element
//				last=nextLast;//set new last element
//			}
//			delete last;//deletion of last element in list
//			head=NULL;//head reset
//		}
//	}
//};

//functions prototypes
void outputMainMenu();//menu and info
short treeTypeSelect(short treeType);
void outputTreeTypeMenu(short treeType);//tree type setup
void aboutProgramme();//about programme
bool unitTesting(int testNumber);//unit testing

const int TESTS_NUMBER=33;//number of units tests

int main()//main function
{
	short operationCode=0, passedTests=0, treeType=1;
	//CircularDoublyLinkedList *myCircularDoublyLinkedList = new CircularDoublyLinkedList();
	BinaryTree binaryTree;
	int index;
	char value, valueNext;
	Node *current;
	srand (time(NULL));//initialization of random seed
	SetConsoleOutputCP(1251);//console localization
	SetConsoleCP(1251);
	outputMainMenu();//show menu of available commands
	while(operationCode!=48)//repeat until exit
	{
		operationCode=getch();//get command
		system("cls");//clear console screen
		switch (operationCode)//command choice
		{
			case 49://insert to tree
				std::cout<<"INSERT TO TREE\n";
				std::cout<<"Enter value to insert:\n";//value input
				std::cin>>value;
				//myCircularDoublyLinkedList->push(new Data(value), valueNext);//node addition
				std::cout<<"Done!\n";
				break;
			case 50://remove from tree
				std::cout<<"REMOVE FROM TREE\n";
				std::cout<<"Enter value to remove:\n";//index input
				std::cin>>index;
				//current=myCircularDoublyLinkedList->pop(index);//node deletion
//				if (current!=NULL) {//check if deletion was successful
//					std::cout<<"Element \""<<current->data->value<<"\" was successfully removed\n";
//				}
//				else
//					std::cout<<"List does not contain element with index "<<index<<std::endl;
				std::cout<<"Done!\n";
				break;
			case 51://search data
				std::cout<<"SEARCH IN TREE\n";
				std::cout<<"Enter value to search:\n";//value input
				std::cin>>value;
//				if (myCircularDoublyLinkedList->searchByKey(value)!=NULL)
//					std::cout<<"Element \""<<myCircularDoublyLinkedList->searchByKey(value)<<"\" found\n";
//				else
//					std::cout<<"Element \""<<myCircularDoublyLinkedList->searchByKey(value)<<"\" not found\n";
				std::cout<<"Done!\n";
				break;
			case 52://tree rebalancing
				std::cout<<"TREE REBALANCING\n";
				std::cout<<"Done!\n";
				break;
			case 53://tree output
				std::cout<<"TREE PRINTOUT\n";
				//myCircularDoublyLinkedList->print(myCircularDoublyLinkedList->getHead());
				std::cout<<"Done!\n";
				break;
			case 54://tree destruction
				std::cout<<"TREE DESTRUÑTION\n";
//				delete myCircularDoublyLinkedList;
//				myCircularDoublyLinkedList = new CircularDoublyLinkedList();
				std::cout<<"Done!\n";
				break;
			case 55://unit testing
				std::cout<<"UNIT TESTING\n";
				for (int i = 0; i < TESTS_NUMBER; ) {//go through all tests
					i++;//go to next test
					std::cout<<"Test "<<std::setw(2)<<i<<": ";//print test number
					if (unitTesting(i)) {//if current test passed
						std::cout<<"Passed\n";
						passedTests++;//increase quantity of passed tests
					}
					else{//if current test failed
						std::cout<<"Failed\n";
					}
				}
				std::cout<<"\nPassed tests: "<<passedTests<<"/"<<TESTS_NUMBER<<"\n";//print summary
				passedTests=0;//reset passed tests quantity
				std::cout<<"Done!\n";
				break;
			case 56:
				binaryTree.createFakeTree();
				binaryTree.inOrderPrint();
				treeType=treeTypeSelect(treeType);


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

bool unitTesting(int testNumber){//auto testing if everything is working as intended
//	int value, valueSecond;//initializing variables
//	char valueNext;
//	Node *current;
//	std::string listString="";
//	CircularDoublyLinkedList *myCircularDoublyLinkedList = new CircularDoublyLinkedList();
//	switch (testNumber) {
//		case 1://add to empty list
//			value=rand() % 1000 + 1;//generate random values
//			valueNext=rand() % 1000 + 1;
//			myCircularDoublyLinkedList->push(new Data(value), valueNext);//node addition
//			if (myCircularDoublyLinkedList->searchByKey(value)==NULL){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 2://remove from empty list
//			if (myCircularDoublyLinkedList->pop(rand() % 1000 + 1)==NULL){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 3://search in empty list
//			if (myCircularDoublyLinkedList->searchByKey(rand() % 1000 + 1)==NULL){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 4://print empty list
//			std::cout.setstate(std::ios_base::failbit);//suppressing console list printout
//			myCircularDoublyLinkedList->print(myCircularDoublyLinkedList->getHead());
//			std::cout.clear();//removing suppression
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 5://destroy empty list
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 6://add to list with 1 element before this element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), 'a');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//getting nodes values
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="ba"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//it test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 7://add to list with 1 element to the end
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//getting nodes values
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="ab"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 8://remove from list with 1 element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//node addition
//			if (myCircularDoublyLinkedList->pop(1)->data->value=='a'){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 9://remove wrong element from list with 1 element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//node addition
//			if (myCircularDoublyLinkedList->pop(2)==NULL){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 10://print list with 1 element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//node addition
//			std::cout.setstate(std::ios_base::failbit);//suppressing console list printout
//			myCircularDoublyLinkedList->print(myCircularDoublyLinkedList->getHead());
//			std::cout.clear();//removing suppression
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 11://destroy list with 1 element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//node addition
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 12://add to list with 2 elements before first element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), 'a');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//nodes deletion
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="cab"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 13://add to list with 2 elements before second element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), 'b');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//nodes deletion
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="acb"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 14://add to list with 2 elements to the end
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//nodes deletion
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="abc"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			return true;
//			break;
//		case 15://remove first element from list with 2 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			if (myCircularDoublyLinkedList->pop(1)->data->value=='a'){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 16://remove last element from list with 2 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			if (myCircularDoublyLinkedList->pop(2)->data->value=='b'){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 17://remove wrong element from list with 2 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			if (myCircularDoublyLinkedList->pop(3)==NULL){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 18://print list with 2 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			std::cout.setstate(std::ios_base::failbit);//suppressing console list printout
//			myCircularDoublyLinkedList->print(myCircularDoublyLinkedList->getHead());
//			std::cout.clear();//removing suppression
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 19://destroy list with 2 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 20://add to list with 5 elements before first element
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			myCircularDoublyLinkedList->push(new Data('6'), 'a');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//nodes deletion
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="fabcde"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 21://add to list with 5 elements in the middle of list
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			myCircularDoublyLinkedList->push(new Data('6'), 'd');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//nodes deletion
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="abcfde"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 22://add to list with 5 elements to the end
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			myCircularDoublyLinkedList->push(new Data('6'), '0');
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;//nodes deletion
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			listString+=myCircularDoublyLinkedList->pop(1)->data->value;
//			if (listString=="abcdef"){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 23://remove first element from list with 5 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			if (myCircularDoublyLinkedList->pop(1)->data->value=='a'){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 24://remove element from the middle of the list with 5 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			if (myCircularDoublyLinkedList->pop(3)->data->value=='c'){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 25://remove last element from list with 5 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			if (myCircularDoublyLinkedList->pop(5)->data->value=='e'){//it test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 26://remove wrong element from list with 5 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			if (myCircularDoublyLinkedList->pop(6)==NULL){//if test passed
//				delete myCircularDoublyLinkedList;
//				return true;
//			}
//			else{//if test failed
//				delete myCircularDoublyLinkedList;
//				return false;
//			}
//			break;
//		case 27://print list with 5 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			std::cout.setstate(std::ios_base::failbit);//suppressing console list printout
//			myCircularDoublyLinkedList->print(myCircularDoublyLinkedList->getHead());
//			std::cout.clear();//removing suppression
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 28://destroy list with 5 elements
//			myCircularDoublyLinkedList->push(new Data('1'), '0');//nodes addition
//			myCircularDoublyLinkedList->push(new Data('2'), '0');
//			myCircularDoublyLinkedList->push(new Data('3'), '0');
//			myCircularDoublyLinkedList->push(new Data('4'), '0');
//			myCircularDoublyLinkedList->push(new Data('5'), '0');
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 29://add thousands of elements and delete them
//			for (int i = 0; i < 26000; i++) {
//				myCircularDoublyLinkedList->push(new Data(i), 0);
//			}
//			for (int i = 0; i < 26000; i++) {
//				myCircularDoublyLinkedList->pop(1);
//			}
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 30://add and delete thousands of elements
//			for (int i = 0; i < 26000; i++) {
//				myCircularDoublyLinkedList->push(new Data(i), 0);
//				myCircularDoublyLinkedList->pop(1);
//			}
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 31://add element to list and destroy it thousands of times
//			for (int i = 0; i < 26000; i++) {
//				myCircularDoublyLinkedList->push(new Data(i), 0);
//				delete myCircularDoublyLinkedList;
//				myCircularDoublyLinkedList = new CircularDoublyLinkedList();
//			}
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 32://add thousands of elements and destroy list
//			for (int i = 0; i < 26000; i++) {
//				myCircularDoublyLinkedList->push(new Data(i), 0);
//			}
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		case 33://randomly add thousands of elements and delete them
//			for (int i = 0; i < 26000; i++) {
//				myCircularDoublyLinkedList->push(new Data(rand() % 1000 + 1), rand() % 1000 + 1);
//				myCircularDoublyLinkedList->pop(rand() % 1000 + 1);
//			}
//			delete myCircularDoublyLinkedList;
//			return true;
//			break;
//		default://if unknown unit test number
//			return false;
//	}
}

short treeTypeSelect(short treeType)//sorting data
{
	short OperationCode=0;//defining variables
	outputTreeTypeMenu(treeType);//show menu of available commands
	while(OperationCode<48 || OperationCode>57)//repeat until exit
	{
		OperationCode=getch();//get command
	}
    system("cls");//clear console screen
	outputTreeTypeMenu(OperationCode-48);//show menu of available commands
	return OperationCode-48;
}

void outputMainMenu()//show menu of available commands in main menu
{
	std::cout<<"MAIN MENU\n";
	std::cout<<"Choose operation:\n";
	std::cout<<"Press 1 to insert data into tree\n";
	std::cout<<"Press 2 to delete data from tree\n";
	std::cout<<"Press 3 to search data in tree\n";
	std::cout<<"Press 4 to balance tree\n";
	std::cout<<"Press 5 to print tree\n";
	std::cout<<"Press 6 to clear tree\n";
	std::cout<<"Press 7 to perform unit testing\n";
	std::cout<<"Press 8 to change tree type\n";
	std::cout<<"Press 9 to get info about the programme\n";
	std::cout<<"Press 0 to exit";
}

void outputTreeTypeMenu(short treeType)//show menu of available commands in main menu
{
	std::cout<<"TREE TYPE CHOICE\n";
	std::cout<<"Choose tree type:\n";
	std::cout<<"Press 1 to select binary tree without self-balancing\n";
	std::cout<<"Press 2 to select AVL tree\n";
	std::cout<<"Press 3 to select Red-Black tree\n";
	std::cout<<"Press 4 to select splay tree\n";
	std::cout<<"Press 5 to select B-tree\n";
	std::cout<<"Press 6 to select R-tree\n";
	std::cout<<"Press 7 to select R*-tree\n";
	std::cout<<"Press 8 to select M-tree\n";
	std::cout<<"Press 9 to select AA-tree\n";
	std::cout<<"Press 0 to select 2-3 tree\n";
	std::cout<<"Current tree type is ";
	switch (treeType)//command choice
		{
			case 1://sort by code
				std::cout<<"binary tree without self-balancing"<<std::endl;
				break;
			case 2://sort by last name
				std::cout<<"AVL tree"<<std::endl;
				break;
			case 3://sort by first name
				std::cout<<"Red-black tree"<<std::endl;
				break;
			case 4://sort by middle name
				std::cout<<"splay tree"<<std::endl;
				break;
			case 5://sort by group
				std::cout<<"B-tree"<<std::endl;
				break;
			case 6://sort by grades
				std::cout<<"R-tree"<<std::endl;
				break;
			case 7://exit submenu
				std::cout<<"R*-tree"<<std::endl;
				break;
			case 8:
				std::cout<<"M-tree"<<std::endl;
				break;
			case 9:
				std::cout<<"AA-tree"<<std::endl;
				break;
			case 0:
				std::cout<<"2-3 tree"<<std::endl;
				break;
			default://case of pressing button not assigned to commands
				std::cout<<" UNKNOWN tree"<<std::endl;//show menu of available commands
		}
}

void aboutProgramme()//show info about the programme
{
	std::cout<<"ABOUT PROGRAMME\n\n";
	std::cout<<"Algorithms and data structures\n";
	std::cout<<"Practical assignment #2\n";
	std::cout<<"Binary tree without self-balancing\n\n";
	std::cout<<"Version 0.0.2.0 build 20180310200000\n\n";
	std::cout<<"Development and testing: Yaskovich Dmitry (ISBO-05-15)\n\n";
	std::cout<<"Dimini Inc, 2018";
}
