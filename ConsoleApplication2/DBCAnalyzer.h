#pragma once

#include "Message.h"
#include "Comment.h"
#include "AttributeValue.h"
#include "SignalValue.h"
#include "AttributeAnalyzer.h"
#include "NodeAnalyzer.h"

class DBCFileDescriptor {
public:
	// 
	void AddMessage(Message const & _msg) {
		m_messages.push_back(_msg);
	}
	std::vector<Message> & Messages(void) {
		return m_messages;
	}

	std::vector<Message> const & Messages(void) const {
		return m_messages;
	}
	///search for  signal's receiver
	uint32_t SignalReceiversSearch(const std::string & receiver, std::vector<Signal> & vs)const;
	uint32_t SignalReceiversSearch(const std::string & receiver, bool output = false, std::ostream & os = std::cout)const;

	bool MessageIdSearch(uint32_t id, bool output = false, std::ostream & os = std::cout)const;
	bool MessageIdSearch(uint32_t id, Message & vc)const;

	bool MessageNameSearch(const std::string & name, bool output = false, std::ostream & os = std::cout);
	bool MessageNameSearch(const std::string & name, Message & vc)const;

	uint32_t MessageTransmitterSearch(const std::string & name, bool output = false, std::ostream & os = std::cout)const;
	uint32_t MessageTransmitterSearch(const std::string & name, std::vector<Message> & vm)const;

	void AddComment(const Comment & _cmt)
	{
		m_comments.push_back(_cmt);
	}
	std::vector<Comment> & Comments(){ return m_comments; }
	const std::vector<Comment> & Comments() const { return m_comments; }

	///search for comment's message id,the second parameter will contain these class
	uint32_t CommentMessageIdSearch(uint32_t id, bool output = false, std::ostream & os = std::cout)const;
	uint32_t CommentMessageIdSearch(uint32_t id, std::vector<Comment> & vc)const;

	void AddAttributeValue(const AttributeValue & _atv)
	{
		m_attributevalues.push_back(_atv);
	}
	std::vector<AttributeValue> & AttributeValues(){ return m_attributevalues; }
	const std::vector<AttributeValue> & AttributeValues() const { return m_attributevalues; }

	///search for attribute_value's object type,the second parameter will contain these class
	uint32_t AttributeValueObjectTypeSearch(AttributeValue::_ObjectType ot, bool output = false, std::ostream & os = std::cout)const;
	uint32_t AttributeValueObjectTypeSearch(AttributeValue::_ObjectType ot, std::vector<AttributeValue> & va)const;

	void AddSignalValue(const SignalValue & _sv)
	{
		m_signalvalues.push_back(_sv);
	}
	std::vector<SignalValue> & SignalValues(){ return m_signalvalues; }
	const std::vector<SignalValue> & SignalValues() const { return m_signalvalues; }

	uint32_t SignalValueMessageIdSearch(uint32_t id, bool output = false, std::ostream & os = std::cout)const;
	uint32_t SignalValueMessageIdSearch(uint32_t id, std::vector<SignalValue> & vs)const;

	void AddAttribute(Attribute const & _attribute) {
		m_attributes.push_back(_attribute);
	}
	std::vector<Attribute> & Attributes(void) {
		return m_attributes;
	}
	std::vector<Attribute> const & Attributes(void) const {
		return m_attributes;
	}
	///search for attribute_definitoin's attribute_name,the second parameter will contain these class
	bool AttributeNameSearch(const std::string & name, bool output = false, std::ostream & os = std::cout) const;
	bool AttributeNameSearch(const std::string & name, Attribute & at) const;

	///search for attribute_definitoin's object type,the second parameter will contain these class
	uint32_t AttValueTypeSearch(Attribute::VALUE_TYPE ot, bool output = false, std::ostream & os = std::cout) const;
	uint32_t AttValueTypeSearch(Attribute::VALUE_TYPE ot, std::vector<Attribute> & va) const;

	void AddNode(Node const & _nodes) {
		m_nodes.push_back(_nodes);
	}
	std::vector<Node> & Nodes(void) {
		return m_nodes;
	}
	std::vector<Node> const & Nodes(void) const {
		return m_nodes;
	}

	///output
	void PrintNodes(std::ostream & os)const;
	void PrintMessages(std::ostream & os)const;
	void PrintComments(std::ostream & os)const;
	void PrintAttributes(std::ostream & os)const;
	void PrintAttributeValues(std::ostream & os)const;
	void PrintSignalValues(std::ostream & os)const;
	void PrintDescriptor(std::ostream & os)const;
	
private:

	std::vector<Message> m_messages;//<messages container include messages

	std::vector<Comment> m_comments;//<comments container include comment
	std::vector<AttributeValue> m_attributevalues;//< attributevalues container include attributevlaue
	std::vector<SignalValue> m_signalvalues;//<signalvalues container include signalvalue
	std::vector<Attribute> m_attributes;//< attributes container include attribute
	std::vector<Node> m_nodes;//< nodes container include node
};


// @brief input a dbc file, output a DBCFileDescriptor for DBC
//        DBC Ref to : <DBC-File-Format-Documentation.pdf>
//        Analyzer file by Regular Expression.
//
class DBCAnalyzer
{
public:
	DBCAnalyzer();
	~DBCAnalyzer();


public:
	DBCFileDescriptor Analyze(std::string const & _file_name);


private:
	void AnalyzerDBCByLines(std::vector<std::string> const & _lines, DBCFileDescriptor & _file_descriptor);

	bool MessageRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor);

	bool CommentRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor);
	bool AttributeValueRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor);
	bool SignalValueRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor);
	/**@brief Analyze the attribute section.*/
	bool AttributeRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor);
	/**@brief Analyze the node section.*/
	bool NodeRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor);

	bool SignalRecognizer(std::string const & _line, Message & _msg);

	uint8_t ChangMotorolaOrderMSBT2LSB(uint8_t start_bit, uint8_t signal_size);
};