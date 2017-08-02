#pragma once

#ifndef _ATTRIBUTEVALUE_H_
#define _ATTRIBUTEVALUE_H_

#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

//deal with attribute value
class AttributeValue{
public:
	enum _ObjectType{ _, BU_, BO_, SG_, EV_ };//<five types

	void SetName(const std::string & name)
	{
		m_attribute_name = name;
	}
	const std::string & Name() const { return m_attribute_name; }

	void SetValue(double value)
	{
		m_attribute_value = value;
	}
	double Value() const { return m_attribute_value; }

	void SetNodeName(const std::string & node_name)
	{
		m_node_name = node_name;
	}
	const std::string & NodeName() const { return m_node_name; }

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

	void SetEnvVarName(const std::string & env_var_name)
	{
		m_env_var_name = env_var_name;
	}
	const std::string & EnvVarName() const { return m_env_var_name; }

	void SetObjectType(_ObjectType flag)
	{
		m_object_type = flag;
	}
	_ObjectType ObjectType() const { return m_object_type; }
private:
	std::string m_attribute_name;//<attribute name
	double m_attribute_value;//<attribute value
	std::string m_node_name;//<node name
	uint32_t m_message_id;//<message id
	std::string m_signal_name;//<signal value
	std::string m_env_var_name;//<env_var_name
	_ObjectType m_object_type;//<object type
};

std::ostream & operator<<(std::ostream & os, const AttributeValue & attribute_value);

#endif