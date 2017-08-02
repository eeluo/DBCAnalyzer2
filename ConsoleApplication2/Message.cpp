#include "stdafx.h"
#include "Message.h"

/**
*  @brief output class message
*  @param an ostream object
*  @param class message
*  @return an ostream object
*/
std::ostream & operator<<(std::ostream & os, const Message & message)
{
	os << "Message name: " << message.Name() <<
		"\tMessage ID: " << message.ID() <<
		"\tMessage size: " << message.MsgSize() <<
		"\tMessage transmitter: " << message.Msgtransmitter() <<
		"\n";
	for (auto iter_signal = message.Signals().begin(); iter_signal < message.Signals().end(); iter_signal++)
	{
		os << '\t' << *iter_signal << '\n';
	}
	return os;
}