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
#include "Configurator.hpp"

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

/*
int main(int argc, char **argv) {
	std::string configFile;

	switch (argc) {
		case 1:
			configFile = DEFAULT_CONFIG_FILE;
			std::cout << argv[0] << " " << DEFAULT_CONFIG_FILE << " - I'm default config file!" << std::endl; // delete
			break;
		case 2:
			configFile = argv[1];
			break;
		default:
			std::cerr << RED "Invalid number of arguments." << std::endl;
			std::cerr << "Usage: ./webserv [configuration file]" RESET << std::endl;
			return EXIT_FAILURE;
	}
	try {
		// https://stackoverflow.com/questions/14561941/how-do-i-pass-multiple-ints-into-a-vector-at-once
		// valid command list
		std::vector<std::string> validCommands;
		validCommands.push_back("listen");
		validCommands.push_back("location");
		validCommands.push_back("server_name");
		validCommands.push_back("error_page");
		validCommands.push_back("client_max_body_size");
		validCommands.push_back("index");
		validCommands.push_back("root");
		validCommands.push_back("accepted_methods");
		validCommands.push_back("autoindex");
		validCommands.push_back("cgi");
		validCommands.push_back("upload");
		validCommands.push_back("return");

		std::ifstream file;
		openFile(file, configFile);
		std::string line;

		std::string command;
		char c;
		bool comment = false;
		std::stack<char> curlyBraces;
		std::stack<std::string> directives;
		Server server;

		// loop getting single characters
		while (file.get(c)) {
			switch (c) {
				case '{':
					std::cout << "curly: " << command << std::endl;
					directives.push(command); // fix
					command = "";
					curlyBraces.push(c);
					// function to validate insert command and curly brace in the stack
					break;
				case ';':
					std::cout << Configurator::splitDirective(command).first << " [" << Configurator::splitDirective(command).second << "]" << std::endl;
					command = "";
					break;
				case '#':
					// CHECKEO TRATAMIENTO
					//std::cout << "comentario" << std::endl;
					comment = true;
					break;
				case '\n':
					comment = false;
					// ignore all jump line
					break;
//				case ' ':
//				case '\t':
//				case '\v':
//				case '\f':
//				case '\r':
//					if (in_array(command, validCommands)) {
//						std::cout << "in array: " << command << std::endl;
//						command = "";
//					}
//					break;
				case '}':
					// disparador or trigger
					if (curlyBraces.empty() && c == '}') {
						throw std::runtime_error("Config error: unbalanced curly braces.");
					}
					if (c == '}') {
						curlyBraces.pop();
					}
					break;
				default:
					if (!comment)
						command += c;
					break;
			}
		}
		if (!curlyBraces.empty()) {
			throw std::runtime_error("Config error: unbalanced curly braces.");
		}
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return EXIT_SUCCESS;
}
*/

// set::insert (C++98)
#include <iostream>
#include <set>
// test atoi vs stringstream
#include <typeinfo>

int main() {
	try {
		std::string input1 = "   	allowed_methods   GET		get POST          DELETE";
		std::string input = "   	listen   127.0.0.1:8000	8000	80         localhost:8000  	9090";
		std::string input2 = "		  error_page   	 404   /404.html		";

		Directive res = Configurator::splitDirective(input1);
		std::cout << "res key is: " << res.key << std::endl;
		for (size_t i = 0; i < res.value.size(); ++i) {
			std::cout << "res second is: " << res.value.at(i) << std::endl;
		}




//		std::string str = "454528";
//		if (!utils::isPositiveNumber(str)){
//			std::cerr << "no valid number" << std::endl;
//		}
//		size_t res = utils::stringToPositiveNum(str);
//		Configurator::_isValidPortRange(str);

//		std::string str1 = "1.1.1.255.";
//		bool res = Configurator::_isValidIpv4Address(str1);
//		std::cout << std::boolalpha << "answer is: " <<  res << std::endl;

//		std::string str2 = "localhost";
//		bool res1 = Configurator::_isValidIpPort(str2);
//		std::cout << std::boolalpha << "answer is d: " <<  res1 << std::endl;

//		std::string str3 = "		  505   			/localhost"     ;
//		bool res2 = Configurator::_isValidErrorPageConfig(str3);
//		std::cout << std::boolalpha << "answer for path is: " <<  res2 << std::endl;

//		std::string str4 = "-510";
//		bool res3 = Configurator::_isValidErrorCode(str4);
//		std::cout << std::boolalpha << "answer for error code is: " <<  res3 << std::endl;

//		std::string str5 = "155555500G";
//		bool res4 = Configurator::_isValidBodySize(str5);
//		std::cout << std::boolalpha << "answer for body size is: " <<  res4 << std::endl;

//		std::string str6 = "Location.hpp";
//		bool res5 = Configurator::_isValidRoot(str6);
//		std::cout << std::boolalpha << "answer for root is: " << res5 << std::endl;

//		std::string str7 = "     get 	GET   post				";
//		bool res6 = Configurator::_isValidAllowedMethod(str7);
//		std::cout << std::boolalpha << "answer for allowed method is: " << res6 << std::endl;
//
//		std::vector<std::string> directiveValues;
//		std::string input = "   	allowed_methods   GET		get POST          DELETE";
//		Configurator::splitDirective(input);

//		std::string cleanInput = utils::trim(input);
//		size_t splitPos = cleanInput.find_first_of(WHITESPACES);
//		std::string a = cleanInput.substr(0, splitPos);
//		directiveValues.push_back(a);
//
//		std::string b = cleanInput.substr(splitPos, cleanInput.size() - 1);
//		b = utils::trim(b);
//		size_t splitPos1 = b.find_first_of(WHITESPACES);
//		std::string c = b.substr(0, splitPos1);
//		directiveValues.push_back(c);

//		std::string cleanInput = utils::trim(input);








//		std::cout << "res is: " << res << std::endl;
//		std::cout << typeid(res).name() << std::endl;
//
//		size_t num = std::atoi(str.c_str());
//		std::cout << "res 2 is: " << num << std::endl;
//		std::cout << typeid(num).name() << std::endl;
//		size_t num1 = std::stoi(str);
//
//		std::cout << typeid(num1).name() << 	num1 << std::endl;
	} catch (std::exception &e) {
		std::cerr << PURPLE << e.what() << RESET << std::endl;
	}
	return 0;
}


/*
int main() {
	try{
		std::string str = "1.1.1.256";
		std::string str1 = "1.1.1.1:8000 32.142.2.4:80";
		std::string str2 = "800";
		std::string str3 = "1.1.1.1";
		std::string str4 = "localhost:8000";

		std::cout << std::boolalpha << utils::validateIpAddress(str) << std::endl; // testing, delete
		if (utils::_isValidIpv4Address(str)){
			std::cout << "mal" << std::endl;
		}
//		Configurator::_isValidPortRange(str2);
	} catch (std::exception &e){
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return 0;
}
*/
