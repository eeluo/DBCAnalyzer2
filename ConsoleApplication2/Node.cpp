#include "stdafx.h"
#include "NodeAnalyzer.h"

/**
*  @brief output class node
*  @param an ostream object
*  @param class node
*  @return an ostream object
*/
std::ostream & operator<<(std::ostream & os, const Node & node)
{
	os << "Node names are: ";
	for (auto iter = node.NodeName().begin(); iter < node.NodeName().end(); iter++)
	{
		os << *iter << ' ';
	}
	return os;
}