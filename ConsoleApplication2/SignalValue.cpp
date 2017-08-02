#include "stdafx.h"
#include "SignalValue.h"
#include "Signal.h"
#include <regex>

/**
*  For example:
*  liyiwe,12,iwijdf
*  -> 12  ---> _vi
*  -> liyiwe|iwijdf ---> _vs
*  @param an string which has numbers and words
*  @param an uint32_t vector which is used to save numbers
*  @param an string vector which is used to save other words
*  @return void
*/
void split_number_and_string(const std::string & _Str, std::vector<uint32_t> & _Vi, std::vector<std::string> & _Vs, char c)
{
	std::vector<std::string> vs;
	SPILT(_Str, ' ', vs);
	for (auto iter = vs.begin(); iter < vs.end(); iter++)
	{
		if (std::regex_match(*iter, std::regex(R"(\d+)")))
		{
			_Vi.push_back(std::stol(*iter));
		}
		else
		{
			_Vs.push_back(*iter);
		}
	}
}

/**
*  @brief output class signalvalue
*  @param an ostream object
*  @param class signalvalue
*  @return an ostream object
*/
std::ostream & operator<<(std::ostream & os, const SignalValue & signal_value)
{
	os << "SignalValue's message id: " << signal_value.MessageId() <<
		"\tSignalValue's signal name: " << signal_value.SignalName() <<
		"\tSignalValue's value description: ";
	for (auto iter_value_description = signal_value.ValueDescription().begin(); iter_value_description < signal_value.ValueDescription().end(); iter_value_description++)
	{
		os << iter_value_description->Value() << ' ' << iter_value_description->Name() << '\t';
	}
	return os;
}