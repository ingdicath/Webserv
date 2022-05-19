//
// Created by Diana catherine Salamanca leguizamon on 5/17/22.
//

#pragma once

#include <string>


class ConfigChecker {
public:

	enum eHTTPMethods {
		GET, POST, DELETE
	};

	ConfigChecker(const std::string &filePath);
//	ConfigChecker(ConfigChecker const &obj);
//	ConfigChecker &operator=(ConfigChecker const &obj);
	virtual ~ConfigChecker();

	const std::string &getFilePath() const;

	const std::string &getFileData() const;

	void _readFile();


private:
	std::string _filePath;
	std::string _fileData;


	void _deleteCommentsEmptyLines();

	void _checkCurlyBraces();

	static void _checkSemiColon(const std::string &line);

	static void _checkJunkData(const std::string &line);

	static void _checkLocationIntro(const std::string &line);

	static bool isCorrectServerBlockIntro(const std::string &line);

	static void _checkAllowedMethods(const std::string &line);

	void _checkLocationBlock(std::string line, std::istringstream stream);


};
