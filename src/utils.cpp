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
//		int x;
//		int count=0;
		while (it != str.end() && std::isdigit(*it)) {
			it++;
//			x = (*it+'0')+(count++ * 10);
		}

		return !str.empty() && it == str.end();
	}

	size_t stringToPositiveNum(const std::string &input) {
		size_t res;
		if (!isPositiveNumber(input))
			throw std::runtime_error("no soy positivo");
		std::stringstream ss(input);
		ss >> res;
		return res;
	}

//	size_t stringToPositiveNum(const std::string &input) {
//		size_t res;
//
//
//		return res;
//	}




// trim from end of string (right)
	std::string &rtrim(std::string &str) {
		str.erase(str.find_last_not_of(WHITESPACES) + 1);
		return str;
	}

// trim from beginning of string (left)
	std::string &ltrim(std::string &str) {
		str.erase(0, str.find_first_not_of(WHITESPACES));
		return str;
	}

// trim from both ends of string (right then left)
	std::string &trim(std::string &str) {
		return ltrim(rtrim(str));
	}

// it is not allowed used inet_pton
//#include <arpa/inet.h>
//
//	bool validateIpAddress(const std::string &ipAddress){
//		struct sockaddr_in sa = {};
////		int res = inet_addr(ipAddress.c_str());
//		int res = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
//		return res != 0;
//	}

//	bool validateIpAddress(const std::string &ipAddress){
//		std::vector<std::string> vec;
//		vec = splitString(ipAddress, '.');
//		if (vec.size() != 4){
//			return false;
//		}
//		for (size_t i = 0; i < vec.size(); i++){
//			vec.at(i)
//
//		}
//
//		return true;
//	}

} // end namespace utils
