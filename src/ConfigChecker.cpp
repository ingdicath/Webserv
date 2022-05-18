//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> //find_first_of
#include "ConfigChecker.hpp"

ConfigChecker::ConfigChecker(std::string filePath) : _filePath(filePath) {}

ConfigChecker::~ConfigChecker() {}

/**
 * PUT IN ANOTHER FILE
 */

void removeWhiteSpaces(std::string str) {
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

/**
 * Put in another file option 1 for split
 */
std::vector<std::string> splitString2(std::string str, char splitter) {
	std::vector<std::string> res;
	std::string current;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == splitter) {
			if (!current.empty()) {
				res.push_back(current);
				current.clear();
			}
			continue;
		}
		current += str[i];
	}
	if (!current.empty())
		res.push_back(current);
	return res;
}

// https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character

/**
 * Put in another file option 2 for split
 */
std::vector<std::string> splitString(const std::string &str, char splitter) {
	std::vector<std::string> res;
	std::stringstream line(str);
	std::string segment;

	while (std::getline(line, segment, splitter)) {
		res.push_back(segment);
	}
	return res;
}


// You need to convert to a c-string if you are not using c++11 or later.
void ConfigChecker::_readFile() {
	std::ifstream file;
	std::string line;

	file.open(_filePath.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Configuration file failed to open.");
	}
	while (std::getline(file, line)) {
		_fileData.append(line + "\n");
	}
	file.close();
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
		temp = line;
		removeWhiteSpaces(temp); //implement, namespace utils??
		if (!line.empty() && !temp.empty()) {
			_fileData.append(line + "\n");
		}
	}
}

void ConfigChecker::_checkCurlyBraces() {
	std::stack<char> curlyBraces;

	for (size_t i = 0; i < _fileData.length(); i++) {
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

void ConfigChecker::_checkSemiColon(const std::string &line) {
	size_t size;
	std::string temp = line;
	removeWhiteSpaces(temp);
	size_t firstPos = temp.find_first_of(';');
	size = temp.size();
	if (firstPos != (size - 1)) { //chequear si se puede hace de otra forma
		//https://stackoverflow.com/questions/348833/how-to-know-the-exact-line-of-code-where-an-exception-has-been-caused
		throw std::runtime_error("Config error: invalid semicolon at line " + std::to_string(__LINE__));
	}
}

void ConfigChecker::_checkJunkData(const std::string &line) {
	const std::string& temp = line;
	removeWhiteSpaces(temp);
	if (!temp.empty()) {
		throw std::runtime_error("Config error: not allowed data in file");
	}
}

const std::string &ConfigChecker::getFilePath() const {
	return _filePath;
}

const std::string &ConfigChecker::getFileData() const {
	return _fileData;
}

/**
 * case 3 elements: "location␣/path␣{" is valid, has 2 spaces.
 * case 2 elements: "location␣/path{" is valid, has 1 spaces.
 */
void ConfigChecker::_checkLocation(const std::string &line) {
	std::vector<std::string> splitLocation;
	size_t size;

	splitLocation = splitString(line, ' ');
	size = splitLocation.size();
	switch (size) {
		case 2:
			if (splitLocation[0] != "location" || splitLocation[1] != "{") { //faltaria validar el arg[1],o en otro lado??
				throw std::runtime_error("Config error: invalid format in location block");
			}
			break;
		case 3:
			if (splitLocation[0] != "location" || splitLocation[2] != "{") {
				throw std::runtime_error("Config error: invalid format in location block");
			}
			break;
		default:
			throw std::runtime_error("Config error: invalid format in location block");


	}

}



