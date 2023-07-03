#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "BST.h"


using namespace std;


BST::BST() {
	root = NULL;
	count = 0;
}

bool BST::empty() {
	if (count == 0) return true;
	return false;
}

int BST::size() {
	return count;
}

void BST::preOrderPrint() {
	if (root == NULL) return;// handle special case
	else preOrderPrint2(root);// do normal process
	cout << endl;
}

void BST::preOrderPrint2(BTNode *cur) {
	if (cur == NULL) return;
	cur->item.print(cout);
	preOrderPrint2(cur->left);
	preOrderPrint2(cur->right);
}

void BST::inOrderPrint() {
	if (root == NULL) return;// handle special case
	else inOrderPrint2(root);// do normal process
}

void BST::inOrderPrint2(BTNode *cur) {
	if (cur == NULL) return;
	inOrderPrint2(cur->left);
	cur->item.print(cout);
	inOrderPrint2(cur->right);
}

void BST::postOrderPrint() {
	if (root == NULL) return;// handle special case
	else postOrderPrint2(root);// do normal process
}

void BST::postOrderPrint2(BTNode *cur) {
	if (cur == NULL) return;
	postOrderPrint2(cur->left);
	postOrderPrint2(cur->right);
	cur->item.print(cout);
}

int BST::countNode() {
	int	counter = 0;
	if (root == NULL) return 0;
	countNode2(root, counter);
	return counter;
}

void BST::countNode2(BTNode *cur, int &count) {
	if (cur == NULL) return;
	countNode2(cur->left, count);
	countNode2(cur->right, count);
	count++;
}

bool BST::findGrandsons(type grandFather) {
	if (root == NULL) return false;
	return (fGS2(grandFather, root));
}

bool BST::fGS2(type grandFather, BTNode *cur) {
	if (cur == NULL) return false;
	//if (cur->item == grandFather) {
	if (cur->item.compare2(grandFather)) {

		fGS3(cur, 0);// do another TT to find grandsons
		return true;
	}
	if (fGS2(grandFather, cur->left)) return true;
	return fGS2(grandFather, cur->right);
}

void BST::fGS3(BTNode *cur, int level) {
	if (cur == NULL) return;
	if (level == 2) {
		cur->item.print(cout);
		return;  // No need to search downward
	}
	fGS3(cur->left, level + 1);
	fGS3(cur->right, level + 1);
}

void BST::topDownLevelTraversal() {
	BTNode			*cur;
	Queue		    q;


	if (empty()) return; 	// special case
	q.enqueue(root);	// Step 1: enqueue the first node
	while (!q.empty()) { 	// Step 2: do 2 operations inside
		q.dequeue(cur);
		if (cur != NULL) {
			cur->item.print(cout);

			if (cur->left != NULL)
				q.enqueue(cur->left);

			if (cur->right != NULL)
				q.enqueue(cur->right);
		}
	}
}

//insert for BST
bool BST::insert(type newItem) {
	BTNode	*cur = new BTNode(newItem);
	if (!cur) return false;		// special case 1
	if (root == NULL) {
		root = cur;
		count++;
		return true; 			// special case 2
	}
	insert2(root, cur);			// normal
	count++;
	return true;
}

void BST::insert2(BTNode *cur, BTNode *newNode) {
	//if (cur->item > newNode->item) {
	if (cur->item.compare1(newNode->item)) {
		if (cur->left == NULL)
			cur->left = newNode;
		else
			insert2(cur->left, newNode);
	}
	else {
		if (cur->right == NULL)
			cur->right = newNode;
		else
			insert2(cur->right, newNode);
	}
}

bool BST::remove(type item) {
	if (root == NULL) return false; 		// special case 1: tree is empty
	return remove2(root, root, item); 		// normal case
}

bool BST::remove2(BTNode *pre, BTNode *cur, type item) {

	// Turn back when the search reaches the end of an external path
	if (cur == NULL) return false;

	// normal case: manage to find the item to be removed
	//if (cur->item == item) {
	if (cur->item.compare2(item)) {
		if (cur->left == NULL || cur->right == NULL)
			case2(pre, cur);	// case 2 and case 1: cur has less than 2 sons
		else
			case3(cur);		// case 3, cur has 2 sons
		count--;				// update the counter
		return true;
	}

	// Current node does NOT store the current item -> ask left sub-tree to check
	//if (cur->item > item)
	if (cur->item.compare1(item))
		return remove2(cur, cur->left, item);

	// Item is not in the left subtree, try the right sub-tree instead
	return remove2(cur, cur->right, item);
}

void BST::case2(BTNode *pre, BTNode *cur) {

	// special case: delete root node
	if (pre == cur) {
		if (cur->left != NULL)	// has left son?
			root = cur->left;
		else
			root = cur->right;

		free(cur);
		return;
	}

	if (pre->right == cur) {		// father is right son of grandfather? 
		if (cur->left == NULL)			// father has no left son?
			pre->right = cur->right;			// connect gfather/gson
		else
			pre->right = cur->left;
	}
	else {						// father is left son of grandfather?
		if (cur->left == NULL)			// father has no left son? 
			pre->left = cur->right;				// connect gfather/gson
		else
			pre->left = cur->left;
	}

	free(cur);					// remove item
}

void BST::case3(BTNode *cur) {
	BTNode		*is, *isFather;

	// get the IS and IS_parent of current node
	is = isFather = cur->right;
	while (is->left != NULL) {
		isFather = is;
		is = is->left;
	}

	// copy IS node into current node
	cur->item = is->item;

	// Point IS_Father (grandfather) to IS_Child (grandson)
	if (is == isFather)
		cur->right = is->right;		// case 1: There is no IS_Father    
	else
		isFather->left = is->right;	// case 2: There is IS_Father

	// remove IS Node
	free(is);
}



// Question B
int BST::get_height(BTNode *node) {

	// Base case
	if (node == NULL)
		return 0;

	// recursive case
	return max(get_height(node->left), get_height(node->right)) + 1;
}

void BST::deepest(BTNode *node, int level) {
	if (!node)
		return;
	if (level == 1) {
		node->item.print(cout);
		cout << "\n*****************************************************************" << endl;
	}
	else if (level > 1) { //go to the deepest node
		deepest(node->left, level - 1);
		deepest(node->right, level - 1);
	}
}

bool BST::deepestNodes() {

	if (empty()) { // when BST is empty
		cout << "*****************************************************************" << endl;
		cout << "Results:" << endl << endl;
		cout << "ERROR FOUND: The BST is empty.\n" << endl;
		cout << "*****************************************************************" << endl;
		cout << "                            THE END" << endl;
		cout << "*****************************************************************" << endl;
		return false;
	}

	int level = get_height(root); // get the height of the tree
	cout << "*****************************************************************" << endl;
	cout << "The deepest node is: " << endl;
	deepest(root, level);
	cout << "                            THE END" << endl;
	cout << "*****************************************************************" << endl;
	return true;
}

// Question C
bool BST::display(int order, int source) {
	ofstream writeFile;

	if (empty()) { //special case 1: BST tree is empty
		cout << "\nERROR FOUND: The BST is empty.\n" << endl;
		cout << "*****************************************************************" << endl;
		return false;
	}

	if (order == 1 && source == 1) { //print content of every node in BST tree in ascending order to the screen
		inOrderPrintInFile(cout);
	}
	else if (order == 2 && source == 1) { //print content of every node in BST tree in descending order to the screen
		inOrderPrintInFileDescendingOrder(cout);
	}
	else if (order == 1 && source == 2) { //print content of every node in BST tree in ascending order to the file
		writeFile.open("student-info.txt");
		if (writeFile.fail()) { //special case: the file is unable to open
			cout << "The file cannot be opened.." << endl;
			return false;
		}
		else {
			inOrderPrintInFile(writeFile);
			writeFile.close();
			cout << "\nThe file is written successfully...\n\n";
			cout << "*****************************************************************" << endl;
		}
	}
	else if (order == 2 && source == 2) { //print content of every node in BST tree in descending order to the file
		writeFile.open("student-info.txt");
		if (writeFile.fail()) { //special case: the file is unable to open
			cout << "The file cannot be opened.." << endl;
			return false;
		}
		else {
			inOrderPrintInFileDescendingOrder(writeFile);
			writeFile.close();
			cout << "\nThe file is written successfully...\n\n";
			cout << "*****************************************************************" << endl;
		}
	}
	return true;
}

void BST::inOrderPrintInFile(ostream &out) {
	if (root == NULL) return;// handle special case
	else inOrderPrintInFile2(root, out);// do normal process
}

void BST::inOrderPrintInFile2(BTNode *cur, ostream &out) {
	if (cur == NULL) return;
	inOrderPrintInFile2(cur->left, out);
	cur->item.print(out); //print current nodes
	out << "\n*****************************************************************" << endl;
	inOrderPrintInFile2(cur->right, out);
}

void BST::inOrderPrintInFileDescendingOrder(ostream &out) {
	if (root == NULL) return;// handle special case
	else inOrderPrintInFileDescendingOrder2(root, out);// do normal process
}

void BST::inOrderPrintInFileDescendingOrder2(BTNode *cur, ostream &out) {
	if (cur == NULL) return;
	inOrderPrintInFileDescendingOrder2(cur->right, out);
	cur->item.print(out); //print current nodes
	out << "\n*****************************************************************" << endl;
	inOrderPrintInFileDescendingOrder2(cur->left, out);
}

// Question D
bool BST::CloneSubtree(BST t1, type item) {

	bool found = false;

	root = NULL; //ensure cloned tree is an empty tree before cloning
	count = 0;

	if (t1.root == NULL) { //special case 1: original BST tree is empty
		cout << "*****************************************************************" << endl;
		cout << "Results:" << endl << endl;
		cout << "ERROR FOUND: The BST is empty.\n" << endl;
		cout << "*****************************************************************" << endl;
		cout << "                            THE END" << endl;
		cout << "*****************************************************************" << endl;
		return false;
	}

	cout << "Student ID : "; //ask user input student id
	cin >> item.id;

	findNode(t1, item, found);
	if (found == true) {
		return true;
	}
	else {
		cout << "*****************************************************************" << endl; //prompt student id is not match with every node inside the BST tree
		cout << "Results:" << endl << endl;
		cout << "ERROR FOUND: The student id is not exist...\n\n";
		cout << "*****************************************************************" << endl;
		cout << "                            THE END" << endl;
		cout << "*****************************************************************" << endl;
		return false;
	}

}

bool BST::findNode(BST t1, type item, bool &found) {
	if (t1.root == NULL) return false;// handle special case
	findNode2(t1.root, item, found); // do normal process
	cout << endl;
	if (found == true)
		return true;
	else {
		return false;
	}

}

void BST::findNode2(BTNode *cur, type item, bool &found) {
	if (cur == NULL) return;
	if (cur->item.id == item.id) { //when the current original BST tree node's student id is match with the prompt student id
		found = true;
		findNode3(cur);
		return;
	}
	findNode2(cur->left, item, found);
	findNode2(cur->right, item, found);

}

void BST::findNode3(BTNode *cur) {
	if (cur == NULL) return;
	insert(cur->item);
	findNode3(cur->left);
	findNode3(cur->right);
}

// Question E
bool BST::printAncestor(type item) {

	if (empty()) { // when the BST is empty
		cout << "*****************************************************************" << endl;
		cout << "Results:" << endl << endl;
		cout << "ERROR FOUND: The BST is empty.\n" << endl;
		cout << "*****************************************************************" << endl;
		return false;
	}

	cout << "Student ID : ";
	cin >> item.id; // user prompt ID
	cout << endl;
	cout << "*****************************************************************" << endl;
	cout << setfill(' ');
	cout << "*                      Node: " << setw(35) << left << item.id << "*" << endl;
	cout << "*****************************************************************" << endl;
	cout << "Results:" << endl;

	if (root->item.compare3(item.id)) { // when the root matched with the ID inserted
		cout << "              There is no ancestor for this item" << endl << endl;
		cout << "*****************************************************************" << endl;
	}

	else if (!printAncestor_2(item, root)) {
		cout << "             The student ID required is not found.." << endl << endl;
		cout << "*****************************************************************" << endl;
		return false;
	}
	return true;
}

bool BST::printAncestor_2(type item, BTNode *node) {
	if (node == nullptr) // if the node point to NULL
		return false;

	if (node->item.compare3(item.id)) // return true when the node inserted is found 
		return true;

	if (printAncestor_2(item, node->left) || printAncestor_2(item, node->right)) {
		node->item.print(cout); // cout the ancestor 
		cout << endl;
		cout << "*****************************************************************" << endl;
		return true;
	}
	return false; // when the node inserted is not found
}

// Question F
bool BST::printSpiral() {

	if (empty()) { // special case 1: BST tree is empty
		cout << "*****************************************************************" << endl;
		cout << "Results:" << endl << endl;
		cout << "ERROR FOUND: The BST is empty.\n" << endl;
		cout << "*****************************************************************" << endl;
		return false;
	}

	int level = 1; // start from level 1
	cout << "*****************************************************************" << endl;
	cout << "The nodes in a BST in spiral order is: \n";

	while (printLevelFromLeftToRight(root, level++) && printLevelFromRightToLeft(root, level++)); //normal case: run till the function traverse every node in BST tree
	return true;
}

bool BST::printLevelFromLeftToRight(BTNode* node, int level) {

	if (node == NULL) { //special case 1
		return false;
	}

	if (level == 1) { //normal case
		node->item.print(cout);
		cout << "\n*****************************************************************" << endl;
		return true;
	}

	//process left child first, then right child
	bool left = printLevelFromLeftToRight(node->left, level - 1);
	bool right = printLevelFromLeftToRight(node->right, level - 1);

	return left || right;

}

bool BST::printLevelFromRightToLeft(BTNode* node, int level) {
	if (node == NULL) { //special case 1
		return false;
	}

	if (level == 1) { //normal case
		node->item.print(cout);
		cout << "\n*****************************************************************" << endl;
		return true;
	}

	//process right child first, then left child
	bool right = printLevelFromRightToLeft(node->right, level - 1);
	bool left = printLevelFromRightToLeft(node->left, level - 1);

	return right || left;
}
