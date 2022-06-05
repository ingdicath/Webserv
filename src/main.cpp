//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include "Server.hpp"
#include "settings.hpp"
#include "utils.hpp"

bool in_array(const std::string &value, const std::vector<std::string> &array) {
	return std::find(array.begin(), array.end(), value) != array.end();
}

bool isEmptyFile(std::ifstream &inputFile) {
	return inputFile.peek() == std::ifstream::traits_type::eof();
}

void openFile(std::ifstream &file, const std::string &filePath) {
	file.open(filePath.c_str(), std::ifstream::in);
	if (!file.is_open()) {
		throw std::runtime_error("Config error: Configuration file failed to open.");
	}
	if (isEmptyFile(file)) {
		throw std::runtime_error("Config error: Empty file.");
	}
//	file.close(); // dont forget to close
}

//int main(int argc, char **argv) {
//	std::string configFile;
//
//	switch (argc) {
//		case 1:
//			configFile = DEFAULT_CONFIG_FILE;
//			std::cout << argv[0] << " " << DEFAULT_CONFIG_FILE << " - I'm default config file!" << std::endl; // delete
//			break;
//		case 2:
//			configFile = argv[1];
//			break;
//		default:
//			std::cerr << RED "Invalid number of arguments." << std::endl;
//			std::cerr << "Usage: ./webserv [configuration file]" RESET << std::endl;
//			return EXIT_FAILURE;
//	}
//	try {
//		// https://stackoverflow.com/questions/14561941/how-do-i-pass-multiple-ints-into-a-vector-at-once
//		// valid command list
//		std::vector<std::string> validCommands;
//		validCommands.push_back("listen");
//		validCommands.push_back("location");
//		validCommands.push_back("server_name");
//		validCommands.push_back("error_page");
//		validCommands.push_back("client_max_body_size");
//		validCommands.push_back("index");
//		validCommands.push_back("root");
//		validCommands.push_back("accepted_methods");
//		validCommands.push_back("autoindex");
//		validCommands.push_back("cgi");
//		validCommands.push_back("upload");
//		validCommands.push_back("return");
//
//		std::ifstream file;
//		openFile(file, configFile);
//		std::string line;
//
//		std::string command;
//		char c;
//		bool comment = false;
//		std::stack<char> curlyBraces;
//		std::stack<std::string> directives;
//		Server server;
//
//		// loop getting single characters
//		while (file.get(c)) {
//			switch (c) {
//				case '{':
//					std::cout << "curly: " << command << std::endl;
//					directives.push(command); // fix
//					command = "";
//					curlyBraces.push(c);
//					// function to validate insert command and curly brace in the stack
//					break;
//				case ';':
//					std::cout << "semicolon: " << command << std::endl;
//					command = "";
//					break;
//				case '#':
//					// CHECKEO TRATAMIENTO
//					//std::cout << "comentario" << std::endl;
//					comment = true;
//					break;
//				case '\n':
//					comment = false;
//					// ignore all jump line
//					break;
////				case ' ':
////				case '\t':
////				case '\v':
////				case '\f':
////				case '\r':
////					if (in_array(command, validCommands)) {
////						std::cout << "in array: " << command << std::endl;
////						command = "";
////					}
////					break;
//				case '}':
//					// disparador or trigger
//					if (curlyBraces.empty() && c == '}') {
//						throw std::runtime_error("Config error: unbalanced curly braces.");
//					}
//					if (c == '}') {
//						curlyBraces.pop();
//					}
//					break;
//				default:
//					if (!comment)
//						command += c;
//					break;
//			}
//
////		if (test == "§$" ){
////			std::cout<< "aqui";
////			break;
////		}
//		}
//		if (!curlyBraces.empty()) {
//			throw std::runtime_error("Config error: unbalanced curly braces.");
//		}
//	} catch (std::exception &e) {
//		std::cerr << RED << e.what() << RESET << std::endl;
//	}
//
//
//	return EXIT_SUCCESS;
//}



/*

 // test atoi vs stringstream
#include <typeinfo>

int main(){
	std::string str = "456899798798798798798798798798798";
	if (!utils::isPositiveNumber(str)){
		std::cerr << "no valid number" << std::endl;
	}
	size_t res = utils::stringToNum(str);
	std::cout << "res is: " << res << std::endl;
	std::cout << typeid(res).name() << std::endl;

	size_t num = std::atoi(str.c_str());
	std::cout << "res 2 is: " << num << std::endl;
	std::cout << typeid(num).name() << std::endl;
	return 0;
}
*/

//bool isValidListen (std::string value){
//
//	return false;
//}

//const char *ws = " \t\n\r\f\v";
//const  std::string a = " \n\t\r\f\v";
//// trim from end of string (right)
//std::string &rtrim(std::string &s, const char *t = ws) {
//	s.erase(s.find_last_not_of(t) + 1);
//	return s;
//}
//
//// trim from beginning of string (left)
//std::string &ltrim(std::string &s, const char *t = ws) {
//	s.erase(0, s.find_first_not_of(t));
//	return s;
//}
//
//// trim from both ends of string (right then left)
//std::string &trim(std::string &s, const char *t = ws) {
//	return ltrim(rtrim(s, t), t);
//}


// trim from end of string (right)
std::string &rtrim(std::string &s) {
//	std::string ws = " \n\t\r\f\v";
	s.erase(s.find_last_not_of(WHITESPACES) + 1);
	return s;
}

// trim from beginning of string (left)
std::string &ltrim(std::string &s) {
//	std::string ws = " \n\t\r\f\v";
	s.erase(0, s.find_first_not_of(WHITESPACES));
	return s;
}

// trim from both ends of string (right then left)
std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

int main() {
	std::string str = "1.1.1.1:8000";
	std::string str1 = "1.1.1.1:8000 32.142.2.4:80";
	std::string str2 = "8000";
	std::string str3 = "1.1.1.1";
	std::string str4 = "localhost:8000";
//	std::cout << isValidListen(str);
//	std::cout << isValidListen(str2);
//	std::cout << isValidListen(str3);
//	std::cout << isValidListen(str4);
	std::string s = "		server_name                             localhost";

	std::string first_token = s.substr(0, s.find(' '));
	std::string second_part = s.substr(s.find(' '), s.size());
	std::cout << trim(first_token) << std::endl;
	std::cout << trim(second_part) << std::endl;
	return 0;
}
//
