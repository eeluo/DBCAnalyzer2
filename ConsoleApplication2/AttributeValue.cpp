#include "stdafx.h"
#include "AttributeValue.h"

/**
*  @brief output class attributevalue
*  @param an ostream object
*  @param class attributevalue
*  @return an ostream object
*/
std::ostream & operator<<(std::ostream & os, const AttributeValue & attribute_value)
{
	os << "Attribute value's name: " << attribute_value.Name();
	switch (attribute_value.ObjectType())
	{
	case AttributeValue::_:
		os <<"\tAttribute value's value: " << attribute_value.Value();
		break;
	case AttributeValue::BU_:
		os << "\tBU_" << " Attribute value's node name: " << attribute_value.NodeName() <<
			"\tAttribute value's value: " << attribute_value.Value();
		break;
	case AttributeValue::BO_:
		os << "\tB0_" << " Attribute value's message id: " << attribute_value.MessageId() <<
			"\tAttribute value's value: " << attribute_value.Value();
		break;
	case AttributeValue::SG_:
		os << "\tSG_" << " Attribute value's message id: " << attribute_value.MessageId() <<
			"\tAttribute value's signal name: " << attribute_value.SignalName() <<
			"\tAttribute value's value: " << attribute_value.Value();
		break;
	case AttributeValue::EV_:
		os << "\tEV_" << " Attribute value's env_var_name: " << attribute_value.EnvVarName() <<
			"\tAttribute value's value: " << attribute_value.Value();
		break;
		//NO default
	}
	return os;
}