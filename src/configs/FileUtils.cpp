/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileUtils.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: diana <diana@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 08:35:37 by diana         #+#    #+#                 */
/*   Updated: 2022/10/25 08:35:37 by diana         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
		throw std::runtime_error("[ERROR] Configuration file failed to open.");
	}
	if (isEmptyFile(file)) {
		throw std::runtime_error("[ERROR] Empty file.");
	}
}

void FileUtils::closeFile(std::ifstream &file) {
	if (file.is_open()) {
		file.close();
	}
}
