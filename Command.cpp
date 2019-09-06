#include<sstream>
#include<iostream>
#include<string>
#include "Command.h"

using namespace std;



// Default constructor
Command::Command() : command_line{}, symbol{}, address1{}, address2{}, valid{ true } {}

// Constructor by the @command_line string
Command::Command(const string& command_line): command_line {}, symbol{}, address1{}, address2{}, valid{true} {
	parse(command_line);
}

// Passive analyser (first responder) for @command_line transformation into a @Command
void Command::parse(const string& command_line) {
	char comm = '\0';								// 'char' type for symbol initialized to null
	istringstream ss{ command_line };
	switch (command_line[0]) {							// finds the command
	case 'a':
	case 'i':
	case 'v':
	case 'd':
	case 'x':
	case 'j':
	case 'p':
	case 'c':
	case 'u':
	case 'n':
	case 'g':
	case 'w':
	case 'q':
		ss >> comm;									// Capture the command as a symbolc
		symbol = tolower(comm);						// Converts the char into string
	case '.':
	case ',':
	case '$':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		symbol = (comm == '\0') ? 'p' : comm;	//deafult command print
		getline(ss, address1, ',');
		getline(ss, address2);
		break;
	default:											// Bad command
		cout << endl << "bad command: " << command_line[0] << endl;
		valid = false;
		break;
	}
}

string Command::getSymbol() {
	return symbol;
}

string Command::getAddress1() {
	return address1;
}


string Command::getAddress2() {
	return address2;
}

bool Command::getStatus() {
	return valid;
}


void Command::setStatus(bool status) {
	Command::valid = status;
}