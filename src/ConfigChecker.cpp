//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#include <iostream>
#include <cstdlib> //size_t
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
std::vector<std::string> splitString(std::string str, char splitter) {
	std::vector<std::string> res;
	std::string current;
	for (int i = 0; i < str.size(); i++) {
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
 * Put in another file option 3 for split
 */
std::stringstream test("this_is_a_test_string");
std::string segment;
std::vector<std::string> seglist;

	while(std::getline(test, segment, '_'))
	{
	seglist.push_back(segment);
	}

/**
 * Put in another file option 2 for split
 */
void tokenize(std::string const &str, const char *delim, std::vector<std::string> &out) {
	char *token = strtok(const_cast<char *>(str.c_str()), delim);
	while (token != NULL) {
		out.push_back(std::string(token));
		token = strtok(NULL, delim);
	}
}

/**
 * Put in another file option 2 for split
 */
// https://stackoverflow.com/questions/5607589/right-way-to-split-an-stdstring-into-a-vectorstring
std::string s = "What is the right way to split a string into a vector of strings";
std::stringstream ss(s);
std::istream_iterator<std::string> begin(ss);
std::istream_iterator<std::string> end;
std::vector<std::string> vstrings(begin, end);
std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

// You need to convert to a c-string if you are not using c++11 or later.
void ConfigChecker::_readFile() {
	std::ifstream file;
	std::string line;

	file.open(_filePath.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Configuration file failed to open");
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
	std::string temp = line;
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

void ConfigChecker::_checkLocation(const std::string &line) {
	std::vector<std::string> splitLocation;
	size_t size;

	splitLocation = splitString(line);
	size = splitLocation.size();


}



