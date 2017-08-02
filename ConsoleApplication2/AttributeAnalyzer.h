/**
*  @file
*  @brief Defines an Attribute class.
*  @author luoaling
*  @date 2017-7-27
*/
#pragma once

#ifndef _ATTRIBUTEANALYZER_H_
#define _ATTRIBUTEANALYZER_H_

#include <string>
#include <vector>
#include <cstdint>

/**
*  @class
*  @biref User-defined attributes are used to extend the properties of DBC file objects.
*
*  The Attribute class contains only the attribute definition part and
*  does not contain the attribute value, which consists of two parts:
*  1.attribute_definition£ºIt also contains object_type, attribute_name,and attribute_value_type three parts.
*  2.attribute_default£ºIt contains only attribute_name attribute_value.
*
*/

class Attribute {

public:
	///  Enumeration VALUE_TYPE storages the first part of the attribute_value_type.
	enum VALUE_TYPE {
		INT,
		HEX,
		FLOAT,
		STRING,
		ENUM
	};

	std::string const & ObjType() const { return m_obj_type; }
	void SetObjType(std::string const & _obj_type) {
		m_obj_type = _obj_type;
	}

	std::string const & AttributeName() const { return m_att_name; }
	void SetAttributeName(std::string const & _att_name) {
		m_att_name = _att_name;
	}

	void AddValueType(std::vector<std::string> const & _val_type) {
		m_val_type = _val_type;
	}
	std::vector<std::string> & ValueType(void) {
		return m_val_type;
	}
	std::vector<std::string> const & ValueType(void) const {
		return m_val_type;
	}

	VALUE_TYPE GetValueType(void) const  { return m_value_type; }
	void SetValueType(VALUE_TYPE _value_type) {
		m_value_type = _value_type;
	}

	std::string const & DefaultValue() const { return m_att_def_val; }
	void SetDefaultValue(std::string const & _att_def_val) {
		m_att_def_val = _att_def_val;
	}

private:

	std::string m_obj_type;///< m_obj_type can only be '' | 'BU_' | 'BO_' | 'SG_' | 'EV_' .
	std::string m_att_name;///< m_att_name must be a double-quoted C-identifier.
	std::string m_att_def_val;///< m_att_def_val can only be unsigned_integer | signed_integer | double | char_string .
	std::vector<std::string> m_val_type;///< Used to store the attribute_value_type part of the sub-string after the division.
	VALUE_TYPE m_value_type;///< Enumeration variable definition.

};

std::ostream & operator<<(std::ostream & os, const Attribute & atb);

#endif

