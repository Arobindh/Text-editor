#pragma once
#include<string>
using std::string;
class Command {
private:
	string command_line, symbol, address1, address2;
	bool valid;
public:
	Command();
	Command(const string& command_line);
	void parse(const string& command_line);// to parse the command 
	string getSymbol();// to get symbol a private member
	string getAddress1();// to get address 1 a private member
	string getAddress2();// to get address 2 a private member
	bool getStatus();// returns the validity of the command
	void setStatus(bool status);// set the status of the command
};

