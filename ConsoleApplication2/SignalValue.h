#pragma once

#ifndef _SIGNALVALUE_H_
#define _SIGNALVALUE_H_

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

void split_number_and_string(const std::string & _Str, std::vector<uint32_t> & _Vi, std::vector<std::string> & _Vs, char c = ' ');

///deal with signal_value
class SignalValue{
public:
	class value_description{
	public:
		value_description(uint32_t value = 0, std::string name = "") :m_value(value), m_name(name){}

		void SetValue(uint32_t value)
		{
			m_value = value;
		}
		uint32_t Value() const { return m_value; }

		void SetName(const std::string & name)
		{
			m_name = name;
		}
		const std::string & Name() const { return m_name; }

	private:
		uint32_t m_value;       //< enumration value
		std::string m_name; //< enumration name
	};
	//
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

	void AddValueDescription(uint32_t num,const std::string & string)
	{
		m_value_description.push_back(value_description(num, string));
		// m_value_description.emplace_back(num, string);
	}
	void SetValueDescription(const std::vector<value_description> & value_description)
	{
		m_value_description = value_description;
	}
	const std::vector<value_description> & ValueDescription() const { return m_value_description; }

private:
	uint32_t m_message_id;//<message id
	std::string m_signal_name;//<signal name
	
	std::vector<value_description> m_value_description;//<value description
};

std::ostream & operator<<(std::ostream & os, const SignalValue & signal_value);

#endif