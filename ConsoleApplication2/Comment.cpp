#include "stdafx.h"
#include "Comment.h"

/**
*  @brief output class comment
*  @param an ostream object
*  @param class comment
*  @return an ostream object
*/
std::ostream & operator<<(std::ostream & os, const Comment & comment)
{
	os << "Comment\'s message id: " << comment.MessageId() <<
		"\tComment\'s signal name: " << comment.SignalName() <<
		"\tComment\'s char string: " << comment.CharString();
	return os;
}