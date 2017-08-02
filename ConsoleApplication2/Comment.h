#pragma once

#ifndef _COMMENT_H_
#define _COMMENT_H_

#include <string>
#include <cstdint>
#include <iostream>

class Comment{
public:
	void SetMessageId(uint32_t message_id)
	{
		m_message_id = message_id;
	}
	uint32_t MessageId() const { return m_message_id; }

	void SetSignalName(const std::string & signal_name)
	{
		m_signal_name = signal_name;
	}
	const std::string & SignalName() const { return m_signal_name; }

	void SetCharString(const std::string & char_string)
	{
		m_char_string = char_string;
	}
	const std::string & CharString() const { return m_char_string; }
private:
	uint32_t m_message_id;//<message id
	std::string m_signal_name;//<signal name
	std::string m_char_string;//<char string
};

std::ostream & operator<<(std::ostream & os, const Comment & comment);

#endif