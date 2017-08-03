#include "stdafx.h"
#include "DBCAnalyzer.h"
#include "Search.h"
#include <fstream>
#include <iostream>
#include <Windows.h>

//#define DEBUG
#ifndef DEBUG

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

#else

#include <gtest\gtest.h>
#include <gmock\gmock.h>
#include <deque>

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(yizhengc, split1)
{
	std::deque<std::string> vs;
	SPILT("he llo wor ld he llo wo rld   ", ' ', vs);
	EXPECT_EQ(8, vs.size());
}

TEST(yizhegc, spilt2)
{
	std::vector<std::string> vs;
	std::vector<uint32_t> vi;
	split_number_and_string("hello 2 3 4 5 sdfgh 45 ty 9", vi, vs);
	EXPECT_EQ(6, vi.size());
	EXPECT_EQ(3, vs.size());
}

TEST(yizhengc, regex)
{
	DBCAnalyzer afile;
	auto descriptor = afile.Analyze("D:\\Git-SourceTree\\test.dbc");
	EXPECT_EQ(3, descriptor.Messages().size());
	EXPECT_EQ(6, descriptor.Comments().size());
	EXPECT_EQ(8, descriptor.AttributeValues().size());
	EXPECT_EQ(5, descriptor.SignalValues().size());
}
#endif

