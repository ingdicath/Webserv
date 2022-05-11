//
// Created by Diana catherine Salamanca leguizamon on 4/28/22.
//
#pragma once
#include <string>

class Parser {
public:
	Parser();
	virtual ~Parser();

	void getFile(const std::string &configFile);
private:
	std::string _configFileName;
};

