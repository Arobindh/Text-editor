#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<iterator>
#include "editor.h"
#include "command.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

using namespace std;

// Constructor to get the filename
LineEd::LineEd(const string filename){
	LineEd::file = filename;
	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs.is_open()) {
		cout << "\"" << (filename == "" ? "?" : filename) << "\" [New File]" << endl;
		switchMode("command");
	} 
	else {
		std::string line;
		string fetchedLine;
		//adding the contents of contents of the file to the buffer
		while (!ifs.eof()) {																	
			getline(ifs, fetchedLine);															
			buffer.push_back(fetchedLine);														
		}
		current = buffer.size();
		cout << "\"" << filename << "\" "<< buffer.size() << " line(s)";
		switchMode("command");
	}

}

// switching between ip and command mode.
void LineEd::switchMode(string mode){
	cout << endl << "Entering " << mode << " mode." << endl;
	if (mode == "command"){
		cin >> skipws;
	}
	else if (mode == "input"){
		cin >> noskipws;
	}
	
}

bool LineEd::userYesNo(string message){
	char response = ' ';
	cout << message;
	while (response != 'y' && response != 'n') {
		cin >> response;
		response = tolower(response);
	}
	return response;
}
//method that runs the command.
void LineEd::runCommand(const string syntax){
	list<string>::iterator it1, it2;
	string fetchedLine = "";
	int lastInsertedLine;
	Command com{ syntax };
	if (validateCommand(com)) {
		//validate the address
		string address1 = com.getAddress1(); 
		string address2 = com.getAddress2();
		int add1, add2;
		if (buffer.size() == 0) {
			add1 = add2 = 1;
			it1 = it2 = buffer.begin();
		}
		else{

			// correcting address 1 values.
			if (address1 == "$"){
				add1 = buffer.size(); 
				it1 = buffer.end(); 
			}
			else if (address1 == ".") { 
				add1 = current; 
				it1 = buffer.begin();
				advance(it1, current - 1);
			}
			else if (address1 == ""){
				add1 = 1; 
				it1 = buffer.begin();
			}
			else { 
				istringstream adres1{ address1 };
				adres1 >> add1;
			}

			// correcting address 2 values.
			if (address2 == "$"){
				add2 = buffer.size();
				it2 = buffer.end();
			}
			else if (address2 == "."){
				add2 = current; 
				it2 = buffer.begin(); 
				advance(it2, current - 1);
			}
			else if (address2 == ""){ 
				add2 = add1; 
				it2 = it1;
			}
			else {
				istringstream adres2{ address2 };
				adres2 >> add2;
				}

			// swap add if small
			if (add2 <= add1) {
				int temp;
				temp = add1; 
				add1 = add2;
				add2 = temp; 
			}

			// add1 if less than 0
			if (add1 < 0){ 
				add1 = 1;
				it1 = buffer.begin();
			}								
			// add1 if greater than buffer size
			if (add1 > buffer.size()){ 
				add1 = buffer.size(); 
				it1 = buffer.end();
			}							
			it1 = buffer.begin();
			advance(it1, add1 - 1);										

			if (add2 < 0) {
				add2 = 0; 
				it2 = buffer.begin();
			}									// same assignments as address1
			if (add2 > buffer.size()){ 
				add2 = buffer.size();
				it2 = buffer.end();
			}				\
			it2 = buffer.begin(); 
			advance(it2, add2 - 1);										
		}

		list<string>::iterator iter1;
		list<string>::iterator iter2;
		string concat, lineText;
		string original, replacement;
		int curr_working, findInstance;

		char commandLetter = com.getSymbol()[0];		
		switch (commandLetter) {								
		case 'a':												// operation for a done in next case
		case 'i':												//insert command
			switchMode("input");
			current = add1;
			while (fetchedLine != "."){
				clear();
				getline(cin, fetchedLine);
				if (fetchedLine == ".") 
				 break; 
				if (commandLetter == 'i')						// for i insert
					buffer.insert(it1, fetchedLine);			
				else											
					buffer.push_back(fetchedLine);				// for  'a'ppend
				    current++;									// Update current
			}
			if (commandLetter == 'i')
				current--;									// reverting current
			else												// Surely coming from 'a'
				current = buffer.size();
			switchMode("command");
			break;
		case 'v':												// Paste at add1
			if (clipboard.size() < 1) {							// check for clip board empty
				cout << " Clipboard is empty." << endl; 
			}
			else{
				current = add1 + clipboard.size() - 1;
				iter1 = it1;
				int vit = clipboard.size();
				while (clipboard.size() != 0) {
					string copiedLine = clipboard.at(vit-1);	// Fetch the content from last
					vit--;
					clipboard.pop_back();				//  remove the content from clipboard
					buffer.insert(iter1, copiedLine);				//  loading in the buffer
					advance(iter1, -1);							// moving back copying from last
				}
			}
			break;
		case 'd':												// Delete all from add1 to add2
			if (buffer.size() < 1) {
				cout << "File empty. Must use one of I,A,Q commands." << endl;
			}
			else{
				current = add1 - 1;
				curr_working = add1 - 1;
				for (iter1 = it1; iter1 != it2;) {					// to delete from begin
					curr_working++;
					iter2 = iter1;
					++iter1;
					buffer.erase(iter2);
				}
				buffer.erase(it2);
			}
			break;
		case 'x':												// Cut all from add1 to add2
			if (buffer.size() < 1) {							
				  cout << " Buffer is empty." << endl; 
			}
			else{
				current = add1;
				for (iter1 = it1; iter1 != it2;) {			// Cycle through @buffer and cut targets
					clipboard.push_back(*iter1);					//  from @add1 and @add2 to @clipboard
					iter2 = iter1;
					++iter1;
					buffer.erase(iter2);
				}

				clipboard.push_back(*it2);//to insert the last element
				buffer.erase(it2);
			}
			break;
		
		case 'j':												// Join all from add1 to add2 into add1
			if (add2-add1 > 0) {
				current = add1;								// Initialize the list iterator 
				concat = *it1;							
				iter1 = it1;										
				iter1++;											
				curr_working = add1;								
				while( iter1 != it2) {						// Travels from address1 to address2 and add all into the string concat
					concat += *iter1;						
					iter2 = iter1;
					++iter1;
					buffer.erase(iter2);
				}
				concat += *it2;
				buffer.erase(it2);
				*it1 = concat;
			}
			break;

		case 'p':												// Print all from add1 to add2
			if (buffer.size() < 1) {
				cout << "File empty. Must use one of I,A,Q commands." << endl;
			}
			else{
				curr_working = add1 - 1;
			
				for (iter1 = it1; iter1 != it2; ++iter1) {
					curr_working++;
					cout  << curr_working << (curr_working == current ? ">" : ":") << *iter1 << endl;
				}
				curr_working++;
				cout  << curr_working << (curr_working == current ? ">" : ":") << *iter1 << endl;
			}
			break;
		case 'c':												// Replace all substrings from add1 to add2
			cout << "Insert the text to change ";									//clear cin
			clear();											
			getline(cin, original);

			cout << "Enter the text to replace ";
			clear();
			getline(cin, replacement);
			
			curr_working = add1 - 1;
			for (iter1 = it1; iter1 != it2; ++iter1) {				// Traverse from address1 to address2
				curr_working++;									
				lineText = *iter1;
				findInstance = lineText.find(original);// search for original
				if (findInstance != string::npos) {   //replaces if match is found
					current = curr_working;
					lineText.replace(findInstance, original.length(), replacement);
					*iter1 = lineText;
				}
			}
			lineText = *it2;
			findInstance = lineText.find(original);
			if (findInstance != string::npos) {
				current = add2;
				lineText.replace(findInstance, original.length(), replacement);
				*it2 = lineText;
			}
			break;
		case 'u':// move up by s line
			if (commandLetter == 'u') {
				if (--current < 1) {						
					current++;
					cout << "BOF reached." << endl;
				}
			}
			iter1 = buffer.begin();
			advance(iter1, current - 1);
			cout << current << ">" << *iter1 << endl;			// Indicated the currentLine and its contents
			break;
		case 'n':												//move down by a line
			if (commandLetter == 'n') {
				if (++current > buffer.size()) {			// Clip increment to end of file
					current--;
					cout << "EOF reached." << endl;
				}
			}
			iter1 = buffer.begin();
			advance(iter1, current - 1);
			cout << current << ">" << *iter1 << endl;			// Printing current line
			break;
		case 'g':												// Sets current line
			current = add1;
			cout << current << ">" << *iter1 << endl;			// prints the current line
			break;
		case 'w':												// Write changes
		case 'q':												// Quit but ask for writing changes before
			bool saveYN, wantedName;
			if (commandLetter == 'q') {
				saveYN = userYesNo("There are unsaved changes.\nDo you wish to save the changes (y or n)? ");
				if (saveYN) {
					if (file == ""){
						cout << "Enter the name of a file to write to: ";
						clear();
						getline(cin, file);
					}
				}
			}
			else{
				saveYN = userYesNo("Save changes to the file: " + file + " (y or n)? ");
				if (file == ""){
					cout << "Enter the name of a file to write to: ";
					clear();
					getline(cin, file);
				}
			}
			if (saveYN){//write for both q and w
				std::ofstream ofs(file, std::ofstream::out);	// Write the file
				for (iter1 = buffer.begin(); iter1 != buffer.end(); iter1++) {
					ofs << *iter1 << endl;
				}
				ofs.close();
				cout << buffer.size() << " line(s) written to file " << "\"" << file << "\"" << endl;
			}
			if (commandLetter == 'q') {								// Exit the program
				cout << "bye" << endl;
				exit(EXIT_SUCCESS);
			}
			break;
		default:													//when none of the case satisfies bad comment
			cout << "bad command: " << commandLetter << endl;
			break;
		}
	}
}

void LineEd::run(){
	string command;
	while (true){					// infinite loop
		cout << "?";				// To indicate the question
		getline(cin, command);		// Get command
		runCommand(command);		// Run it as a command
	}
}

// Clears Cin
void LineEd::clear(){
	if (cin.peek() == EOF || cin.peek() == '\n')
		cin.ignore(1);
}

// Test of a command is valid
bool LineEd::validateCommand(Command& com) {
	string add1 = com.getAddress1();
	string add2 = com.getAddress2();
	if (com.getStatus()) {
		bool valid1 = isValidAddress(add1);
		bool valid2 = isValidAddress(add2);
		if (!valid1) { cout << "Bad address 1: " << add1 << endl; }
		if (!valid2) { cout << "Bad address 2: " << add2 << endl; }
		com.setStatus( valid1 && valid2);
	}
	else{
		 cout << " Command cannot be validated." << endl; 
	}
	return com.getStatus();
}

// Test if the given addresses is valid
bool LineEd::isValidAddress(string& text){
	int test = 0;
	istringstream check{ text };
	check >> test;
	return (text == "" || text == "." || text == "$") || (test != 0 && check.peek() == EOF);
}