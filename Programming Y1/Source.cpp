#include <iostream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <fstream>
#include <sstream>
using namespace std;

int A1 = 0, A2 = 0, B1 = 0, B2 = 0; // check number of booking
string datelist[25] = { "01062020","02062020","03062020" ,"04062020" ,"05062020" ,"06062020" ,"07062020","08062020" ,"09062020" ,"10062020" ,"11062020" ,"12062020", "13062020","14062020","21092020" ,"22092020" ,"23092020" ,"24092020" ,"25092020" ,"26092020" ,"27092020" ,"28092020" ,"29092020" ,"30092020", "31092020" };
// Function Prototype
void logo();
void loading();
void newbooking();
void deletebooking(int& A1, int& A2, int& B1, int& B2);
void searchbooking();
void updatebooking(int& A1, int& A2, int& B1, int& B2);
void rules();
int menupage();
int select_info_to_update();
int day(int year, int month, int day);
void checkbooking(int d, int m, int y, string& realdate);
bool checkbookingoccupied(string realdate, int num, int slot, int& A1, int& A2, int& B1, int& B2, bool& firstbooking);
void INupdatebooking(string realdate, string zoom, string timeslot, string coursecode, int number);
void record_booking_to_be_changed(int choice, string& zoom_changed, string& timeslot_changed, string& course_changed, string& numstudent_changed, string& date_changed);

// Declare structure
struct bookingINFO {
	string name;
	string coursecode;
	int numberofstudent = 0;
};

struct delete_info {
	int date;
	int month;
};

int main() {
	int menu;
	int selection;
	char thanks[34] = { 'T','H','A','N','K','S',' ','F','O','R',' ','U','S','I','N','G',' ','T','H','I','S',' ','P','R','O','G','R','A','M','M','E','.','.','.' }; //design
	menu = menupage(); // print menu and let user choose
	cout << endl;

	if (menu == 1) {
		cout << "You are proceeding to new booking page.." << endl << endl;
		cout << "If you would like to proceed, please key in 1 to proceed or key in any other number to back... " << endl;
		cout << "Any number you like : ";
		cin >> selection;
		if (selection == 1)
			newbooking(); // go to new booking feature
		else
			main();
	}
	else if (menu == 2) {
		cout << "You are proceeding to delete booking page.." << endl << endl;
		cout << "If you would like to proceed, please key in 1 to proceed or key in any other number to back... " << endl;
		cout << "Any number you like : ";
		cin >> selection;
		if (selection == 1)
			deletebooking(A1, A2, B1, B2); // go to delete booking feature
		else
			main();
	}
	else if (menu == 3) {
		cout << "You are proceeding to search booking page.." << endl << endl;
		cout << "If you would like to proceed, please key in 1 to proceed or key in any other number to back... " << endl;
		cout << "Any number you like : ";
		cin >> selection;
		if (selection == 1)
			searchbooking(); // go to search booking feature
		else
			main();
	}
	else if (menu == 4) {
		cout << "You are proceeding to update booking page.." << endl << endl;
		cout << "If you would like to proceed, please key in 1 to proceed or key in any other number to back... " << endl;
		cout << "Any number you like : ";
		cin >> selection;
		if (selection == 1)
			updatebooking(A1, A2, B1, B2); // go to update booking feature
		else
			main();
	}
	else if (menu == 5) {
		cout << "You are proceeding to rules and regulations page.." << endl << endl;
		cout << "If you would like to proceed, please key in 1 to proceed or key in any other number to back... " << endl;
		cout << "Any number you like : ";
		cin >> selection;
		if (selection == 1)
			rules(); // go to rules preview page
		else
			main();
	}
	else {
		for (int i = 0; i < 34; i++) {
			cout << thanks[i]; // decoration when user want to quit
			Sleep(20);
		}
		Sleep(500);
		exit(0); // quit program
	}
}

int menupage() {
	int selection;
	do {
		system("cls");
		logo();
		cout << "				*****************************************" << endl;
		cout << "				*         Choose Your Options:          *" << endl;
		cout << "				*****************************************" << endl;
		cout << "				*       1. Add a new booking            *" << endl;
		cout << "				*       2. Delete a booking             *" << endl;
		cout << "				*       3. Search for a booking         *" << endl;
		cout << "				*       4. Update a booking             *" << endl;
		cout << "				*       5. Rules and regulations        *" << endl;
		cout << "				*       6. Exit                         *" << endl;
		cout << "				*****************************************" << "\n\n\n" << endl;
		cout << "Selection --> ";
		cin >> selection; // input selection to go to the page they want

		if (selection <= 0 || selection > 6 || !cin >> selection || !isspace(cin.peek())) { // loop when user key in wrong input
			cout << "Please retry..."; // error message
			Sleep(1000);
			cin.clear();
			cin.ignore(999, '\n');
		}
	} while (selection <= 0 || selection > 6 || !cin >> selection || !isspace(cin.peek()));

	return selection;
}

int day(int year, int month, int day)
{
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= month < 3;
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7; // equation to find out which day on the date
}

void newbooking() {
	string name_temp, course_code_temp, line, realdate, zoom, timeslot;
	int date, month, slotChosen, findoutday;
	const int year = 2020;
	bool checkbook, firstbooking = false, trying;
	bookingINFO user;
	ofstream inputdate;
	ifstream outputdate;
	ofstream appendbook;
	ifstream design;

	do {
		system("cls");
		logo();			// display logo
		trying = false;
		// ask user to key in date
		cout << "Please key in the date you want to book.." << endl;
		cout << "Kindly reminder : The booking is just available for June trimester starting from 15 June 2020 to 20 September 2020." << endl;
		cout << "Enter the day (01 - 31)		: ";
		cin >> date;
		while (date < 1 || date > 31) { // loop when user key in wrong date
			cout << "Please key in a correct date." << endl;
			cin.clear();
			cin.ignore(999, '\n');
			cout << "Enter the day (01 - 31)		: ";
			cin >> date;
			cout << endl;
		}
		cout << "Enter the month (06 - 09)	: ";
		cin >> month;
		while (month < 6 || month > 9) { // loop when user key in wrong date
			cout << "Please key in a correct month." << endl;
			cin.clear();
			cin.ignore(999, '\n');
			cout << "Enter the month (06 - 09)	: ";
			cin >> month;
			cout << endl;
		}
		cout << "Enter the year			: " << year << endl;
		findoutday = day(year, month, date); // find out which day on that date
		cout << endl << endl;
		cout << "                        *****************************" << endl;
		cout << "                        *   The day is ";
		switch (findoutday) { // display which day it is
		case 0: cout << "Sunday.      *\n                        *****************************\n\n";
			break;
		case 1: cout << "Monday.      *\n                        *****************************\n\n";
			break;
		case 2: cout << "Tuesday.     *\n                        *****************************\n\n";
			break;
		case 3: cout << "Wednesday.   *\n                        *****************************\n\n";
			break;
		case 4: cout << "Thursday.    *\n                        *****************************\n\n";
			break;
		case 5: cout << "Friday.      *\n                        *****************************\n\n";
			break;
		case 6: cout << "Saturday.    *\n                        *****************************\n\n";
			break;
		}
		// set the format of date become XXXXXXXX
		inputdate.open("checkdate.txt", ofstream::out | ofstream::trunc);
		inputdate << setfill('0') << setw(2) << date << setw(2) << month << year << endl;
		inputdate.close();
		outputdate.open("checkdate.txt");
		while (getline(outputdate, line))
			realdate = line;
		outputdate.close();
		for (int i = 0; i < 25; i++) {
			if (datelist[i] == realdate)
				trying = true;
		}
		if (trying == true) {
			cout << "The available date is starting from 15 June 2020 to 20 September 2020." << endl;
			system("pause"); // display wrong message
		}
		else if (findoutday == 0 || findoutday == 6) {
			cout << "Sorry, you can't teach during weekend.\nPlease have some rest." << endl;
			trying = true;	// display wrong message
			system("pause");
		}

	} while (findoutday == 0 || findoutday == 6 || trying == true); //loop when user key in wrong date

	checkbooking(date, month, year, realdate);
	system("cls");
	cin.ignore(999, '\n');
	logo();
	cout << "Please type in your details accordingly..\n\n"; // prompt to get users details.
	cout << "Name               (eg: Mr Lim)    : ";
	getline(cin, user.name);
	cout << "Code of course     (eg: UCCD1004 PROGRAMMING CONCEPTS AND PRACTICES)  : ";
	getline(cin, user.coursecode);
	cout << "Number of students (range : 1 - 500)       : ";
	cin >> user.numberofstudent;
	cout << endl;
	while ((cin.fail()) || (!isspace(cin.peek())) || (user.numberofstudent <= 0) || (user.numberofstudent > 500)) { // loop when key in a wrong datatype.
		cout << "ERROR:Please retry..." << endl;
		cin.clear();
		cin.ignore(100, '\n');
		cout << "Number of students (range : 1 - 500)       : ";
		cin >> user.numberofstudent;
		cout << endl;
	}
	do {
		cout << "Please choose your time slot.\n\n";
		cout << "*******************************" << endl;
		cout << "*           Time slot         *" << endl;
		cout << "*******************************" << endl;
		cout << "*                             *" << endl;
		cout << "*      1. 11 a.m - 1 p.m.     *" << endl;
		cout << "*      2. 2 p.m. - 4 p.m.     *" << endl;
		cout << "*                             *" << endl;
		cout << "*******************************" << endl;
		cout << "\nPick a time slot ---> ";
		cin >> slotChosen; // prompt to get time slot they want
		if (slotChosen != 1 && slotChosen != 2) { // display error when user key in wrong input
			cout << "\nERROR:Please retry..." << endl;
			Sleep(500);
			system("cls");
			logo();
			cin.clear();
			cin.ignore(999, '\n');
		}
	} while (slotChosen != 1 && slotChosen != 2); //loop when user key in wrong input

	// check if there is any booking is already booked this timeslot on this date
	checkbook = checkbookingoccupied(realdate, user.numberofstudent, slotChosen, A1, A2, B1, B2, firstbooking);

	if (checkbook == false) { // when user want to update current booking into old booking
		if (user.numberofstudent > 250 && slotChosen == 1) {
			zoom = "Zoom A";
			timeslot = "Slot 11am-1pm";
		}
		else if (user.numberofstudent > 250 && slotChosen == 2) {
			zoom = "Zoom A";
			timeslot = "Slot 2pm-4pm";
		}
		else if (user.numberofstudent <= 250 && slotChosen == 1) {
			zoom = "Zoom B";
			timeslot = "Slot 11am-1pm";
		}
		else if (user.numberofstudent <= 250 && slotChosen == 2) {
			zoom = "Zoom B";
			timeslot = "Slot 2pm-4pm";
		}
		// append updated booking into booking.txt
		INupdatebooking(realdate, zoom, timeslot, user.coursecode, user.numberofstudent);
	}
	// append new booking into booking.txt 
	else if (checkbook == true && firstbooking == true) {
		appendbook.open("booking.txt", ios::app);
		if (user.numberofstudent > 250 && slotChosen == 1) {
			zoom = "Zoom A";
			timeslot = "Slot 11am-1pm";
			appendbook << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent > 250 && slotChosen == 2) {
			zoom = "Zoom A";
			timeslot = "Slot 2pm-4pm";
			appendbook << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent <= 250 && slotChosen == 1) {
			zoom = "Zoom B";
			timeslot = "Slot 11am-1pm";
			appendbook << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent <= 250 && slotChosen == 2) {
			zoom = "Zoom B";
			timeslot = "Slot 2pm-4pm";
			appendbook << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		appendbook.close();
	}

	else if (checkbook == true && firstbooking == false) {
		appendbook.open("booking.txt", ios::app);
		if (user.numberofstudent > 250 && slotChosen == 1 && A1 == 0) {
			zoom = "Zoom A";
			timeslot = "Slot 11am-1pm";
			appendbook << endl << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent > 250 && slotChosen == 2 && A2 == 0) {
			zoom = "Zoom A";
			timeslot = "Slot 2pm-4pm";
			appendbook << endl << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent <= 250 && slotChosen == 1 && B1 == 0) {
			zoom = "Zoom B";
			timeslot = "Slot 11am-1pm";
			appendbook << endl << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent <= 250 && slotChosen == 2 && B2 == 0) {
			zoom = "Zoom B";
			timeslot = "Slot 2pm-4pm";
			appendbook << endl << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent <= 250 && slotChosen == 1 && B1 == 1 && A1 == 0) {
			zoom = "Zoom A";
			timeslot = "Slot 11am-1pm";
			appendbook << endl << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		else if (user.numberofstudent <= 250 && slotChosen == 2 && B2 == 1 && A2 == 0) {
			zoom = "Zoom A";
			timeslot = "Slot 2pm-4pm";
			appendbook << endl << zoom << endl << timeslot << endl << user.coursecode << endl << user.numberofstudent << endl << realdate;
		}
		appendbook.close();
	}
	system("CLS");
	cout << endl << endl;
	// display booking successfully created message
	design.open("yeah.txt");
	if (design.is_open()) {
		while (!design.eof()) {
			getline(design, line);
			cout << line << endl;
			Sleep(400);
		}
	}
	else
		cout << "The file cannot open.." << endl;
	design.close();
	cout << endl << endl;
	cout << "Congratulations " << user.coursecode << " have successfully booked this timeslot on " << realdate << endl;
	cout << "Back to Main Menu";
	for (int i = 0; i < 3; i++) {
		cout << '.';
		Sleep(500);
	}
	Sleep(2000);
	system("cls");
	main();
}

void INupdatebooking(string realdate, string zoom, string timeslot, string coursecode, int number) {
	ifstream readfile;
	ofstream inputfile;
	string ZOOM, slot, course, num, date;
	int a = 0;


	readfile.open("booking.txt");
	inputfile.open("temporarybooking.txt", ifstream::out, ifstream::trunc); // discard any things exist in the temporary file
	if (inputfile.is_open()) {
		if (readfile.is_open()) {
			while (!readfile.eof()) {
				getline(readfile, ZOOM);
				getline(readfile, slot);
				getline(readfile, course);
				getline(readfile, num);
				getline(readfile, date);

				if (realdate == date && zoom == ZOOM && timeslot == slot) // check if the information is same with the information inside the booking.txt
					continue; // if same, skip append into temporary file
				else { // append all other information into temporary file
					if (a == 0) {
						inputfile << ZOOM << endl << slot << endl << course << endl << num << endl << date;
						a++;
					}
					else
						inputfile << endl << ZOOM << endl << slot << endl << course << endl << num << endl << date;
				}
			}
		}
		else
			cout << "The file cannot open.." << endl;
		readfile.close();
	}
	else
		cout << "The file cannot open.." << endl;
	if (a == 0) {
		// append the information that want to update
		inputfile << zoom << endl << timeslot << endl << coursecode << endl << number << endl << realdate;
	}
	else
		inputfile << endl << zoom << endl << timeslot << endl << coursecode << endl << number << endl << realdate;
	inputfile.close();

	a = 0;

	inputfile.open("booking.txt", ifstream::out, ifstream::trunc);
	readfile.open("temporarybooking.txt");
	if (inputfile.is_open()) {
		if (readfile.is_open()) {
			while (!readfile.eof()) {
				getline(readfile, ZOOM);
				getline(readfile, slot);
				getline(readfile, course);
				getline(readfile, num);
				getline(readfile, date);

				if (a == 0) { // rewrite all information into booking.txt
					inputfile << ZOOM << endl << slot << endl << course << endl << num << endl << date;
					a++;
				}
				else
					inputfile << endl << ZOOM << endl << slot << endl << course << endl << num << endl << date;
			}
		}
		else
			cout << "The file cannot open.." << endl;
		readfile.close();
	}
	else
		cout << "The file cannot open.." << endl;
	inputfile.close();
}

bool checkbookingoccupied(string realdate, int num, int slot, int& A1, int& A2, int& B1, int& B2, bool& firstbooking) {
	ifstream checkfile;
	string zoom, course, date, timeslot, numstudent;
	int action;
	A1 = 0;
	B1 = 0;
	A2 = 0;
	B2 = 0;

	checkfile.open("booking.txt");
	if (checkfile.is_open()) {
		if (checkfile.peek() == ifstream::traits_type::eof()) { // when file is empty
			firstbooking = true;
			checkfile.close();
			return true;
		}
		while (!checkfile.eof()) { // count the number of booking
			getline(checkfile, zoom);
			getline(checkfile, timeslot);
			getline(checkfile, course);
			getline(checkfile, numstudent);
			getline(checkfile, date);

			if (realdate == date) {
				if (zoom == "Zoom B" && timeslot == "Slot 11am-1pm")
					B1++;
				if (zoom == "Zoom B" && timeslot == "Slot 2pm-4pm")
					B2++;
				if (zoom == "Zoom A" && timeslot == "Slot 11am-1pm")
					A1++;
				if (zoom == "Zoom A" && timeslot == "Slot 2pm-4pm")
					A2++;
			}
		}
	}
	else
		cout << "The file is unable to open.." << endl;
	checkfile.close();

	checkfile.open("booking.txt");
	if (checkfile.is_open()) {
		while (!checkfile.eof()) {
			getline(checkfile, zoom);
			getline(checkfile, timeslot);
			getline(checkfile, course);
			getline(checkfile, numstudent);
			getline(checkfile, date);

			if (realdate == date) { // if there is someone took this zoom account on this time slot
				if (zoom == "Zoom B" && timeslot == "Slot 11am-1pm") {
					if (num <= 250 && slot == 1 && A1 >= 1 && B1 >= 1) {
						cout << "Sorry, this timeslot is already occupied by " << course << endl; //display occupied by who
						do {
							cin.clear();
							cin.ignore(999, '\n');
							cout << "\nAction can be taken:\n1) Update this booking\n2) Remain old booking" << endl;
							cout << "Enter your action (1/2) -->  ";
							cin >> action;

							if (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))) {
								cout << "Sorry, please enter 1 or 2.." << endl;
							}
						} while (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))); // loop when user key in wrong input

						if (action == 1) {
							checkfile.close();
							return false;
						}
						else {
							cout << "Back to Main Menu";
							for (int i = 0; i < 3; i++) {
								cout << '.';
								Sleep(500);
							}
							checkfile.close();
							main();
						}
					}
				}
				else if (zoom == "Zoom B" && timeslot == "Slot 2pm-4pm") {
					if (num <= 250 && slot == 2 && A2 >= 1 && B2 >= 1) {
						cout << "Sorry, this timeslot is already occupied by " << course << endl; //display occupied by who
						do {
							cin.clear();
							cin.ignore(999, '\n');
							cout << "\nAction can be taken:\n1) Update this booking\n2) Remain old booking" << endl;
							cout << "Enter your action (1/2) -->  ";
							cin >> action;

							if (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))) {
								cout << "Sorry, please enter 1 or 2.." << endl;
							}
						} while (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))); // loop when user key in wrong input

						if (action == 1) {
							checkfile.close();
							return false;
						}
						else {
							cout << "Back to Main Menu";
							for (int i = 0; i < 3; i++) {
								cout << '.';
								Sleep(500);
							}
							checkfile.close();
							main();
						}
					}
				}
				else if (zoom == "Zoom A" && timeslot == "Slot 11am-1pm") {
					if (num > 250 && slot == 1) {
						cout << "Sorry, this timeslot is already occupied by " << course << endl; //display occupied by who
						do {
							cin.clear();
							cin.ignore(999, '\n');
							cout << "\nAction can be taken:\n1) Update this booking\n2) Remain old booking" << endl;
							cout << "Enter your action (1/2) -->  ";
							cin >> action;

							if (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))) {
								cout << "Sorry, please enter 1 or 2.." << endl;
							}
						} while (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))); // loop when user key in wrong input

						if (action == 1) {
							checkfile.close();
							return false;
						}
						else {
							cout << "Back to Main Menu";
							for (int i = 0; i < 3; i++) {
								cout << '.';
								Sleep(500);
							}
							checkfile.close();
							main();
						}
					}
				}
				else if (zoom == "Zoom A" && timeslot == "Slot 2pm-4pm") {
					if (num > 250 && slot == 2) {
						cout << "Sorry, this timeslot is already occupied by " << course << endl; //display occupied by who
						do {
							cin.clear();
							cin.ignore(999, '\n');
							cout << "\nAction can be taken:\n1) Update this booking\n2) Remain old booking" << endl;
							cout << "Enter your action (1/2) -->  ";
							cin >> action;

							if (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))) {
								cout << "Sorry, please enter 1 or 2.." << endl;
							}
						} while (action <= 0 || action >= 3 || (cin.fail()) || (!isspace(cin.peek()))); // loop when user key in wrong input

						if (action == 1) {
							checkfile.close();
							return false;
						}
						else {
							cout << "Back to Main Menu";
							for (int i = 0; i < 3; i++) {
								cout << '.';
								Sleep(500);
							}
							checkfile.close();
							main();
						}
					}
				}
			}
		}
	}
	else
		cout << "The file cannot be opened.." << endl;
	checkfile.close();
	return true;
}

void deletebooking(int& A1, int& A2, int& B1, int& B2) {
	int choice;
	int a;
	int c = 0, makesure = 0;
	ifstream myfile;
	ofstream changed_booking;
	ofstream temporaryfile;
	ifstream temporaryfile1;
	string zoom, course, date, timeslot, numstudent;
	string zoom_delete, course_delete, date_delete, timeslot_delete, numstudent_delete;

	do {
		a = 1;
		system("cls");
		logo();
		cout << "The following are the details of the booked zoom accounts starting from 15 June 2020 to 20 September 2020:" << endl << endl;
		cout << "**************************************************************************************************************************************" << endl;
		cout << "| No  |   Zoom Room  |     Time slot      |                    Course Name                    |    Number of Student   |     Date    |" << endl;
		cout << "**************************************************************************************************************************************" << endl;
		myfile.open("booking.txt");
		if (myfile.is_open()) {
			if (myfile.peek() == ifstream::traits_type::eof()) { // if file is empty, display no booking
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                 There is no any booking currently..                                                *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "**************************************************************************************************************************************" << endl;
				system("pause");
				cout << "Back to Main Menu";
				for (int i = 0; i < 3; i++) {
					cout << '.';
					Sleep(500);
				}
				system("cls");
				myfile.close();
				main(); // back to menu 
			}
			while (!myfile.eof()) { // else display every booking
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				cout << "| " << a << ")  |";
				cout << "   " << zoom << "     |" << "   " << setw(13) << left << timeslot << "    |   " << setw(47) << left << course << " |           " << setw(3) << left << numstudent << "          |" << setw(3) << right << "" << date << "  |" << endl;
				cout << "**************************************************************************************************************************************" << endl;
				a += 1;
			}
		}
		else
			cout << "The file is unable to open.." << endl;
		myfile.close();

		cout << endl;
		cout << "Please select which booked zoom account do you wish to delete!" << endl;
		cout << "Selection (Choose only number shown above or insert 0 to back to main menu): ";
		cin >> choice;

		while (choice < 0 || choice >= a || (cin.fail())) { // loop when user key in invalid number
			cout << "Please key in according to the instructions given." << endl;
			cin.clear();
			cin.ignore(999, '\n');
			cout << "Selection (Choose only number shown above or insert 0 to back to main menu): ";
			cin >> choice;
			cout << endl;
		}

		if (choice == 0) {
			cout << "Back to Main Menu";
			for (int i = 0; i < 3; i++) {
				cout << '.';
				Sleep(500);
			}
			main(); // back to menu
		}
		// record the booking that want to delete
		record_booking_to_be_changed(choice, zoom_delete, timeslot_delete, course_delete, numstudent_delete, date_delete);
		do {
			system("CLS");
			logo();
			cout << endl;
			cout << "The booking selected is " << endl << endl;
			cout << "***************************************************************************" << endl;
			cout << "*  Zoom Account      :  " << setw(50) << left << zoom_delete << "*" << endl;
			cout << "*  Time slot         :  " << setw(50) << left << timeslot_delete << "*" << endl;
			cout << "*  Course Code       :  " << setw(50) << left << course_delete << "*" << endl;
			cout << "*  Number of student :  " << setw(50) << left << numstudent_delete << "*" << endl;
			cout << "*  Date              :  " << setw(50) << left << date_delete << "*" << endl;
			cout << "***************************************************************************" << endl << endl;
			cout << "Are you sure that you want to delete this booking? (1 - YES / 2 - NO)" << endl;
			cout << "Selection --> ";
			cin >> makesure; // to confirm user want to delete this booking

			if (makesure != 1 && makesure != 2) {
				cout << "Please only insert 1 or 2.." << endl;
				Sleep(1000);
				cin.clear();
				cin.ignore(999, '\n');
			}

		} while (makesure != 1 && makesure != 2); // loop when user key in wrong value

	} while (makesure == 2); // loop when user don't want to delete this booking
	myfile.open("booking.txt");
	if (myfile.is_open()) {
		while (!myfile.eof()) {
			getline(myfile, zoom);
			getline(myfile, timeslot);
			getline(myfile, course);
			getline(myfile, numstudent);
			getline(myfile, date);

			if (date_delete == date) { // let the number of zoom account become 0 
				if (zoom_delete == "Zoom A" && timeslot_delete == "Slot 11am-1pm") {
					A1 = 0;
				}
				if (zoom_delete == "Zoom A" && timeslot_delete == "Slot 2pm-4pm") {
					A2 = 0;
				}
				if (zoom_delete == "Zoom B" && timeslot_delete == "Slot 11am-1pm") {
					B1 = 0;
				}
				if (zoom_delete == "Zoom B" && timeslot_delete == "Slot 2pm-4pm") {
					B2 = 0;
				}
			}
		}
	}
	else {
		cout << "The file is unable to open.." << endl;
	}
	myfile.close();

	temporaryfile.open("temporarybooking.txt", ofstream::out | ofstream::trunc); // discard everything in temporary file
	if (temporaryfile.is_open()) {
		myfile.open("booking.txt");
		if (myfile.is_open()) {
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (c == 0) { // append all info in booking.txt to temporary file
					temporaryfile << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;
					c++;
				}
				else
					temporaryfile << endl << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;
			}
		}
		else
			cout << "The file is unable to open.." << endl;
		myfile.close();
	}
	else
		cout << "The file is unable to open.." << endl;
	temporaryfile.close();
	c = 0;
	changed_booking.open("booking.txt", ofstream::out | ofstream::trunc); // discard everything in booking.txt
	if (changed_booking.is_open()) {
		temporaryfile1.open("temporarybooking.txt");
		if (temporaryfile1.is_open()) {
			while (!temporaryfile1.eof()) {
				getline(temporaryfile1, zoom);
				getline(temporaryfile1, timeslot);
				getline(temporaryfile1, course);
				getline(temporaryfile1, numstudent);
				getline(temporaryfile1, date);

				if (zoom_delete == zoom && timeslot_delete == timeslot && course_delete == course && numstudent_delete == numstudent && date_delete == date)
					continue; // skip append if the booking in temporary file is same with the booking that want to delete

				else {
					if (c == 0) { // append all other booking into booking.txt
						changed_booking << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;
						c++;
					}
					else
						changed_booking << endl << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;
				}
			}
		}
		else
			cout << "The file is unable to open.." << endl;
		temporaryfile1.close();
	}
	else
		cout << "The file is unable to open.." << endl;
	changed_booking.close();
	// display booking successfully deleted message
	cout << "The booked zoom account of the course " << course_delete << " on " << date_delete << " has succesfully deleted...";
	Sleep(800);
	system("cls");
	main();
}

void searchbooking() {
	int a;
	int selection, carryon, selecttimeslot, d, m, y = 2020, findoutday;
	string searchwhat, line;
	string zoom, slot, course, num, date;
	bool search = false, trying = false;
	ifstream myfile;
	ofstream inputfile;

	myfile.open("booking.txt");
	if (myfile.is_open()) {
		if (myfile.peek() == ifstream::traits_type::eof()) { // if booking.txt is empty, it shows no booking 
			system("cls");
			logo();
			cout << "There is no any booking currently..." << endl;
			system("pause");
			cout << "Back to Main Menu";
			for (int i = 0; i < 3; i++) {
				cout << '.';
				Sleep(500);
			}
			myfile.close();
			system("cls");
			main(); // back to menu
		}
	}
	else
		cout << "The file is unable to open.." << endl;
	myfile.close();

	do {
		system("CLS");
		logo();
		do {
			cout << "				*****************************************" << endl;
			cout << "				*         Choose Your Options:          *" << endl;
			cout << "				*****************************************" << endl;
			cout << "				*          1. Search Date               *" << endl;
			cout << "				*          2. Search Time Slot          *" << endl;
			cout << "				*          3. Search Course Name        *" << endl;
			cout << "				*          4. Back                      *" << endl;
			cout << "				*****************************************" << "\n\n\n" << endl;
			cout << "Selection --> ";
			cin >> selection; // prompt to get what user want to search

			if (selection > 4 || selection < 1) { // display error message when user key in wrong selection
				cout << "Please key in number 1 - 4" << endl;
				Sleep(500);
				cin.clear();
				cin.ignore(999, '\n');
				system("cls");
				logo();
			}
		} while (selection > 4 || selection < 1);// loop when user key in wrong selection
		system("cls");
		logo();

		if (selection == 1) { // search date
			a = 1;
			system("cls");
			logo();			// display logo
			do {
				search = false;
				trying = false;
				cin.clear();
				cin.ignore(999, '\n');
				cout << "Please key in the date you want to search.." << endl; // key in date that user want to search
				cout << "Kindly reminder : The booking is just available for June trimester starting from 15 June 2020 to 20 September 2020." << endl;
				cout << "Enter the day (01 - 31)		: ";
				cin >> d;
				while (d < 1 || d > 31) { // loop when user key in wrong number
					cout << "Please key in a correct date." << endl;
					cin.clear();
					cin.ignore(999, '\n');
					cout << "Enter the day (01 - 31)		: ";
					cin >> d;
					cout << endl;
				}
				cout << "Enter the month (06 - 09)	: ";
				cin >> m;
				while (m < 6 || m > 9) { // loop when user key in wrong number
					cout << "Please key in a correct month." << endl;
					cin.clear();
					cin.ignore(999, '\n');
					cout << "Enter the month (06 - 09)	: ";
					cin >> m;
					cout << endl;
				}
				cout << "Enter the year			: " << y << endl;
				findoutday = day(y, m, d);
				cout << endl << endl;
				cout << "                        *****************************" << endl;
				cout << "                        *   The day is ";
				switch (findoutday) { // display which day it is 
				case 0: cout << "Sunday.      *\n                        *****************************\n\n";
					break;
				case 1: cout << "Monday.      *\n                        *****************************\n\n";
					break;
				case 2: cout << "Tuesday.     *\n                        *****************************\n\n";
					break;
				case 3: cout << "Wednesday.   *\n                        *****************************\n\n";
					break;
				case 4: cout << "Thursday.    *\n                        *****************************\n\n";
					break;
				case 5: cout << "Friday.      *\n                        *****************************\n\n";
					break;
				case 6: cout << "Saturday.    *\n                        *****************************\n\n";
					break;
				}
				// set the date become format of XXXXXXXX
				inputfile.open("checkdate.txt", ofstream::out | ofstream::trunc);
				inputfile << setfill('0') << setw(2) << d << setw(2) << m << y << endl;
				inputfile.close();
				myfile.open("checkdate.txt");
				while (getline(myfile, line))
					searchwhat = line;
				myfile.close();
				for (int i = 0; i < 25; i++) {
					if (datelist[i] == searchwhat)
						trying = true;
				}
				if (trying == true) { // display error message
					cout << "The available date is starting from 15 June 2020 to 20 September 2020." << endl;
					system("pause");
					system("cls");
					logo();
				}
				else if (findoutday == 0 || findoutday == 6) { // display error message
					cout << "Sorry, the booking is always empty during weekend.\nPlease try another date." << endl;
					trying = true;
					system("pause");
					system("cls");
					logo();
				}
			} while (trying == true || findoutday == 0 || findoutday == 6); // loop when user key in wrong date
			cout << "The results on date " << searchwhat << " are : \n";
			cout << "**************************************************************************************************************************************" << endl;
			cout << "| No  |   Zoom Room  |     Time slot      |                    Course Name                    |    Number of Student   |     Date    |" << endl;
			cout << "**************************************************************************************************************************************" << endl;
			myfile.open("booking.txt");
			if (myfile.is_open()) {
				while (!myfile.eof()) {
					getline(myfile, zoom);
					getline(myfile, slot);
					getline(myfile, course);
					getline(myfile, num);
					getline(myfile, date);

					if (searchwhat == date) { // display every booking is matched with the date
						cout << "| " << a << ")  |";
						cout << "   " << zoom << "     |" << "   " << setw(13) << left << slot << "    |   " << setw(47) << left << course << " |           " << setw(3) << left << num << "          |" << setw(3) << right << "" << date << "  |" << endl;
						cout << "**************************************************************************************************************************************" << endl;
						a += 1;
						search = true;
					}
				}
				if (search == false) { // if no booking is matched, display no booking
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                   There is no any booking for this date currently..                                                *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "**************************************************************************************************************************************" << endl;
				}
			}
			else
				cout << "The file is unable to open.." << endl;
			myfile.close();
		}
		else if (selection == 2) { // search time slot
			cin.ignore(999, '\n');
			a = 1;
			search = false;
			cout << "Show All Time Slot : " << endl;;
			cout << "1) 11.a.m - 1.p.m\n2) 2.pm - 4.pm\n\n" << endl;
			cout << "Choose the time slot you want to show --> ";
			cin >> selecttimeslot; // prompt to get what time slot want to search
			while (selecttimeslot != 1 && selecttimeslot != 2) { //loop when user key in wrong number 
				cout << "Please enter 1 or 2.." << endl;
				Sleep(700);
				cin.clear();
				cin.ignore(999, '\n');
				system("CLS");
				logo();
				cout << "Show All Time Slot : " << endl;;
				cout << "1) 11.a.m - 1.p.m\n2) 2.pm - 4.pm\n\n" << endl;
				cout << "Choose the time slot you want to show --> ";
				cin >> selecttimeslot;
			}
			if (selecttimeslot == 1)
				searchwhat = "Slot 11am-1pm";
			else if (selecttimeslot == 2)
				searchwhat = "Slot 2pm-4pm";
			cout << "The results of Time " << searchwhat << " are : \n";
			cout << "**************************************************************************************************************************************" << endl;
			cout << "| No  |   Zoom Room  |     Time slot      |                    Course Name                    |    Number of Student   |     Date    |" << endl;
			cout << "**************************************************************************************************************************************" << endl;
			myfile.open("booking.txt");
			if (myfile.is_open()) {
				while (!myfile.eof()) {
					getline(myfile, zoom);
					getline(myfile, slot);
					getline(myfile, course);
					getline(myfile, num);
					getline(myfile, date);

					if (searchwhat == slot) {// display every booking is matched with the date
						cout << "| " << a << ")  |";
						cout << "   " << zoom << "     |" << "   " << setw(13) << left << slot << "    |   " << setw(47) << left << course << " |           " << setw(3) << left << num << "          |" << setw(3) << right << "" << date << "  |" << endl;
						cout << "**************************************************************************************************************************************" << endl;
						a += 1;
						search = true;
					}
				}
				if (search == false) {// if no booking is matched, display no booking
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                   There is no any booking for timeslot currently..                                                 *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "**************************************************************************************************************************************" << endl;
				}
			}
			else
				cout << "The file is unable to open.." << endl;
			myfile.close();
		}
		else if (selection == 3) { // search course name
			cin.ignore(999, '\n');
			a = 1;
			search = false;
			cout << "Please make sure you key in full course name.\n" << endl;
			cout << "Search Course Name (UCCD1004 Programming Concepts and Practices) : ";
			getline(cin, searchwhat);
			cout << "The results of " << searchwhat << " are : \n";
			cout << "**************************************************************************************************************************************" << endl;
			cout << "| No  |   Zoom Room  |     Time slot      |                    Course Name                    |    Number of Student   |     Date    |" << endl;
			cout << "**************************************************************************************************************************************" << endl;
			myfile.open("booking.txt");
			if (myfile.is_open()) {
				while (!myfile.eof()) {
					getline(myfile, zoom);
					getline(myfile, slot);
					getline(myfile, course);
					getline(myfile, num);
					getline(myfile, date);

					if (searchwhat == course) {// display every booking is matched with the date
						cout << "| " << a << ")  |";
						cout << "   " << zoom << "     |" << "   " << setw(13) << left << slot << "    |   " << setw(47) << left << course << " |           " << setw(3) << left << num << "          |" << setw(3) << right << "" << date << "  |" << endl;
						cout << "**************************************************************************************************************************************" << endl;
						a += 1;
						search = true;
					}
				}
				if (search == false) {// if no booking is matched, display no booking
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                   There is no any booking for course currently..                                                   *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "*                                                                                                                                    *" << endl;
					cout << "**************************************************************************************************************************************" << endl;
				}
			}
			else
				cout << "The file is unable to open.." << endl;
			myfile.close();
		}
		else if (selection == 4) { // back to main menu
			cout << "Back to Main Menu";
			for (int i = 0; i < 3; i++) {
				cout << '.';
				Sleep(500);
			}
			main();
		}

		cout << "\nIf you would like to back to main menu, please key in 1";
		cout << "\nIf you would like to continue searching, please key in 2";
		cout << "\nSelection: ";
		cin >> carryon; // prompt to know whether user want to continue searching

		while (carryon <= 0 || carryon >= 3) { // loop when user key in wrong number
			cout << "Please only key in 1 or 2..." << endl << endl;
			cin.clear();
			cin.ignore(999, '\n');
			cout << "Selection: ";
			cin >> carryon;
			cout << endl;
		}

		if (carryon == 1) { // back to main menu
			cout << "Back to Main Menu";
			for (int i = 0; i < 3; i++) {
				cout << '.';
				Sleep(500);
			}
			main();
		}
	} while (carryon == 2); // loop when user want to continue searching

}

void updatebooking(int& A1, int& A2, int& B1, int& B2) {
	int a = 1;
	int b = 0;
	int y = 2020;
	int selection, choice, makesure;
	int findoutday;
	ifstream myfile;
	ofstream outputfile;
	ifstream outputdate;
	ofstream inputdate;
	string zoom, course, date, timeslot, numstudent;
	string zoom_update, course_update, date_update, timeslot_update, numstudent_update;
	string line;
	int updated_studentnumber;
	delete_info user;
	bool trying;

	do {
		a = 1;
		system("cls");
		logo();
		cout << "The following are the details of the booked zoom accounts starting from 15 June 2020 to 20 September 2020:" << endl << endl;
		cout << "**************************************************************************************************************************************" << endl;
		cout << "| No  |   Zoom Room  |     Time slot      |                    Course Name                    |    Number of Student   |     Date    |" << endl;
		cout << "**************************************************************************************************************************************" << endl;
		myfile.open("booking.txt");
		if (myfile.is_open()) { // if booking.txt is empty, display no booking 
			if (myfile.peek() == ifstream::traits_type::eof()) {
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                              There is no any booking currently..                                                   *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "*                                                                                                                                    *" << endl;
				cout << "**************************************************************************************************************************************" << endl;
				system("pause");
				cout << "Back to Main Menu";
				for (int i = 0; i < 3; i++) {
					cout << '.';
					Sleep(500);
				}
				system("cls");
				myfile.close();
				main(); // back to main menu
			}
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				cout << "| " << a << ")  |"; // display every occupied time slot
				cout << "   " << zoom << "     |" << "   " << setw(13) << left << timeslot << "    |   " << setw(47) << left << course << " |           " << setw(3) << left << numstudent << "          |" << setw(3) << right << "" << date << "  |" << endl;
				cout << "**************************************************************************************************************************************" << endl;
				a += 1;
			}
		}
		else
			cout << "The file is unable to open.." << endl;
		myfile.close();

		cout << endl;
		cout << "Please select which booked zoom account do you want to update!!!" << endl;
		cout << "Selection (Choose only number shown above or insert 0 to back to main menu): ";
		cin >> selection; // choose which booking want to update

		while (selection < 0 || selection >= a || (cin.fail())) { // loop when user key in wrong number 
			cout << "Please key in according to the instructions given." << endl;
			cin.clear();
			cin.ignore(999, '\n');
			cout << "Selection (Choose only number shown above or insert 0 to back to main menu): ";
			cin >> selection;
			cout << endl;
		}

		if (selection == 0) {
			cout << "Back to Main Menu";
			for (int i = 0; i < 3; i++) {
				cout << '.';
				Sleep(500);
			}
			main();
		}
		// record which booking that want to update
		record_booking_to_be_changed(selection, zoom_update, timeslot_update, course_update, numstudent_update, date_update);
		do {
			system("cls");
			logo();
			cout << endl;
			cout << "The booking selected is " << endl << endl;
			cout << "***************************************************************************" << endl;
			cout << "*  Zoom Account      :  " << setw(50) << left << zoom_update << "*" << endl;
			cout << "*  Time slot         :  " << setw(50) << left << timeslot_update << "*" << endl;
			cout << "*  Course Code       :  " << setw(50) << left << course_update << "*" << endl;
			cout << "*  Number of student :  " << setw(50) << left << numstudent_update << "*" << endl;
			cout << "*  Date              :  " << setw(50) << left << date_update << "*" << endl;
			cout << "***************************************************************************" << endl << endl;
			cout << "Are you sure that you want to update this booking? (1 - YES / 2 - NO)" << endl;
			cout << "Selection --> ";
			cin >> makesure; // ask user whether want to update 
			if (makesure != 1 && makesure != 2) { // display error message when key in wrong number
				cout << "Please only insert 1 or 2.." << endl;
				Sleep(1000);
				cin.clear();
				cin.ignore(999, '\n');
			}
		} while (makesure != 1 && makesure != 2); // loop when user key in wrong number

	} while (makesure == 2); // loop when user don't want to update this booking

	myfile.open("booking.txt");
	outputfile.open("temporarybooking.txt", ifstream::out, ifstream::trunc);
	if (outputfile.is_open()) {
		if (myfile.is_open()) {
			if (myfile.peek() == ifstream::traits_type::eof()) {
				b = 0;
			}
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date == date_update && zoom == zoom_update && timeslot == timeslot_update)
					continue; // skip append into temporary file if the update info is same with info in booking.txt
				else {
					if (b == 0) { // else append every other booking into temporary file
						outputfile << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;
						b++;
					}
					else
						outputfile << endl << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;
				}
			}
		}
		else
			cout << "The file cannot open.." << endl;
		myfile.close();
	}
	else
		cout << "The file cannot open.." << endl;
	outputfile.close();

	system("cls");
	choice = select_info_to_update(); // prompt to get what user want to update
	system("cls");
	loading();
	logo();
	istringstream(numstudent_update) >> updated_studentnumber;
	cout << endl;

	if (choice == 1) { // update zoom account
		if (zoom_update == "Zoom A" && timeslot_update == "Slot 11am-1pm") {
			if (updated_studentnumber > 250) {// zoom A with more than 250 students are not allow to change to zoom B
				cout << "Sorry, you are not allowed to change to Zoom B since student number more than 250..." << endl << endl;
				system("pause");
				cout << "Back to Main Menu";
				for (int i = 0; i < 3; i++) {
					cout << '.';
					Sleep(500);
				}
				system("cls");
				main();
			}
			else {
				myfile.open("booking.txt");
				while (!myfile.eof()) {
					getline(myfile, zoom);
					getline(myfile, timeslot);
					getline(myfile, course);
					getline(myfile, numstudent);
					getline(myfile, date);

					if (date_update == date && zoom == "Zoom B" && timeslot == "Slot 11am-1pm") { // if zoom B is occupied
						cout << "Sorry, you are not allowed to change to Zoom B since Zoom B is occupied..." << endl << endl;
						system("pause");
						cout << "Back to Main Menu";
						for (int i = 0; i < 3; i++) {
							cout << '.';
							Sleep(500);
						}
						system("cls");
						myfile.close();
						main(); // back to main menu
					}
				}
				myfile.close();
				zoom_update = "Zoom B";
				A1 = 0;
				B1 = 1;
			}
		}
		else if (zoom_update == "Zoom A" && timeslot_update == "Slot 2pm-4pm") {
			if (updated_studentnumber > 250) {// zoom A with more than 250 students are not allow to change to zoom B
				cout << "Sorry, you are not allowed to change to Zoom B since student number more than 250..." << endl << endl;
				system("pause");
				cout << "Back to Main Menu";
				for (int i = 0; i < 3; i++) {
					cout << '.';
					Sleep(500);
				}
				system("cls");
				main();
			}
			else {
				myfile.open("booking.txt");
				while (!myfile.eof()) {
					getline(myfile, zoom);
					getline(myfile, timeslot);
					getline(myfile, course);
					getline(myfile, numstudent);
					getline(myfile, date);

					if (date_update == date && zoom == "Zoom B" && timeslot == "Slot 2pm-4pm") { // if zoom B is occupied
						cout << "Sorry, you are not allowed to change to Zoom B since Zoom B is occupied..." << endl << endl;
						system("pause");
						cout << "Back to Main Menu";
						for (int i = 0; i < 3; i++) {
							cout << '.';
							Sleep(500);
						}
						system("cls");
						myfile.close();
						main();
					}
				}
				myfile.close();
				zoom_update = "Zoom B";
				A2 = 0;
				B2 = 1;
			}
		}
		else if (zoom_update == "Zoom B" && timeslot_update == "Slot 11am-1pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom A" && timeslot == "Slot 11am-1pm") { // if zoom A is occupied
					cout << "Sorry, you are not allowed to change to Zoom A since Zoom A is occupied..." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
			zoom_update = "Zoom A";
			B1 = 0;
			A1 = 1;
		}
		else if (zoom_update == "Zoom B" && timeslot_update == "Slot 2pm-4pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom A" && timeslot == "Slot 2pm-4pm") {// if zoom A is occupied
					cout << "Sorry, you are not allowed to change to Zoom A since Zoom A is occupied..." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
			zoom_update = "Zoom A";
			B2 = 0;
			A2 = 1;
		}
	}
	else if (choice == 2) { //update time slot
		if (zoom_update == "Zoom A" && timeslot_update == "Slot 11am-1pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom A" && timeslot == "Slot 2pm-4pm") { // if slot 2pm-4pm is occupied
					cout << "Sorry, you are not allowed to change to timeslot 2pm-4pm since timeslot 2pm-4pm is occupied..." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
			timeslot_update = "Slot 2pm-4pm";
			A1 = 0;
			A2 = 1;
		}
		else if (zoom_update == "Zoom A" && timeslot_update == "Slot 2pm-4pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom A" && timeslot == "Slot 11am-1pm") { // if slot 2pm-4pm is occupied
					cout << "Sorry, you are not allowed to change to timeslot 11am-1pm since timeslot 11am-1pm is occupied..." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
			timeslot_update = "Slot 11am-1pm";
			A2 = 0;
			A1 = 1;
		}
		else if (zoom_update == "Zoom B" && timeslot_update == "Slot 11am-1pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom B" && timeslot == "Slot 2pm-4pm") { // if slot 11am-1pm is occupied
					cout << "Sorry, you are not allowed to change to timeslot 2pm-4pm since timeslot 2pm-4pm is occupied..." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
			timeslot_update = "Slot 2pm-4pm";
			B1 = 0;
			B2 = 1;
		}
		else if (zoom_update == "Zoom B" && timeslot_update == "Slot 2pm-4pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom B" && timeslot == "Slot 11am-1pm") {// if slot 11am-1pm is occupied
					cout << "Sorry, you are not allowed to change to timeslot 11am-1pm since timeslot 11am-1pm is occupied..." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
			timeslot_update = "Slot 11am-1pm";
			B2 = 0;
			B1 = 1;
		}
	}
	else if (choice == 3) { // update course name
		cout << "Please insert a new course name (e.g. UCCD1004 Programming Concepts and Practices): ";
		cin.clear();
		cin.ignore(999, '\n');
		getline(cin, course_update); // prompt to get course name that want to update
	}
	else if (choice == 4) { // update number of students
		cout << "Please insert the new student number (e.g. 100): ";
		if (zoom_update == "Zoom B") {
			cin >> numstudent_update;
			istringstream(numstudent_update) >> updated_studentnumber;
			while (updated_studentnumber > 250 || updated_studentnumber <= 0) { // loop when user key in wrong number of student
				cout << "Since Zoom B only allow students with maximum number of 250 students, you cannot change more than 250 students..." << endl;
				cin.clear();
				cin.ignore(999, '\n');
				cout << "Please insert the new student number (e.g. 100): ";
				cin >> numstudent_update;
				istringstream(numstudent_update) >> updated_studentnumber;
			}
		}
		else if (zoom_update == "Zoom A") {
			cin >> numstudent_update;
			istringstream(numstudent_update) >> updated_studentnumber;
			while (updated_studentnumber > 500 || updated_studentnumber <= 0) { // loop when user key in wrong number of student
				cout << "Please insert maximum only 500 students!!!" << endl;
				cin.clear();
				cin.ignore(999, '\n');
				cout << "Please insert the new student number (e.g. 100): ";
				cin >> numstudent_update;
				istringstream(numstudent_update) >> updated_studentnumber;
			}
		}
	}
	else if (choice == 5) { // search date
		do {
			trying = false;
			cout << "Please key in the date you want to update.." << endl;
			cout << "Kindly reminder : The booking is just available for June trimester starting from 15 June 2020 to 20 September 2020." << endl;
			cout << "Enter the day (01 - 31)		: ";
			cin >> user.date;
			while (user.date < 1 || user.date > 31) { // loop when user key in wrong day
				cout << "Please key in a correct date." << endl;
				cin.clear();
				cin.ignore(999, '\n');
				cout << "Enter the day (01 - 31)		: ";
				cin >> user.date;
				cout << endl;
			}
			cout << "Enter the month (06 - 09)	: ";
			cin >> user.month;
			while (user.month < 6 || user.month > 9) {// loop when user key in wrong month
				cout << "Please key in a correct month." << endl;
				cin.clear();
				cin.ignore(999, '\n');
				cout << "Enter the month (06 - 09)	: ";
				cin >> user.month;
				cout << endl;
			}
			cout << "Enter the year			: " << y << endl;
			findoutday = day(y, user.month, user.date); // find out which day it is
			cout << endl << endl;
			cout << "                        *****************************" << endl;
			cout << "                        *   The day is ";
			switch (findoutday) {//display which day it is
			case 0: cout << "Sunday.      *\n                        *****************************\n\n";
				break;
			case 1: cout << "Monday.      *\n                        *****************************\n\n";
				break;
			case 2: cout << "Tuesday.     *\n                        *****************************\n\n";
				break;
			case 3: cout << "Wednesday.   *\n                        *****************************\n\n";
				break;
			case 4: cout << "Thursday.    *\n                        *****************************\n\n";
				break;
			case 5: cout << "Friday.      *\n                        *****************************\n\n";
				break;
			case 6: cout << "Saturday.    *\n                        *****************************\n\n";
				break;
			}
			// set the format of date become XXXXXXXX
			inputdate.open("checkdate.txt", ofstream::out | ofstream::trunc);
			inputdate << setfill('0') << setw(2) << user.date << setw(2) << user.month << y << endl;
			inputdate.close();
			outputdate.open("checkdate.txt");
			while (getline(outputdate, line))
				date_update = line;
			outputdate.close();
			for (int i = 0; i < 25; i++) {
				if (datelist[i] == date_update)
					trying = true;
			}
			if (trying == true) { //display error message if user key in wrong date
				cout << "The available date is starting from 15 June 2020 to 20 September 2020." << endl;
				system("pause");
				system("cls");
				logo();
			}

			else if (findoutday == 0 || findoutday == 6) { // display error message if the day is weekend
				cout << "Sorry, you can't teach during weekend.\nPlease have some rest." << endl;
				system("pause");
				system("cls");
				logo();
			}
		} while (findoutday == 0 || findoutday == 6 || trying == true); // loop until user key in correct date 

		if (zoom_update == "Zoom A" && timeslot_update == "Slot 11am-1pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom A" && timeslot == "Slot 11am-1pm") { // if someone the time slot on that date
					cout << "Sorry, you are not allowed to change to " << date_update << " because Zoom A with timeslot 11am-1pm is occupied on that day." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
		}
		else if (zoom_update == "Zoom A" && timeslot_update == "Slot 2pm-4pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom A" && timeslot == "Slot 2pm-4pm") { // if someone the time slot on that date
					cout << "Sorry, you are not allowed to change to " << date_update << " because Zoom A with timeslot 2pm-4pm is occupied on that day." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
		}
		else if (zoom_update == "Zoom B" && timeslot_update == "Slot 11am-1pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom B" && timeslot == "Slot 11am-1pm") { // if someone the time slot on that date
					cout << "Sorry, you are not allowed to change to " << date_update << " because Zoom B with timeslot 11am-1pm is occupied on that day." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
		}
		else if (zoom_update == "Zoom B" && timeslot_update == "Slot 2pm-4pm") {
			myfile.open("booking.txt");
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (date_update == date && zoom == "Zoom B" && timeslot == "Slot 2pm-4pm") { // if someone the time slot on that date
					cout << "Sorry, you are not allowed to change to " << date_update << " because Zoom B with timeslot 2pm-4pm is occupied on that day." << endl << endl;
					system("pause");
					cout << "Back to Main Menu";
					for (int i = 0; i < 3; i++) {
						cout << '.';
						Sleep(500);
					}
					system("cls");
					myfile.close();
					main();
				}
			}
			myfile.close();
		}
	}
	else if (choice == 6) {// back to menu 
		cout << "Back to Main Menu";
		for (int i = 0; i < 3; i++) {
			cout << '.';
			Sleep(500);
		}
		main();
	}

	myfile.open("temporarybooking.txt");
	outputfile.open("temporarybooking.txt", ios::app);
	if (myfile.is_open()) {
		if (outputfile.is_open()) {
			if (myfile.peek() == ifstream::traits_type::eof()) {
				outputfile << zoom_update << endl << timeslot_update << endl << course_update << endl << numstudent_update << endl << date_update;
			}
			else
				outputfile << endl << zoom_update << endl << timeslot_update << endl << course_update << endl << numstudent_update << endl << date_update;
		}
		else
			cout << "The file is unable to open.." << endl;
		outputfile.close();
	}
	else
		cout << "The file is unable to open.." << endl;
	myfile.close();

	b = 0;
	myfile.open("temporarybooking.txt");
	outputfile.open("booking.txt", ifstream::out, ifstream::trunc);
	if (outputfile.is_open()) {
		if (myfile.is_open()) {
			while (!myfile.eof()) {
				getline(myfile, zoom);
				getline(myfile, timeslot);
				getline(myfile, course);
				getline(myfile, numstudent);
				getline(myfile, date);

				if (b == 0) { // append all booking in temporary file into booking.txt
					outputfile << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;
					b++;
				}
				else
					outputfile << endl << zoom << endl << timeslot << endl << course << endl << numstudent << endl << date;

			}
		}
		else
			cout << "The file cannot open.." << endl;
		myfile.close();
	}
	else
		cout << "The file cannot open.." << endl;
	outputfile.close();
	cout << "\nThe booked zoom account has succesfully updated..." << endl << endl;
	cout << "Latest update info:" << endl << endl; // display updated info
	cout << "***************************************************************************" << endl;
	cout << "*  Zoom Account      :  " << setw(50) << left << zoom_update << "*" << endl;
	cout << "*  Time slot         :  " << setw(50) << left << timeslot_update << "*" << endl;
	cout << "*  Course Code       :  " << setw(50) << left << course_update << "*" << endl;
	cout << "*  Number of student :  " << setw(50) << left << numstudent_update << "*" << endl;
	cout << "*  Date              :  " << setw(50) << left << date_update << "*" << endl;
	cout << "***************************************************************************" << endl << endl;
	cout << endl;
	system("pause");
	cout << "Back to Main Menu";
	for (int i = 0; i < 3; i++) {
		cout << '.';
		Sleep(500);
	}
	main();
}

void rules() {
	system("cls");
	logo(); // all rules and regulations when using this program
	cout << "Rules and Regulations :" << endl << endl;;
	cout << " 1)This system is strictly only for UTAR admin to use. Others are not allowed to use." << endl << endl;
	cout << " 2)Only two timeslots, 11 a.m. - 1 p.m. and 2 p.m. - 4 p.m. are available." << endl << endl;
	cout << " 3)There are only two zoom accounts available which is ZOOM A and ZOOM B." << endl << endl;
	cout << " 4)ZOOM A is able to accept maximum 500 students per session while ZOOM B is limited to 250 students per session." << endl << endl;
	cout << " 5)The booking is just available for June trimester starting from 15 June 2020 to 20" << '\n'
		<< "   September 2020." << endl << endl;
	cout << " 6)More than 1 reservations are allowed for a course as long as the slot is empty." << endl << endl;
	cout << " 7)Name of courses, number of students and also date of booking must be provided while booking is carried out." << endl << endl;
	cout << " 8)Zoom A is used when Zoom B is not available or the class is having more than 250" << '\n'
		<< "   students." << endl << endl;
	cout << " 9)If the selected slot is occupied, the system will show the name of course and ask the admin whether" << '\n'
		<< "   to update (or remain the booking) or not." << endl << endl;
	cout << "10)A lecturer may request the admin to cancel the booking." << endl << endl;
	cout << "11)Please follow all the rules and regulations set by UTAR when using this system." << endl;
	cout << "\n\n";
	system("pause");
	cout << "Back to Main Menu";
	for (int i = 0; i < 3; i++) {
		cout << '.';
		Sleep(500);
	}
	main();
}

void loading() { // loading feature
	system("cls");
	logo();
	for (int i = 0; i < 101; i++) {
		cout << "\rLoading..." << i << "%";
		Sleep(25);
	}
	cout << endl;
	system("cls");
}

void logo() // logo design
{
	cout << "            __          __  _                            _______      ______                     " << endl;
	cout << "            \\ \\        / / | |                          |__   __|    |___  /                     " << endl;
	cout << "             \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___     | | ___      / / ___   ___  _ __ ___  " << endl;
	cout << "              \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\    | |/ _ \\    / / / _ \\ / _ \\| '_ ` _ \\ " << endl;
	cout << "               \\  /\\  /  __/ | (_| (_) | | | | | |  __/    | | (_) |  / /_| (_) | (_) | | | | | |" << endl;
	cout << "                \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|    |_|\\___/  /_____\\___/ \\___/|_| |_| |_|" << endl;
	cout << "" << endl;
	cout << "" << endl;
	cout << "                  ____              _    _                _____           _                 " << endl;
	cout << "                 |  _ \\            | |  (_)              / ____|         | |                " << endl;
	cout << "                 | |_) | ___   ___ | | ___ _ __   __ _  | (___  _   _ ___| |_ ___ _ __ ___  " << endl;
	cout << "                 |  _ < / _ \\ / _ \\| |/ / | '_ \\ / _` |  \\___ \\| | | / __| __/ _ \\ '_ ` _ \\ " << endl;
	cout << "                 | |_) | (_) | (_) |   <| | | | | (_| |  ____) | |_| \\__ \\ ||  __/ | | | | |" << endl;
	cout << "                 |____/ \\___/ \\___/|_|\\_\\_|_| |_|\\__, | |_____/ \\__, |___/\\__\\___|_| |_| |_|" << endl;
	cout << "                                                  _/  |          _/  |                      " << endl;
	cout << "                                                 |___/          |___/                                  " << "\n\n" << endl;
}

void checkbooking(int d, int m, int y, string& realdate) {
	ifstream outputdate;
	string zoom, slot, course, num, date;
	int a = 1;
	bool trying = false;

	outputdate.open("booking.txt");
	cout << "You are allowed to book this date and booking on " << realdate << " are :\n" << endl;
	cout << "**************************************************************************************************************************************" << endl;
	cout << "| No  |   Zoom Room  |     Time slot      |                    Course Name                    |    Number of Student   |     Date    |" << endl;
	cout << "**************************************************************************************************************************************" << endl;
	if (outputdate.is_open()) {
		while (!outputdate.eof()) {
			getline(outputdate, zoom);
			getline(outputdate, slot);
			getline(outputdate, course);
			getline(outputdate, num);
			getline(outputdate, date);

			if (realdate == date) { // display all occupied time slot on that date
				cout << "| " << a << ")  |";
				cout << "   " << zoom << "     |" << "   " << setw(13) << left << slot << "    |   " << setw(47) << left << course << " |           " << setw(3) << left << num << "          |" << setw(3) << right << "" << date << "  |" << endl;
				cout << "**************************************************************************************************************************************" << endl;
				trying = true;
				a += 1;
			}
		}
		if (trying == false) { // no time slot is occupied
			cout << "*                                                                                                                                    *" << endl;
			cout << "*                                                                                                                                    *" << endl;
			cout << "*                                                                                                                                    *" << endl;
			cout << "*                                                 No booking for this date currently..                                               *" << endl;
			cout << "*                                                                                                                                    *" << endl;
			cout << "*                                                                                                                                    *" << endl;
			cout << "*                                                                                                                                    *" << endl;
			cout << "**************************************************************************************************************************************" << endl;
		}
		system("PAUSE");
	}
	else
		cout << "The file is unable to open.." << endl;
	outputdate.close();
}

int select_info_to_update() { // let user choose which info they want to update
	int choice;
	system("cls");
	logo();
	cout << setw(30) << right << "**************************************************************************************" << endl;
	cout << setw(30) << right << "*   Please select what information of the booked zoom account do you want to update  *" << endl;
	cout << setw(30) << right << "************|*********************************************************|***************" << endl;
	cout << setw(30) << right << "*           |                Insert 1 for zoom account                |              *" << endl;
	cout << setw(30) << right << "*           |                Insert 2 for time slot                   |              *" << endl;
	cout << setw(30) << right << "*           |                Insert 3 for course name                 |              *" << endl;
	cout << setw(30) << right << "*           |                Insert 4 for number of students          |              *" << endl;
	cout << setw(30) << right << "*           |                Insert 5 for date                        |              *" << endl;
	cout << setw(30) << right << "*           |                Insert 6 to back to main menu            |              *" << endl;
	cout << setw(30) << right << "**************************************************************************************" << endl;

	do {
		cout << "Selection (1/2/3/4/5/6): ";
		cin >> choice;
		if (choice <= 0 || choice >= 7) { // display error message when user key in wrong number
			cout << "Please key in a correct selection .." << endl;
			cin.clear();
			cin.ignore(999, '\n');
		}
	} while (choice <= 0 || choice >= 7); // loop when user key in wrong number

	return choice;
}

void record_booking_to_be_changed(int choice, string& zoom_changed, string& timeslot_changed, string& course_changed, string& numstudent_changed, string& date_changed) {
	ifstream myfile;
	string zoom, course, date, timeslot, numstudent;
	int b = 0;

	myfile.open("booking.txt");
	if (myfile.is_open()) {
		while (!myfile.eof()) {
			getline(myfile, zoom);
			getline(myfile, timeslot);
			getline(myfile, course);
			getline(myfile, numstudent);
			getline(myfile, date);

			b += 1;
			// record the booking chosen by user
			if (choice == b) {
				zoom_changed = zoom;
				timeslot_changed = timeslot;
				course_changed = course;
				numstudent_changed = numstudent;
				date_changed = date;
			}
		}
	}
	else {
		cout << "The file is unable to open.." << endl;
	}
	myfile.close();
}
