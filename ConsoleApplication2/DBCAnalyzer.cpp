// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "DBCAnalyzer.h"

#include <regex>
#include <fstream>
#include <Windows.h>

DBCAnalyzer::DBCAnalyzer()
{
}


DBCAnalyzer::~DBCAnalyzer()
{
}

DBCFileDescriptor DBCAnalyzer::Analyze(std::string const & _file_name)
{
	DBCFileDescriptor file_descriptor;

	std::ifstream in_file(_file_name);
	if (!in_file.is_open())
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		std::cout << "Error: Can't open this file!" << std::endl;
		return file_descriptor;
	}
	std::vector<std::string> file_lines;
	std::string line;
	while (std::getline(in_file, line)) {
		file_lines.push_back(line);
	}
	AnalyzerDBCByLines(file_lines, file_descriptor);

	return std::move(file_descriptor);
}
/**
*  @brief Analyzer a lines
*/
void DBCAnalyzer::AnalyzerDBCByLines(std::vector<std::string> const & _lines, DBCFileDescriptor & _file_descriptor)
{
	for (auto iter = _lines.begin(); iter != _lines.end(); ++iter)
	{
		if (NodeRecognizer(*iter, _file_descriptor))///Recognizer node
		{
			;///do nothing
		}
		else if (MessageRecognizer(*iter, _file_descriptor))///Recognizer message
		{
			auto & msg = _file_descriptor.Messages().back();
			++iter;
			while (iter != _lines.end() && SignalRecognizer(*iter, msg))///Recognizer signal(from message)
			{
				++iter;
			}
			--iter;
		}
		else if (CommentRecognizer(*iter, _file_descriptor))///Recognizer comment
		{
			;///do nothing
		}
		else if (AttributeRecognizer(*iter, _file_descriptor))///Recognizer attribute
		{
			;///do nothing
		}
		else if (AttributeValueRecognizer(*iter, _file_descriptor))///Recognizer attribute value
		{
			;///do nothing
		}
		else if (SignalValueRecognizer(*iter, _file_descriptor))///Recognizer siganl value
		{
			;///do nothing
		}
	}
}

/**
*  @brief The AttributeRecognizer function determines whether the string of
*         the attribute definitions section conforms to the specification.
*
*  @author luoaling
*
*  AttributeRecognizer function uses regular expressions to check the attribute definitions
*  of the string, in order to achieve the purpose of testing, regular expression matching string
*  is relatively simple, while the use of regular expression matching the default packet order,
*  to the corresponding Attribute class object assignment, and finally add a container that stores an Attribute type.
*
*  @param[in] _lines _lines is a row in the attribute definitions section of the string.
*  @param[in] _file_descriptor _file_descriptor is formed by the corresponding file descriptor.
*
*  @return Returns a Boolean value that indicates whether the string
*          conforms to the specification and conforms to the corresponding file descriptor.
*
*  @note The Attribute class contains only the attribute definition part and
*        does not contain the attribute value, which consists of two parts：
*        attribute_definition and attribute_default.
*
*/
bool DBCAnalyzer::AttributeRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor) {
	/**
	attribute_definitions = { attribute_definition } ;
	attribute_definition = 'BA_DEF_' object_type attribute_name attribute_value_type ';';
	object_type = '' | 'BU_' | 'BO_' | 'SG_' | 'EV_' ;
	attribute_name = '"' C_identifier '"' ;
	attribute_value_type = 'INT' signed_integer signed_integer |
	'HEX' signed_integer signed_integer |
	'FLOAT' double double |
	'STRING' |
	'ENUM'[char_string{ ',' char_string }]

	attribute_defaults = { attribute_default } ;
	attribute_default = 'BA_DEF_DEF_' attribute_name attribute_value ';';
	attribute_value = unsigned_integer | signed_integer | double | char_string;
	*/

	///  This is a regular expression for the attribute_definition section.

	///  Since the attribute_value_type variable is more complex, the regular expression
	///  is separated here so that the regular expression can be matched for a variety of situations.

	///  Matches the regular expression of the string except the attribute_value_type part.
	std::regex raw_definition(R"(BA_DEF_\s(BU_|BO_|SG_|EV_|\s?)\s+(\"([^"]*[a-zA-Z_](\w*))\")\s+(.*);)");
	///  Write the five regular expressions of the attribute_value_type section, respectively.
	std::regex int_definition(R"(INT\s+([-]?\d+)\s+([-]?\d+))");
	std::regex hex_definition(R"(HEX\s+([-]?\d+)\s+([-]?\d+))");
	std::regex float_definition(R"(FLOAT\s+([-]?\d+(\.\d+)?)\s+([-]?\d+(\.\d+)?))");
	std::regex string_definition(R"(STRING\s+)");
	std::regex enum_definition(R"(ENUM\s+(\"([^"]*)\")(,\"([^"]*)\")*)");
	///  This is a regular expression for the attribute_default section.
	std::regex raw_default(R"(BA_DEF_DEF_\s+(\"([^"]*[a-zA-Z_](\w*))\")\s+(\d+|[-]?\d+|[-]?\d+(\.\d+)?|\"([^"]*)\");)");
	///  m0 stores the matching result for attribute_definition, m1 stores the result of attribute_default.
	std::smatch m0, m1;
	if (!(std::regex_match(_line, m0, raw_definition) || std::regex_match(_line, m1, raw_default))) {
		return false;
	}
	///  The five cases of the attribute_value_type part are respectively regular expression matching.
	bool enum_int = std::regex_match(m0[5].str(), int_definition);
	bool enum_hex = std::regex_match(m0[5].str(), hex_definition);
	bool enum_float = std::regex_match(m0[5].str(), float_definition);
	bool enum_str = std::regex_match(m0[5].str(), string_definition);
	bool enum_enum = std::regex_match(m0[5].str(), enum_definition);

	if (!(enum_int | enum_hex | enum_float | enum_str | enum_enum | std::regex_match(_line, m1, raw_default))) {
		return false;
	}

	Attribute att;
	///  Assign a value to the attribute_default part of the variable.
	if (std::regex_match(_line, m1, raw_default)) {
		att.SetAttributeName(m1[1].str());
		att.SetDefaultValue(m1[4].str());
	}
	///  Partial variables for the attribute_definition assignment, also divided into five cases to assign values.
	else {
		att.SetObjType(m0[1].str());
		att.SetAttributeName(m0[2].str());
		if (enum_int) {
			att.SetValueType(Attribute::INT);
			std::string delim = ", ";///  Separate the string with spaces and commas as delimiters. 
			att.AddValueType(split<std::vector<std::string>>(m0[5].str(), delim));
		}
		if (enum_hex) {
			att.SetValueType(Attribute::HEX);
			std::string delim = ", ";
			att.AddValueType(split<std::vector<std::string>>(m0[5].str(), delim));
		}
		if (enum_float) {
			att.SetValueType(Attribute::FLOAT);
			std::string delim = ", ";
			att.AddValueType(split<std::vector<std::string>>(m0[5].str(), delim));
		}
		if (enum_enum) {
			att.SetValueType(Attribute::ENUM);
			std::string delim = ", ";
			att.AddValueType(split<std::vector<std::string>>(m0[5].str(), delim));
		}
		if (enum_str) {
			att.SetValueType(Attribute::STRING);
			std::vector<std::string> vec_str;
			vec_str.push_back(m0[5].str());
			att.AddValueType(vec_str);
		}
	}
	_file_descriptor.AddAttribute(att);
	return true;
}

/**
*  @brief The NodeRecognizer function determines whether the string of
*         the node section conforms to the specification.
*
*  @author luoaling
*
*  NodeRecognizer function uses a regular expression to check the node of the string,
*  in order to achieve the purpose of testing, regular expression matching string
*  is relatively simple, while the use of regular expression matching the default packet order,
*  to the corresponding Node class object assignment,and the finally add a container that stores an Node type.
*
*  @param[in] _lines _lines is a row in the node part of the string.
*  @param[in] _file_descriptor _file_descriptor is formed by the corresponding file descriptor.
*
*  @return Returns a Boolean value that indicates whether the string
*          conforms to the specification and conforms to the corresponding file descriptor.
*
*/
bool DBCAnalyzer::NodeRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor) {
	/**
	nodes = 'BU_:' {node_name} ;
	node_name = C_identifier;
	*/
	std::regex node_definition(R"(BU_:\s+([a-zA-Z_][ \w]+).*)");
	std::smatch m;
	if (!std::regex_match(_line, m, node_definition)) { return false; }

	Node node;
	std::string delim = " ";///  Separate the string with a space as a delimiter.
	std::vector<std::string> vs;
	SPILT(m[1].str(), ' ', vs);
	node.AddNodeName(vs);

	_file_descriptor.AddNode(node);

	return true;
}

/**
*  signal_value = value_descriptions_for_signal
*  value_descriptions_for_signal = 'VAL_' message_id signal_name { value_description } ';';
*  @param a line of string
*  @param class file_description
*  @return match or not match
*/
bool DBCAnalyzer::SignalValueRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor)
{
	std::smatch m;
	if (!std::regex_match(_line, m, std::regex(R"(VAL_\s+(\d+)\s+(\w+)\s+([\s\w"]+);)")))
	{
		return false;
	}
	SignalValue sv;
	sv.SetMessageId(std::stoul(m[1].str()));
	sv.SetSignalName(m[2].str());

	///Set value_description
	std::vector<uint32_t> vi;
	std::vector<std::string> vs;
	split_number_and_string(m[3].str(), vi, vs);
	for (auto i = 0u; i < vi.size(); i++)
	{
		sv.AddValueDescription(vi[i], vs[i]);
	}

	_file_descriptor.AddSignalValue(sv);
	return true;
}

/**
*  attribute_values = { attribute_value_for_object };
*  attribute_value_for_object = 'BA_' attribute_name
*  (attribute_value |
*  'BU_' node_name attribute_value |
*  'BO_' message_id attribute_value |
*  'SG_' message_id signal_name attribute_value |
*  'EV_' env_var_name attribute_value)
*  @param a line of string
*  @param class file_description
*  @return match or not match
*/
bool DBCAnalyzer::AttributeValueRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor)
{
	std::smatch m;

	///read attribute_value and set attribute_name
	if (!std::regex_match(_line, m, std::regex(R"(BA_\s+(\"[-\w]*\")\s+(.*))")))
	{
		return false;
	}
	AttributeValue atv;
	atv.SetName(m[1].str());

	///deal with last attribute_value,which have five object_types
	std::string back_str(m[2].str());
	if (std::regex_match(back_str, m, std::regex(R"((\d+\.*\d*)\s*;.*)")))///only attribute_value
	{
		atv.SetValue(std::stod(m[1].str()));
		atv.SetObjectType(AttributeValue::_);
		_file_descriptor.AddAttributeValue(atv);
		return true;
	}
	else if (std::regex_match(back_str, m, std::regex(R"(BU_\s+(\w+)\s+(\d+\.*\d*)\s*;.*)")))///'BU_' node_name attribute_value
	{
		atv.SetNodeName(m[1].str());
		atv.SetValue(std::stod(m[2].str()));
		atv.SetObjectType(AttributeValue::BU_);
		_file_descriptor.AddAttributeValue(atv);
		return true;
	}
	else if (std::regex_match(back_str, m, std::regex(R"(BO_\s+(\d+)\s+(\d+\.*\d*)\s*;.*)")))///'BO_' message_id attribute_value
	{
		atv.SetMessageId(std::stoul(m[1].str()));
		atv.SetValue(std::stod(m[2].str()));
		atv.SetObjectType(AttributeValue::BO_);
		_file_descriptor.AddAttributeValue(atv);
		return true;
	}
	else if (std::regex_match(back_str, m, std::regex(R"(SG_\s+(\d+)\s+(\w+)\s+(\d+\.*\d*)\s*;.*)")))///'SG_' message_id signal_name attribute_value
	{
		atv.SetMessageId(std::stol(m[1].str()));
		atv.SetSignalName(m[2].str());
		atv.SetValue(std::stod(m[3].str()));
		atv.SetObjectType(AttributeValue::SG_);
		_file_descriptor.AddAttributeValue(atv);
		return true;
	}
	else if (std::regex_match(back_str, m, std::regex(R"(EV_\s+(\w+)\s+(\d+\.*\d*)\s*;.*)")))///'EV_' env_var_name attribute_value
	{
		atv.SetEnvVarName(m[1].str());
		atv.SetValue(std::stod(m[2].str()));
		atv.SetObjectType(AttributeValue::EV_);
		_file_descriptor.AddAttributeValue(atv);
		return true;
	}
	else
	{
		return false;
	}
}

/**
*  comments = {comment} ;
*  comment = 'CM_' (char_string |
*  'BU_' node_name char_string |
*  'BO_' message_id char_string |
*  'SG_' message_id signal_name char_string |
*  'EV_' env_var_name char_string)
*  ';'
*  But in this file,it only has "'CM_' 'SG_' message_id signal_name char_string ';'"
*  @param a line of string
*  @param class file_description
*  @return match or not match
*/
bool DBCAnalyzer::CommentRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor)
{
	std::regex comment_definition(R"(CM_\s+SG_\s+(\d+)\s+(\w+)\s+(\"[ \w+]+\")\s*;.*)");
	std::smatch m;
	if (!std::regex_match(_line, m, comment_definition))
	{
		return false;
	}
	Comment cmt;
	cmt.SetMessageId(std::stoul(m[1].str()));
	cmt.SetSignalName(m[2].str());
	cmt.SetCharString(m[3].str());
	_file_descriptor.AddComment(cmt);
	return true;
}

/**
*  message = BO_ message_id message_name ':' message_size transmitter{ signal };
*  @param a line of string
*  @param class file_description
*  @return match or not match
*/
bool DBCAnalyzer::MessageRecognizer(std::string const & _line, DBCFileDescriptor & _file_descriptor)
{
	std::regex message_definition(R"(BO_\s+(\d+)\s+(\w+)\s*:\s+(\w+)\s+(\w+).*)");
	std::smatch m;
	if (!std::regex_match(_line, m, message_definition))
	{
		return false;
	}
	Message msg;
	msg.SetMessageName(m[2].str());
	msg.SetMsgID(std::stoul(m[1].str()));

	msg.SetMsgSize(std::stoul(m[3].str()));
	msg.SetMsgtransmitter(m[4].str());

	_file_descriptor.AddMessage(msg);
	return true;
}

/**
*  signal = 'SG_' signal_name multiplexer_indicator ':' start_bit '|'
*  signal_size '@' byte_order value_type '(' factor ',' offset ')'
*  '[' minimum '|' maximum ']' unit receiver{ ',' receiver };
*  @param a line of string
*  @param class file_description
*  @return match or not match
*/
bool DBCAnalyzer::SignalRecognizer(std::string const & _line, Message & _msg)
{
	std::regex signal_definition(R"(\s*SG_\s+(\w+)\s+:\s+(\d+)\|(\d+)@(\d)(\+|-)\s+\((\d+\.*\d*),(\d+\.*\d*)\)\s+\[(\d+\.*\d*)\|(\d+\.*\d*)\]\s+(\"\w*\")\s+(\S+).*)");
	std::smatch m;
	if (!std::regex_match(_line, m, signal_definition)) {
		return false;
	}

	Signal signal;
	signal.SetName(m[1].str());
	/**1 - little endian (Intel)
	 * 0 - big endian (Motorola)
	 * @attention : this is different from DBC-File-Format-Documentation.pdf
	 *              chapter 8.1 Signal Definition
	 *              But matches the real behavior of CANdb++
	 */
	signal.SetIsBigEndian(m[4].str() == "0");//0 or 1?

	signal.SetIsUnsigned(m[5].str() == "+");

	uint8_t start_bit = static_cast<uint8_t>(std::stoul(m[2].str()));   // Attention! start_bit should be : LSB position
	uint8_t signal_size = static_cast<uint8_t>(std::stoul(m[3].str()));
	if (signal.IsBigEndian()) {
		start_bit = ChangMotorolaOrderMSBT2LSB(start_bit, signal_size);
	}
	signal.SetBitInfo(start_bit, signal_size);

	signal.SetFactor(std::stod(m[6].str()));
	signal.SetOffset(std::stod(m[7].str()));
	signal.SetMinimum(std::stod(m[8].str()));
	signal.SetMaximum(std::stod(m[9].str()));
	signal.SetUnit(m[10].str());

	//Set receivers
	std::vector<std::string> vs;
	SPILT(m[11].str(), ',', vs);
	signal.SetReceivers(vs);
	
	_msg.AddSignal(signal);

	return true;
}

uint8_t DBCAnalyzer::ChangMotorolaOrderMSBT2LSB(uint8_t start_bit, uint8_t signal_size) {
	while (--signal_size) {
		if (0 == (start_bit % 8)) {
			start_bit += 15;
		}
		else {
			start_bit--;
		}
	}
	return start_bit;
}

/**
*  @brief output nodes
*  @param output position
*  @return void
*/
void DBCFileDescriptor::PrintNodes(std::ostream & os) const
{
	for (auto iter = Nodes().begin(); iter < Nodes().end(); iter++)
	{
		os << *iter << '\n';
	}
}

/**
*  @brief output attributes
*  @param output position
*  @return void
*/
void DBCFileDescriptor::PrintAttributes(std::ostream & os) const
{
	for (auto iter = Attributes().begin(); iter < Attributes().end(); iter++)
	{
		os << *iter << '\n';
	}
}

/**
*  @brief output messages
*  @param output position
*  @return void
*/
void DBCFileDescriptor::PrintMessages(std::ostream & os) const
{
	for (auto iter = Messages().begin(); iter < Messages().end(); iter++)
	{
		os << *iter << '\n';
	}
}

/**
*  @brief output comments
*  @param output position
*  @return void
*/
void DBCFileDescriptor::PrintComments(std::ostream & os) const
{
	for (auto iter_comment = Comments().begin(); iter_comment < Comments().end(); iter_comment++)
	{
		os << *iter_comment << '\n';
	}
}

/**
*  @brief output attribute values
*  @param output position
*  @return void
*/
void DBCFileDescriptor::PrintAttributeValues(std::ostream & os) const
{
	for (auto iter_attribute_value = AttributeValues().begin(); iter_attribute_value < AttributeValues().end(); iter_attribute_value++)
	{
		os << *iter_attribute_value<<'\n';
	}
}

/**
*  @brief output signalvalues
*  @param output position
*  @return void
*/
void DBCFileDescriptor::PrintSignalValues(std::ostream & os) const
{
	for (auto iter_signal_value = SignalValues().begin(); iter_signal_value < SignalValues().end(); iter_signal_value++)
	{
		os << *iter_signal_value << '\n';
	}
}

/**
*  @brief output all descriptor
*  @param output position
*  @return void
*/
void DBCFileDescriptor::PrintDescriptor(std::ostream & os) const
{
	PrintNodes(os);
	PrintMessages(os);
	PrintComments(os);
	PrintAttributes(os);
	PrintAttributeValues(os);
	PrintSignalValues(os);
}

/**
*  @brief search for attribute value's object type
*  @param attribute value's object type
*  @param attribute values which have this object type
*  @return The number of attributevalues which have the object type
*/
uint32_t DBCFileDescriptor::AttributeValueObjectTypeSearch(AttributeValue::_ObjectType ot, std::vector<AttributeValue> & va) const
{
	va.clear();
	uint32_t num = 0;
	for (auto iter = m_attributevalues.begin(); iter < m_attributevalues.end(); iter++)
	{
		if (iter->ObjectType() == ot)
		{
			num++;
			va.push_back(*iter);
		}
	}
	return num;
}

/**
*  @brief search for attribute value's object type
*  @param attribute value's object type
*  @param output or not output
*  @param output position
*  @return The number of attributevalues which have the object type
*/
uint32_t DBCFileDescriptor::AttributeValueObjectTypeSearch(AttributeValue::_ObjectType ot, bool output, std::ostream & os) const
{
	uint32_t num = 0;
	for (auto iter = m_attributevalues.begin(); iter < m_attributevalues.end(); iter++)
	{
		if (iter->ObjectType() == ot)
		{
			num++;
			if (output)
			{
				os << *iter << '\n';
			}
		}
	}
	return num;
}

/**
*  @brief search for signal's receiver
*  @param signal's receiver
*  @param the signals which have this receiver
*  @return The number of receivers which have the receiver.
*/
uint32_t DBCFileDescriptor::SignalReceiversSearch(const std::string & receiver,std::vector<Signal> & vs) const
{
	vs.clear();
	int num = 0;
	for (auto iter_message = Messages().begin(); iter_message < Messages().end(); iter_message++)
	{
		for (auto iter_signal = iter_message->Signals().begin(); iter_signal < iter_message->Signals().end(); iter_signal++)
		{
			for (auto iter_receiver = iter_signal->Receivers().begin(); iter_receiver < iter_signal->Receivers().end(); iter_receiver++)
			{
				if (*iter_receiver == receiver)
				{
					num++;
					vs.push_back(*iter_signal);
					break;
				}
			}
		}
	}
	return num;
}

/**
*  @brief search for signal's receiver
*  @param signal's receiver
*  @param output or not output
*  @param output position
*  @return The number of receivers which have the receiver.
*/
uint32_t DBCFileDescriptor::SignalReceiversSearch(const std::string & receiver, bool output, std::ostream & os) const
{
	int num = 0;
	for (auto iter_message = Messages().begin(); iter_message < Messages().end(); iter_message++)
	{
		for (auto iter_signal = iter_message->Signals().begin(); iter_signal < iter_message->Signals().end(); iter_signal++)
		{
			for (auto iter_receiver = iter_signal->Receivers().begin(); iter_receiver < iter_signal->Receivers().end(); iter_receiver++)
			{
				if (*iter_receiver == receiver)
				{
					num++;
					if (output)
					{
						os << *iter_signal << '\n';
					}
					break;
				}
			}
		}
	}
	return num;
}

/**
*  @brief search for message ID
*  @param message ID
*  @param output or not output
*  @param output position
*  @return have or not have
*/
bool DBCFileDescriptor::MessageIdSearch(uint32_t id, bool output, std::ostream & os)const
{
	for (auto iter = Messages().begin(); iter < Messages().end(); iter++)
	{
		if (iter->ID() == id)
		{
			if (output)
			{
				os << *iter << '\n';
			}
			return true;
		}
	}
	return false;
}

/**
*  @brief search for message ID
*  @param message ID
*  @param the message which has this ID
*  @return have or not have
*/
bool DBCFileDescriptor::MessageIdSearch(uint32_t id, Message & msg) const
{
	for (auto iter = Messages().begin(); iter < Messages().end(); iter++)
	{
		if (iter->ID() == id)
		{
			msg = *iter;
			return true;
		}
	}
	return false;
}

/**
*  @brief search for message name
*  @param message name
*  @param output or not output
*  @param output position
*  @return have or not have
*/
bool DBCFileDescriptor::MessageNameSearch(const std::string & name, bool output, std::ostream & os)
{
	for (auto iter = Messages().begin(); iter < Messages().end(); iter++)
	{
		if (iter->Name() == name)
		{
			if (output)
			{
				os << *iter << '\n';
			}
			return true;
		}
	}
	return false;
}

/**
*  @brief search for message name
*  @param message name
*  @param the message which has this name
*  @return have or not have
*/
bool DBCFileDescriptor::MessageNameSearch(const std::string & name, Message & msg) const
{
	for (auto iter = Messages().begin(); iter < Messages().end(); iter++)
	{
		if (iter->Name() == name)
		{
			msg = *iter;
			return true;
		}
	}
	return false;
}

/**
*  @brief search for comment's message ID
*  @param comment's message ID
*  @param output or not output
*  @return The number of comments which have the id.
*/
uint32_t DBCFileDescriptor::CommentMessageIdSearch(uint32_t id, bool output, std::ostream & os) const
{
	uint32_t num = 0;
	for (auto iter = Comments().begin(); iter < Comments().end(); iter++)
	{
		if (iter->MessageId() == id)
		{
			num++;
			if (output)
			{
				os << *iter << '\n';
			}
		}
	}
	return num;
}

/**
*  @brief search for comment's message ID
*  @param comment's message ID
*  @param the comments which have this ID
*  @return The number of comments which have the id.
*/
uint32_t DBCFileDescriptor::CommentMessageIdSearch(uint32_t id, std::vector<Comment> & vc) const
{
	vc.clear();
	uint32_t num = 0;
	for (auto iter = Comments().begin(); iter < Comments().end(); iter++)
	{
		if (iter->MessageId() == id)
		{
			num++;
			vc.push_back(*iter);
		}
	}
	return num;
}

/**
*  @brief search for message transmitter
*  @param message transmitter
*  @param output or not output
*  @param output position
*  @return The number of messages which have the transimmter.
*/
uint32_t DBCFileDescriptor::MessageTransmitterSearch(const std::string & name, bool output, std::ostream & os) const
{
	uint32_t num = 0;
	for (auto iter = Messages().begin(); iter < Messages().end(); iter++)
	{
		if (iter->Msgtransmitter() == name)
		{
			num++;
			if (output)
			{
				os << *iter << '\n';
			}
		}
	}
	return num;
}

/**
*  @brief search for message transmitter
*  @param message transmitter
*  @param the messages which have this transmitter
*  @return The number of messages which have the transimmter.
*/
uint32_t DBCFileDescriptor::MessageTransmitterSearch(const std::string & name, std::vector<Message> & vm) const
{
	vm.clear();
	uint32_t num = 0;
	for (auto iter = Messages().begin(); iter < Messages().end(); iter++)
	{
		if (iter->Msgtransmitter() == name)
		{
			num++;
			vm.push_back(*iter);
		}
	}
	return num;
}

/**
*  @brief search for signal value's message ID
*  @param signal value's message ID
*  @param ouput or not output
*  @param ouput position 
*  @return The number of signalvalues which have the id.
*/
uint32_t DBCFileDescriptor::SignalValueMessageIdSearch(uint32_t id, bool output, std::ostream & os) const
{
	uint32_t num = 0;
	for (auto iter = SignalValues().begin(); iter < SignalValues().end(); iter++)
	{
		if (iter->MessageId() == id)
		{
			num++;
			if (output)
			{
				os << *iter << '\n';
			}
		}
	}
	return num;
}

/**
*  @brief search for signal value's message ID
*  @param signal value's message ID
*  @param the signalvalues which have this ID
*  @return The number of signalvalues which have the id.
*/
uint32_t DBCFileDescriptor::SignalValueMessageIdSearch(uint32_t id, std::vector<SignalValue> & vs) const
{
	vs.clear();
	uint32_t num = 0;
	for (auto iter = SignalValues().begin(); iter < SignalValues().end(); iter++)
	{
		if (iter->MessageId() == id)
		{
			num++;
			vs.push_back(*iter);
		}
	}
	return num;
}

/**
*  @brief Find whether there have any attributes with this attribute_name.
*  @param[in] name attribute_name.
*  @param[in] output output or not output.
*  @param[out] os  output position.
*  @return whether or not to find the attributes with this attribute_name.
*/
bool DBCFileDescriptor::AttributeNameSearch(const std::string & name, bool output, std::ostream & os) const
{
	for (auto iter = Attributes().begin(); iter < Attributes().end(); iter++)
	{
		if (iter->AttributeName() == name)
		{
			if (output)
			{
				os << *iter << '\n';
			}
			return true;
		}
	}
	return false;
}
/**
*  @brief Find whether there have any attributes with this attribute_name.
*  @param[in] name the attribute_name.
*  @param[out]  the attribute which has this attribute_name.
*  @return whether or not to find the attributes with this attribute_name.
*/
bool DBCFileDescriptor::AttributeNameSearch(const std::string & name, Attribute & at) const
{
	for (auto iter = Attributes().begin(); iter < Attributes().end(); iter++)
	{
		if (iter->AttributeName() == name)
		{
			at = *iter;
			return true;
		}
	}
	return false;
}

/**
*  @brief whether there have any attributes with this value_type.
*  @param[in] vt value_type.
*  @param[in] output output or not output.
*  @param[out] os  output position.
*  @return whether or not to find the attributes with this value_type.
*/
uint32_t DBCFileDescriptor::AttValueTypeSearch(Attribute::VALUE_TYPE vt, bool output, std::ostream & os) const {
	uint32_t num = 0;
	for (auto iter = m_attributes.begin(); iter < m_attributes.end(); iter++)
	{
		if (iter->GetValueType() == vt)
		{
			num++;
			if (output)
			{
				os << *iter << '\n';
			}
		}
	}
	return num;
}
/**
*  @brief Find whether there have any attributes with this value_type.
*  @param[in] vt the value_type.
*  @param[out]  the attribute which has this value_type.
*  @return whether or not to find the attributes with this value_type.
*/
uint32_t DBCFileDescriptor::AttValueTypeSearch(Attribute::VALUE_TYPE vt, std::vector<Attribute> & va) const {
	uint32_t num = 0;
	for (auto iter = m_attributes.begin(); iter < m_attributes.end(); iter++)
	{
		if (iter->GetValueType() == vt)
		{
			num++;
			va.push_back(*iter);
		}
	}
	return num;
}