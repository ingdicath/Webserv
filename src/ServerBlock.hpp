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

class ServerBlock {
public:
	ServerBlock();

	ServerBlock(const ServerBlock &obj);

	virtual ~ServerBlock();

	ServerBlock &operator=(const ServerBlock &obj);

protected:
	std::string _root;
	int _clientMaxBodySize;
};



