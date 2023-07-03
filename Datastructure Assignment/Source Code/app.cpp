#include	<iostream>
#include	<fstream>
#include	<cstdlib>
#include	<cstdio>
#include	<ctime>
#include    <Windows.h>
#include    <string>
#include	"BST.h"
#include    "Student.h"

using namespace std;

bool readFile(char *, BST *);
int menu();

int main() {
	int selection, order, source;
	bool loop;
	char file[20] = "student.txt";
	BST tree1;
	BST tree2;
	string original_tree, subtree;
	Student student_info;
	char thanks[34] = { 'T','H','A','N','K','S',' ','F','O','R',' ','U','S','I','N','G',' ','T','H','I','S',' ','P','R','O','G','R','A','M','.','.','.' }; //design
	do {
		system("cls");
		selection = menu();
		loop = false;
		if (selection == 1) {
			cout << "You are proceeding to read data to BST page.." << endl << endl;//proceed to read data to BST page
			readFile(file, &tree1);
			loop = true;
		}
		else if (selection == 2) {
			cout << "You are proceeding to print deepest nodes page.." << endl << endl; //proceed to print deepest nodes page	
			tree1.deepestNodes();
			loop = true;
		}
		else if (selection == 3) {
			cout << "You are proceeding to display student page.." << endl << endl;//proceed to display student page
			do {
				cout << "Please enter which place do you want to display the tree (1 - screen, 2 - file): ";
				cin >> source;
				if (source != 1 && source != 2) {
					cin.clear();
					cin.ignore(999, '\n');
					cout << "ERROR FOUND: Wrong input is inserted\n" << endl;
				}
			} while (source != 1 && source != 2); // input validation
			do {
				cout << "Please enter which order do you want to display the tree (1 - ascending, 2 - descending): ";
				cin >> order;
				if (order != 1 && order != 2) {
					cin.clear();
					cin.ignore(999, '\n');
					cout << "ERROR FOUND: Wrong input is inserted\n" << endl;
				}
			} while (order != 1 && order != 2); //input validation
			cout << "\n*****************************************************************" << endl;
			cout << "Results:" << endl;
			tree1.display(order, source);
			cout << "                            THE END" << endl;
			cout << "*****************************************************************" << endl;
			loop = true;
		}
		else if (selection == 4) {
			cout << "You are proceeding to clone subtree page.." << endl << endl; //proceed to clone subtree page
			if (!tree2.CloneSubtree(tree1, student_info)) {
			}
			else {
				cout << "*****************************************************************" << endl;
				cout << "Original tree: \n";
				tree1.preOrderPrint();
				cout << "*****************************************************************" << endl;
				cout << "Cloned subtree: \n";
				tree2.preOrderPrint();
				cout << "*****************************************************************" << endl;
				cout << "                            THE END" << endl;
				cout << "*****************************************************************" << endl;
			}
			loop = true;
		}
		else if (selection == 5) {
			cout << "You are proceeding to print ancestor page.." << endl << endl;//proceed to print ancestor page 	
			tree1.printAncestor(student_info);
			cout << "                            THE END" << endl;
			cout << "*****************************************************************" << endl;
			loop = true;
		}
		else if (selection == 6) {
			cout << "You are proceeding to print spiral page.." << endl << endl; //proceed to print spiral page 
			tree1.printSpiral();
			cout << "                            THE END" << endl;
			cout << "*****************************************************************" << endl;
			loop = true;
		}
		else if (selection == 7) {
			cout << endl;
			for (int i = 0; i < 34; i++) {
				cout << thanks[i]; // decoration when user want to quit
				Sleep(20);
			}
			Sleep(500);
			exit(0); // quit program
		}
		cin.clear();
		cin.ignore(999, '\n');
		system("pause");
	} while (loop == true);

	return 0;
}

//Question A
bool readFile(char *filename, BST *tree) {
	ifstream insertfile;
	char trash[2000];
	Student student, compare[50];
	int k = 0;
	bool repeat = false;

	insertfile.open(filename);
	if (insertfile.fail()) { //special case: file is unable to open
		cout << "The file cannot be opened.." << endl;
		return false;
	}
	else {
		while (!insertfile.eof()) {
			repeat = false;
			for (int i = 0; i < 2; i++) {
				insertfile >> trash; //put the unwanted element into variable
			}
			insertfile >> student.id;
			for (int i = 0; i < 2; i++) {
				insertfile >> trash;
			}
			insertfile.getline(student.name, 100);
			for (int i = 0; i < 2; i++) {
				insertfile >> trash;
			}
			insertfile.getline(student.address, 100);
			for (int i = 0; i < 2; i++) {
				insertfile >> trash;
			}
			insertfile.getline(student.DOB, 100);
			for (int i = 0; i < 2; i++) {
				insertfile >> trash;
			}
			insertfile.getline(student.phone_no, 100);
			for (int i = 0; i < 2; i++) {
				insertfile >> trash;
			}
			insertfile >> student.course;
			for (int i = 0; i < 2; i++) {
				insertfile >> trash;
			}
			insertfile >> student.cgpa;
			insertfile.ignore(100, '\n');

			compare[k] = student; // insert into array to compare for repeated case
			k++;

			for (int i = 0; i < tree->size(); i++) {
				if (student.compare2(compare[i])) {
					repeat = true;
					break; // break when repeated information is found
				}
			}
			if (repeat == false)
				tree->insert(student); // insert into tree
		}
		insertfile.close();
		cout << tree->countNode() << " student records had successfully read." << endl << endl;
	}
	return true;
}

int menu() {
	int selection;

	do { // menu page
		cout << "		*****************************************************" << endl;
		cout << "		*                 Choose Your Options:              *" << endl;
		cout << "		*****************************************************" << endl;
		cout << "		*            1. Read data to BST                    *" << endl;
		cout << "		*            2. Print deepest nodes                 *" << endl;
		cout << "		*            3. Display Student                     *" << endl;
		cout << "		*            4. Clone Subtree                       *" << endl;
		cout << "		*            5. Print Ancestor                      *" << endl;
		cout << "		*            6. Print Spiral                        *" << endl;
		cout << "		*            7. Exit                                *" << endl;
		cout << "		*****************************************************" << "\n\n\n" << endl;
		cout << "Please choose a selection..\nYour selection (1-7) --> ";
		cin >> selection;

		if (selection < 1 || selection>7 || !cin >> selection || !isspace(cin.peek()) || cin.fail()) {
			cout << "\nError Found... \nPlease retry..."; // error message
			Sleep(1000);
			cin.clear();
			cin.ignore(999, '\n');
			system("cls");
		}
	} while (selection < 1 || selection>7 || !cin >> selection || !isspace(cin.peek()) || cin.fail());

	return selection;
}