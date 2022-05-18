//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#pragma once

#include <string>

class ConfigChecker {
public:

	ConfigChecker(std::string filePath);

	~ConfigChecker();

	const std::string &getFilePath() const;

	const std::string &getFileData() const;

	void _readFile();

private:
	std::string _filePath;
	std::string _fileData;



	void _excludeCommentsEmptyLines();

	void _checkCurlyBraces();

	static void _checkSemiColon(const std::string &line);

	static void _checkJunkData(const std::string &line);

	static void _checkLocation(const std::string& line);


};
