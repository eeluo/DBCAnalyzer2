#include "stdafx.h"
#include "Search.h"
#include <regex>
#include <fstream>
#include <Windows.h>

static DBCFileDescriptor  descriptor;
static bool file_is_open = false;
/**
* @brief analyzer commands
* @param command
*/
void analyzer(const std::string & _str)
{
	if ("exit" == _str)
	{
		exit(0);
	}
	if ("help" == _str)
	{
		std::cout
			<< "exit: exit this programmer    close: close opened file\n"
			<< "output: output something from file\n"
			<< "(messages|comments|attributevalues|signalvalues|attributes|nodes)\n"
			<< "you need input [output (the last line's things) file(or con)]\n"
			<< "search: You should input what you want,\n"
			<< "such as [search message(or other things) id(or name    etc)],\n"
			<< "and then you need input the id or name. These are a lot of search command:\n"
			<< "message: message id, message name, signal receiver, transimitter\n"
			<< "comment: comment message id\n"
			<< "signalvalue: signalvalue message id\n"
			<< "attribute: attribute name,attribute valuetype\n"
			<< "attributevalue: attribute objecttype\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout
			<< "Be careful about [space]!"
			<< std::endl;
		return;
	}
	if ("close" == _str)
	{
		if (file_is_open)
		{
			file_is_open = false;
			std::cout << "File is closed." << std::endl;
		}
		else
		{
			std::cout << "File is not opened" << std::endl;
		}
		return;
	}


	std::smatch m;
	std::regex_match(_str, m, std::regex(R"((\w+)\s(.*))"));
	if ("open" == m[1].str())
	{
		open_file(m[2].str());
	}
	else if ("search" == m[1].str())
	{
		search(m[2].str());
	}
	else if ("output" == m[1].str())
	{
		output(m[2].str());
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << _str << " is not an effective command!" << std::endl;
	}
	return;
}
/**
* @brief print some
* @param file name
*/
static void output(const std::string & _str)
{
	if (!file_is_open)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: File is not opened!" << std::endl;
		return;
	}

	std::smatch m;
	if (!std::regex_match(_str, m, std::regex(R"((\w+)\s(\w+))")))
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << _str << " is not an effective command!" << std::endl;
		return;
	}

	std::ofstream file(m[2].str());
	if ("message" == m[1].str())
	{
		descriptor.PrintMessages(file);
	}
	else if ("comment" == m[1].str())
	{
		descriptor.PrintComments(file);
	}
	else if ("attribute" == m[1].str())
	{
		descriptor.PrintAttributes(file);
	}
	else if ("attributevalue" == m[1].str())
	{
		descriptor.PrintAttributeValues(file);
	}
	else if ("signalvalue" == m[1].str())
	{
		descriptor.PrintSignalValues(file);
	}
	else if ("node" == m[1].str())
	{
		descriptor.PrintNodes(file);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << m[1].str() << " is not an effective command!" << std::endl;
	}
}
/**
* @brief open file
* @param file name
*/
static void open_file(const std::string & _str)
{
	DBCAnalyzer dbca_file;
	descriptor = dbca_file.Analyze(_str);
	file_is_open = true;
	return;
}
/**
* @brief search somthing
* @param command
*/
static void search(const std::string & _str)
{
	std::smatch m;
	if (!file_is_open)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: File is not opened!" << std::endl;
	}
	else if (std::regex_match(_str, m, std::regex(R"(message\s(.*))")))
	{
		message_search(m[1].str());
	}
	else if (std::regex_match(_str, m, std::regex(R"(comment\s(.*))")))
	{
		comment_search(m[1].str());
	}
	else if (std::regex_match(_str, m, std::regex(R"(attribute\s(.*))")))
	{
		attribute_search(m[1].str());
	}
	else if (std::regex_match(_str, m, std::regex(R"(attributevalue\s(.*))")))
	{
		attributevalue_search(m[1].str());
	}
	else if (std::regex_match(_str, m, std::regex(R"(signalvalue\s(.*))")))
	{
		signalvalue_search(m[1].str());
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << m[1].str() << " is not an effective command!" << std::endl;
	}
	return;
}
/**
* @brief search message
* @param command
*/
static void message_search(const std::string & _str)
{
	if ("id" == _str)
	{
		message_id_search();
	}
	else if ("name" == _str)
	{
		message_name_search();
	}
	else if ("signal receiver" == _str)
	{
		signal_receiver_search();
	}
	else if ("transmitter" == _str)
	{
		message_transmitter_search();
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << _str << " is not an effective command!" << std::endl;
	}
	return;
}
/**
* @brief search message's ID
*/
static void message_id_search()
{
	std::cout << "Please input the ID: ";
	uint32_t id;
	std::cin >> id;
	getchar();
	Message msg;
	if (descriptor.MessageIdSearch(id, msg))
	{
		std::cout << "Have this ID. This message is:\n" << msg;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << id << " can't be found" << std::endl;
	}
}
/**
* @brief search message's name
*/
static void message_name_search()
{
	std::cout << "Please input the name: ";
	std::string str;
	std::cin >> str;
	getchar();
	Message msg;
	if (descriptor.MessageNameSearch(str, msg))
	{
		std::cout << "Have this name. This message is:\n" << msg;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << str << " can't be found" << std::endl;
	}
}
/**
* @brief search message's transmitter
*/
static void message_transmitter_search()
{
	std::cout << "Please input the transmitter: ";
	std::string str;
	std::cin >> str;
	getchar();
	std::vector<Message> vmsg;
	uint32_t num = descriptor.MessageTransmitterSearch(str, vmsg);
	if (num)
	{
		std::cout << num << " signals have this transmitter. Do you want to know them: ";
		std::cin >> str;
		getchar();
		if ("yes" == str)
		{
			for (auto iter = vmsg.begin(); iter < vmsg.end(); iter++)
			{
				std::cout << *iter;
			}
		}
		else if ("no" == str)
		{
			;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "Error: " << str << " is not an effective command!" << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << str << " can't be found!" << std::endl;
	}
	return;
}
/**
* @brief search message's signal receiver
*/
static void signal_receiver_search()
{
	std::cout << "Please input the receiver: ";
	std::string str;
	std::cin >> str;
	getchar();
	std::vector<Signal> vsg;
	uint32_t num = descriptor.SignalReceiversSearch(str, vsg);
	if (num)
	{
		std::cout << num << " signals have this receiver. Do you want to know them?\n: ";
		std::cin >> str;
		getchar();
		if ("yes" == str)
		{
			for (auto iter = vsg.begin(); iter < vsg.end(); iter++)
			{
				std::cout << *iter << std::endl;
			}
		}
		else if ("no" == str)
		{
			;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "Error: " << str << " is not an effective command!" << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << str << " can't be found!" << std::endl;
	}
	return;
}
/**
* @brief search comment
* @param command
*/
static void comment_search(const std::string & _str)
{
	if ("message id" == _str)
	{
		comment_message_id_search();
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << _str << " is not an effective command!" << std::endl;
	}
	return;
}

/**
* @brief search comment's message ID
*/
static void comment_message_id_search()
{
	std::cout << "Please input the ID: ";
	uint32_t id;
	std::cin >> id;
	getchar();
	std::vector<Comment> vcmt;
	uint32_t num = descriptor.CommentMessageIdSearch(id, vcmt);
	if (num)
	{
		std::cout << num << " comments have this ID. Do you want to know them?\n: ";
		std::string str;
		std::cin >> str;
		getchar();
		if ("yes" == str)
		{
			for (auto iter = vcmt.begin(); iter < vcmt.end(); iter++)
			{
				std::cout << *iter << std::endl;
			}
		}
		else if ("no" == str)
		{
			;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "Error: " << str << " is not an effective command!" << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << id << " can't be found!" << std::endl;
	}
}
/**
* @brief search signalvalue
* @param command
*/
static void signalvalue_search(const std::string & _str)
{
	if ("message id" == _str)
	{
		signalvalue_message_id_search();
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << _str << " is not an effective command!" << std::endl;
	}
	return;
}

/**
* @brief search signalavue's message ID
* @param command
*/
static void signalvalue_message_id_search()
{
	std::cout << "Please input the ID: ";
	uint32_t id;
	std::cin >> id;
	getchar();
	std::vector<SignalValue> vsv;
	uint32_t num = descriptor.SignalValueMessageIdSearch(id, vsv);
	if (num)
	{
		std::cout << num << " signalvalues have this ID. Do you want to know them?\n: ";
		std::string str;
		std::cin >> str;
		getchar();
		if ("yes" == str)
		{
			for (auto iter = vsv.begin(); iter < vsv.end(); iter++)
			{
				std::cout << *iter << std::endl;
			}
		}
		else if ("no" == str)
		{
			;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "Error: " << str << " is not an effective command!" << std::endl;
		}
	}
	else
	{
		std::cout << id << " can't be found!" << std::endl;
	}
}

/**
* @brief search attribute
* @param command
*/
static void attribute_search(const std::string & _str)
{
	if ("name" == _str)
	{
		attribute_name_search();
	}
	else if ("valuetype" == _str)
	{
		attribute_valuetype_search();
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << _str << " is not an effective command!" << std::endl;
	}
	return;
}

/**
* @brief search attribute's name
* @param command
*/
static void attribute_name_search()
{
	std::cout << "Please input the name: ";
	std::string str;
	std::cin >> str;
	getchar();
	Attribute abt;
	if (descriptor.AttributeNameSearch(str, abt))
	{
		std::cout << "Have this name. This attribute is;\n" << abt << std::endl;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << str << " can't be found!" << std::endl;
	}
	return;
}

/**
* @brief search attribute's valuetype
* @param command
*/
static void attribute_valuetype_search()
{
	std::cout << "Please input the objecttype.\n0 is nothing, 1 is BO_, 2 is BU_, 3 is SG_, 4 is Ev_\n: ";
	uint32_t value;
	std::cin >> value;
	getchar();
	if (value > 4)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Don't have this type!" << std::endl;
		return;
	}
	AttributeValue::_ObjectType vt = static_cast<AttributeValue::_ObjectType>(value);
	std::vector<AttributeValue> vabtv;
	uint32_t num = descriptor.AttributeValueObjectTypeSearch(vt, vabtv);
	if (num)
	{
		std::cout << num << " attributevalues have this object type. Do you want to know them?\n: ";
		std::string str;
		std::cin >> str;
		getchar();
		if ("yes" == str)
		{
			for (auto iter = vabtv.begin(); iter < vabtv.end(); iter++)
			{
				std::cout << *iter << std::endl;
			}
		}
		else if ("no" == str)
		{
			;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "Error: " << str << " is not an effective command!" << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "NO attributevalues have this value type." << std::endl;
	}
	return;
}

/**
* @brief search attributevalue
* @param command
*/
static void attributevalue_search(const std::string & _str)
{
	if ("objecttype" == _str)
	{
		attributevalue_objecttype_serarch();
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: " << _str << " is not an effective command!" << std::endl;
	}
}

/**
* @brief search attributevalue's object type
*/
static void attributevalue_objecttype_serarch()
{
	std::cout << "Please input the valuetype.\n0 is INT, 1 is HEX, 2 is FLOAT, 3 is STRING, 4 is ENUM: ";
	uint32_t value;
	std::cin >> value;
	getchar();
	if (value > 4)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Don't have this type!" << std::endl;
		return;
	}
	Attribute::VALUE_TYPE vt = static_cast<Attribute::VALUE_TYPE>(value);
	std::vector<Attribute> vabt;
	uint32_t num = descriptor.AttValueTypeSearch(vt, vabt);
	if (num)
	{
		std::cout << num << " attributes have this value type. Do you want to know them?\n: ";
		std::string str;
		std::cin >> str;
		getchar();
		if ("yes" == str)
		{
			for (auto iter = vabt.begin(); iter < vabt.end(); iter++)
			{
				std::cout << *iter << std::endl;
			}
		}
		else if ("no" == str)
		{
			;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			std::cout << "Error: " << str << " is not an effective command!" << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "NO attributes have this value type." << std::endl;
	}
	return;
}