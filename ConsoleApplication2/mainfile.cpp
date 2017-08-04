#include "stdafx.h"
#include "DBCAnalyzer.h"
#include "Search.h"
#include <fstream>
#include <iostream>
#include <Windows.h>

int main()
{
	std::string command_name;
	while (1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
		std::cout << "Please input your command: ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//7 is white
		getline(std::cin, command_name);
		analyzer(command_name);
	}
	return EXIT_FAILURE;
}



