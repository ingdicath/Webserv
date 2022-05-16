//
// Created by Diana catherine Salamanca leguizamon on 5/16/22.
//

#pragma once

#include <iostream>
#include <string>
#include <map>

/**
 * Parsing blocks inside configuration file
 */

class AConfigBlock {
public:
	AConfigBlock();
	AConfigBlock(const AConfigBlock &obj);
	virtual ~AConfigBlock();

	AConfigBlock &operator=(const AConfigBlock &obj);

protected:
	std::string _root;
	int _clientMaxBodySize;




};



