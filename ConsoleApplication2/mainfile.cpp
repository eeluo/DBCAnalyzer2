#include "stdafx.h"
#include "DBCAnalyzer.h"

#include <fstream>
#include <iostream>

//#define DEBUG
#ifndef DEBUG

int main()
{
	std::ofstream file("D:\\Git-SourceTree\\test.txt");
	DBCAnalyzer a_file;
	DBCFileDescriptor descriptor(a_file.Analyze("D:\\Git-SourceTree\\CAN3_151215_CL_local.dbc"));
	descriptor.PrintDescriptor(file);
	std::cout 
		<< descriptor.AttributeValueObjectTypeSearch(AttributeValue::SG_) << std::endl
		<< descriptor.MessagetransmitterSearch("CMU_11") << std::endl
		<< descriptor.CommentMessageIdSearch(1770) << std::endl
		<< descriptor.SignalReceiversSearch("BMU") << std::endl
		<< descriptor.SignalValueMessageIdSearch(1553) << std::endl;
	descriptor.MessageNameSearch("CMU_RS1_11", true);
	descriptor.MessageIdSearch(1620, true);
	file.close();
	system("pause");
	return 0;
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