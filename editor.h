#pragma once
#include<list>
#include<string>
#include<vector>
#include<iterator>
#include "Command.h"
using namespace std;
class LineEd
{
private:
	string file;
	int current;
	int b_Size;
	bool userYesNo(string message);// to get yes no from user
public:
	
	string mode;
	list<string> buffer;
	vector<string> clipboard;
	LineEd(const string filename);// constructor with file name
	void switchMode(string mode);// to swich between input and command mode
	void runCommand(const string syntax);// runs the command from the user
	void run();//starts line editor
	void clear();//clear cin
	bool validateCommand(Command& com);// to validate the command
	bool isValidAddress(string& text);// to vakidate the address
};