//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
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

// this function is deleting comments, but not empty lines nor whitespaces, check
void ConfigChecker::_excludeCommentsEmptyLines() {
	std::istringstream inputStream(_fileData);
	std::string line;
	std::string temp;
	std::size_t hastagPosition;

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

	for (std::size_t i = 0; i < _fileData.length(); i++) {
		if (_fileData[i] == '{') {
			curlyBraces.push('{');
			continue;
		}
		if (curlyBraces.empty() && _fileData[i] == '}') {
			throw std::runtime_error("Config error: Wrong curly braces.");
		}
		if (_fileData[i] == '}') {
			curlyBraces.pop();
		}
	}
	if (!curlyBraces.empty()) {
		throw std::runtime_error("Config error: Wrong curly braces.");
	}
}

void ConfigChecker::_checkSemiColon(const std::string &line) {
	std::size_t size;
	utils::removeWhiteSpaces(line);
	std::size_t firstPos = line.find(';');
	size = line.size();
	if (firstPos != (size - 1)) { //chequear si se puede hace de otra forma
		throw std::runtime_error("Config error: invalid semicolon in " + line);
	}
}

bool ConfigChecker::_isCorrectServerBlockIntro(const std::string &line) {
	utils::removeWhiteSpaces(line);
	if (line != "server {") {
		throw std::runtime_error("Config error: invalid format in opening server block." );
	}
	return true;
}

// chequear si esta funcion es necesaria, la idea es para que sirva despues o antes del server block
void ConfigChecker::_checkJunkData(const std::string &line) {
	utils::removeWhiteSpaces(line);
	if (!line.empty()) {
		throw std::runtime_error("Config error: not allowed data outside server blocks.");
	}
}

/**
 * case 3 elements: "location␣/path␣{" is valid, has 2 spaces.
 * case 2 elements: "location␣/path{" is valid, has 1 spaces.
 */
 // how to do for input line enters without whitespaces???
void ConfigChecker::_checkLocationIntro(const std::string &line) {
	std::vector<std::string> splitLocation = utils::splitString(line, ' ');
	std::size_t size = splitLocation.size();
	switch (size) {
		case 2:
			if (splitLocation[0] != "location" ||
				splitLocation[1] == "{" ||
				splitLocation[1].find('{') != splitLocation[1].length() - 1) {
				throw std::runtime_error("Config error: invalid format in location block." + std::to_string(__LINE__));
			}
			break;
		case 3:
			if (splitLocation[0] != "location" ||
				splitLocation[1].find('/') != 0 || //checks if / is in the first position
				splitLocation[2] != "{") {
				throw std::runtime_error("Config error: invalid format in location block." + std::to_string(__LINE__) );
			}
			break;
		default:
			throw std::runtime_error("Config error: invalid format in location block." + std::to_string(__LINE__) );
	}
}


const std::string &ConfigChecker::getFilePath() const {
	return _filePath;
}

const std::string &ConfigChecker::getFileData() const {
	return _fileData;
}

void ConfigChecker::_checkacceptedMethods(const std::string &line) { //complete this method
	std::vector<std::string> splitAllowedMethods = utils::splitString(line, ' ');
	if (splitAllowedMethods[0] != "accepted_methods") {
		throw std::runtime_error("Config error: invalid directive " + splitAllowedMethods[0]);
	}
}

// check if is necessary this snippet
void ConfigChecker::_checkClosingCurlyBraceLine(const std::string &line) {
	utils::removeWhiteSpaces(line);
	if (line != "}") {
		throw std::runtime_error("Config error: missing closing curly brace.");
	}
}


void ConfigChecker::_checkLocationBlock(std::string line, std::istringstream &stream) {
	_checkLocationIntro(line);
	while (std::getline(stream, line)) {
		if (line.find("accepted_methods") != std::string::npos) {
			_checkacceptedMethods(line);
		} else if (line.find('}') != std::string::npos) {
			std::cout << "hey location " << __LINE__ << std::endl; // delete, testing purposes
			_checkClosingCurlyBraceLine(line);
			break;
		} else {
			std::cout << "hey invalid semicolon location " << __LINE__ << std::endl; // delete, testing purposes
			_checkSemiColon(line);
		}
	}
}

//check this
void ConfigChecker::_checkServerBlocks() {
	bool isPresentServer = false;
	std::string line;
	std::istringstream stream(_fileData);

	while (std::getline(stream, line)) {
		if (line.find("server") != std::string::npos && line.find("server_name") == std::string::npos) {
			isPresentServer = _isCorrectServerBlockIntro(line);
		} else if (!isPresentServer) {
			_checkJunkData(line);
		}
		else if (line.find("location") != std::string::npos) {
//			std::cout << "Y SI ME PONEN LA CANCION" << std::endl;
			_checkLocationBlock(line, stream);
		}
		else if (line.find('{') != std::string::npos && isPresentServer == true) {
			_checkClosingCurlyBraceLine(line);
			isPresentServer = false;
		} else if (line.find("accepted_methods") != std::string::npos && isPresentServer == true) {
			throw std::runtime_error("Config error: directive is not allowed: accepted_methods.");
		} else {
			std::cout << "_checkServerBlocks " << __LINE__ << std::endl; // delete, testing purposes
			_checkSemiColon(line);
		}
	}
}

void ConfigChecker::checkEntireConfig() {
	_readFile();
	_excludeCommentsEmptyLines();
	_checkCurlyBraces();
	_checkServerBlocks();
}



