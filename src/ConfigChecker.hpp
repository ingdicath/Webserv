//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#pragma once

#include <string>

class ConfigChecker {
public:

	ConfigChecker(std::string filePath);

	~ConfigChecker();

private:
	std::string _filePath;
	std::string _fileData;

	void _readFile();
	void _excludeCommentsEmptyLines();
	void _areCurlyBracesPresent();
	void _

};
