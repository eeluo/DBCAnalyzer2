/**
@file
*  @brief Defines a Node class.
*  @author luoaling
*  @date 2017-7-29
*
*  In the DBC file, the node part defines the names of all participating nodes.
*  The names defined in this section must be unique in this section.
*
*/
#pragma once

#ifndef _NODEANALYZER_H_
#define _NODEANALYZER_H_

#include <string>
#include <vector>


/**
*  @class
*  @biref The Node class is simpler, with only one vector container variable that stores the string type node_name.
*
*  In this class, use the AddNodeName function to add the node's node_name,
*  and the NodeName function to get the node_name.
*
*/
class Node {

public:

	/**The AddNodeName function is used to assign the vector container,
	and the NodeName function is used to get the container.*/
	void AddNodeName(std::vector<std::string> const & _node_name) {
		m_node_name = _node_name;
	}
	std::vector<std::string> & NodeName(void) {
		return m_node_name;

	}
	std::vector<std::string> const & NodeName(void) const {
		return m_node_name;
	}

private:
	std::vector<std::string> m_node_name;
};

std::ostream & operator<<(std::ostream & os, const Node & node);

#endif