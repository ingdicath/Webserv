/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: diana <diana@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 08:58:06 by diana         #+#    #+#                 */
/*   Updated: 2022/10/25 08:58:06 by diana         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "utils.hpp"
#include "settings.hpp"

namespace utils {

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
		if (!isPositiveNumber(input)) {
			std::cerr << RED ERROR " Invalid input for positive number: '" + input + "'" RESET << std::endl;
		}
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

	int stringToInt(const std::string &input) {
		int res;
		std::istringstream convert(input);
		if (!(convert >> res)) {
			res = -1;
		}
		return res;
	}

	long stringToLong(const std::string &input) {
		long res;
		std::istringstream convert(input);
		if (!(convert >> res)) {
			res = -1;
		}
		return res;
	}

	std::vector<std::string> splitByWhiteSpaces(const std::string &string, const std::string &whiteSp) {
		size_t startPos = 0;
		size_t endPos;
		size_t splitPos;
		size_t len;
		std::vector<std::string> vec;

		while (startPos < string.size()) {
			splitPos = string.find_first_of(whiteSp, startPos);
			endPos = std::min(splitPos, string.size());
			len = endPos - startPos;
			if (len) {
				std::string sub = string.substr(startPos, len);
				utils::trim(sub);
				vec.push_back(sub);
			}
			startPos = endPos + 1;
		}
		return vec;
	}

} // end namespace utils
