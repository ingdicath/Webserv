//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> //find_first_of
#include "ConfigChecker.hpp"
#include "utils.hpp"

ConfigChecker::ConfigChecker(const std::string &filePath) : _filePath(filePath) {}

ConfigChecker::~ConfigChecker() {}

bool isEmptyFile(std::ifstream &inputFile) {
	return inputFile.peek() == std::ifstream::traits_type::eof();
}

// You need to convert to a c-string if you are not using c++11 or later.
void ConfigChecker::_readFile() {
	std::ifstream file;
	std::string line;

	file.open(_filePath.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Config error: Configuration file failed to open.");
	}
	if (isEmptyFile(file)) {
		throw std::runtime_error("Config error: Empty file.");
	}
	while (std::getline(file, line)) {
		_fileData.append(line + "\n");
	}
	file.close();
}

void ConfigChecker::_deleteCommentsEmptyLines() {
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
		utils::removeWhiteSpaces(temp); //implement, namespace utils??
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
	utils::removeWhiteSpaces(line);
//	size_t firstPos = line.find_first_of(';'); //decide which is better: find_first_of or find
	size_t firstPos = line.find(';');
	size = line.size();
	if (firstPos != (size - 1)) { //chequear si se puede hace de otra forma
		throw std::runtime_error("Config error: invalid semicolon at line " + std::to_string(__LINE__));
	}
}

bool ConfigChecker::isCorrectServerBlockIntro(const std::string &line) {
	utils::removeWhiteSpaces(line);
	if (line != "server {") {
		throw std::runtime_error("Config error: invalid format in server block");
	}
	return true;
}

// chequear si esta funcion es necesaria, la idea es para que sirva despues o antes del server block
void ConfigChecker::_checkJunkData(const std::string &line) {
	utils::removeWhiteSpaces(line);
	if (!line.empty()) {
		throw std::runtime_error("Config error: not allowed data outside server blocks");
	}
}

/**
 * case 3 elements: "location␣/path␣{" is valid, has 2 spaces.
 * case 2 elements: "location␣/path{" is valid, has 1 spaces.
 */
void ConfigChecker::_checkLocationIntro(const std::string &line) {
	std::vector<std::string> splitLocation = utils::splitString(line, ' ');
	size_t size = splitLocation.size();
	switch (size) {
		case 2:
			if (splitLocation[0] != "location" ||
				splitLocation[1] == "{" ||
				splitLocation[1].find('{') != splitLocation[1].length() - 1) {
				throw std::runtime_error("Config error: invalid format in location block");
			}
			break;
		case 3:
			if (splitLocation[0] != "location" ||
				splitLocation[1].find('/') != 0 || //checks if / is in the first position
				splitLocation[2] != "{") {
				throw std::runtime_error("Config error: invalid format in location block");
			}
			break;
		default:
			throw std::runtime_error("Config error: invalid format in location block");
	}
}


const std::string &ConfigChecker::getFilePath() const {
	return _filePath;
}

const std::string &ConfigChecker::getFileData() const {
	return _fileData;
}

void ConfigChecker::_checkAllowedMethods(const std::string &line) { //complete this method
	std::vector<std::string> splitAllowedMethods = utils::splitString(line, ' ');
	if (splitAllowedMethods[0] != "allowed_methods") {
		throw std::runtime_error("Config error: invalid directive " + std::to_string(__LINE__));
	}
}

void ConfigChecker::_checkLocationBlock(std::string line, std::istringstream stream) {
	_checkLocationIntro(line);
//	while (std::getline(stream, line)) {
//		if (line.find("allowed_methods"))
//
//	}

}







