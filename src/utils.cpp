//
// Created by Diana catherine Salamanca leguizamon on 5/19/22.
//

#include <string>
#include <vector>
#include "utils.hpp"

namespace utils {

	void removeWhiteSpaces(std::string str) {
		str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	}

// https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
	std::vector<std::string> splitString(const std::string &str, char splitter) {
		std::vector<std::string> res;
		std::stringstream line(str);
		std::string segment;

		while (std::getline(line, segment, splitter)) {
			res.push_back(segment);
		}
		return res;
	}

	bool isPositiveNumber(const std::string &str) {
		std::string::const_iterator it = str.begin();
		while (it != str.end() && std::isdigit(*it)) {
			it++;
		}
		return !str.empty() && it == str.end();
	}

	size_t stringToPositiveNum(const std::string &input) {
		size_t res;
		if (!isPositiveNumber(input))
			throw std::runtime_error("Invalid input for positive number.");
		std::stringstream ss(input);
		ss >> res;
		return res;
	}

// trim from end of string (right)
	std::string &rTrim(std::string &str) {
		str.erase(str.find_last_not_of(WHITESPACES) + 1);
		return str;
	}

// trim from beginning of string (left)
	std::string &lTrim(std::string &str) {
		str.erase(0, str.find_first_not_of(WHITESPACES));
		return str;
	}

// trim from both ends of string (right then left)
	std::string &trim(std::string &str) {
		return lTrim(rTrim(str));
	}


	std::string deleteLastOf(char toSearch, std::string str) {
		size_t pos = str.find_last_of(toSearch);
		std::string cleanStr;
		if (pos != std::string::npos) {
			cleanStr = str.erase(pos, pos + 1);
		}
		return cleanStr;
	}

	std::string stringToUpper(std::string oString) {
		for (size_t i = 0; i < oString.length(); i++) {
			oString[i] = char(std::toupper(static_cast<unsigned char>(oString[i])));
		}
		return oString;
	}

	std::string stringToLower(std::string oString) {
		for (size_t i = 0; i < oString.length(); i++) {
			oString[i] = char(std::tolower(static_cast<unsigned char>(oString[i])));
		}
		return oString;
	}

} // end namespace utils
