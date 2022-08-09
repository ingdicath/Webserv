//
// Created by Diani on 08/08/2022.
//

#pragma once

class FileUtils {
public:
	FileUtils();
	virtual ~FileUtils();

	bool isEmptyFile(std::ifstream &inputFile);

	void openFile(std::ifstream &file, const std::string &filePath);

	void closeFile(std::ifstream &file);

private:

};

