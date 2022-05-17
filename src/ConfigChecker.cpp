//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#include <iostream>
#include <exception>
#include <stdlib.h> //chequear para que es
#include "ConfigChecker.hpp"

ConfigChecker::ConfigChecker(std::string filePath) : _filePath(filePath) {}

ConfigChecker::~ConfigChecker() {}

// You only need to convert it to a c-string if you are not using c++11 or later.
void ConfigChecker::_readFile() {
	std::ifstream file;
	std::string line;

	file.open(file.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Configuration file failed to open");
	}
	while (std::getline(file, line)) {
		_fileData.append(line + "\n");
	}
	file.close()
}

void ConfigChecker::_excludeCommentsEmptyLines() {
	std::istringstream inputStream(_fileData);
	std::string line;
	std::string temp;
	size_t hastagPosition;

	_fileData.clear();
	while (std::getline(inputStream, line)) {
		hastagPosition = line.find('#');
		if (hastagPosition != std::string::npos) {
			line.erase(hastagPosition, std::string::npos);
		}
		tmp = line;
		removeSpace(tmp); //implement, namespace utils??
		if (!line.empty() && !temp.empty()) {
			_fileData.append(line + "\n");
		}
	}
}

void ConfigChecket::_areCurlyBracesPresent() {
	std::stack<char> curlyBraces;

	for (int i = 0; i < _fileData.length(); i++) {
		if (_fileData[i] == '{') {
			curlyBraces.push('{');
			continue;
		}
		if (curlyBraces.empty() && _fileData[i] == '}') {
			throw std::runtime_error("Config error: Wrong curly braces");
		}
		if (_fileData[i] == '}') {
			curlyBraces.pop();
		}
	}
	if (!curlyBraces.empty()) {
		throw std::runtime_error("Config error: Wrong curly braces");
	}
}


}

