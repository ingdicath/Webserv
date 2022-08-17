//
// Created by Diani on 08/08/2022.
//

#include <fstream>
#include "FileUtils.hpp"

FileUtils::FileUtils() {}

FileUtils::~FileUtils() {}

bool FileUtils::isEmptyFile(std::ifstream &inputFile) {
	return inputFile.peek() == std::ifstream::traits_type::eof();
}

void FileUtils::openFile(std::ifstream &file, const std::string &filePath) {

	file.open(filePath.c_str(), std::ifstream::in);
	if (!file.is_open()) {
		throw std::runtime_error("[ERROR] Config error: Configuration file failed to open.");
	}
	if (isEmptyFile(file)) {
		throw std::runtime_error("[ERROR] Config error: Empty file.");
	}
}

void FileUtils::closeFile(std::ifstream &file) {
	if (file.is_open()) {
		file.close();
	}
}
