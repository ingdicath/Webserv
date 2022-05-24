//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include "ConfigChecker.hpp"
#include "settings.hpp"
#include "utils.hpp"


//int main(int argc, char **argv) {
//	std::string configFile;
//
//	switch (argc) {
//		case 1:
//			configFile = DEFAULT_CONFIG_FILE;
//			std::cout << argv[0] << " " << DEFAULT_CONFIG_FILE << " - I'm calling the default config file!"
//					  << std::endl; // test, delete
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
//		// WebServer webServer; //contains methods to run webserv
//		ConfigChecker conf(configFile);
//		conf.checkEntireConfig();
////		conf._readFile();
////		conf._excludeCommentsEmptyLines();
////		conf._checkCurlyBraces();
//
////		webServer.check(configFile); // validate config file
////		webServer.initialize(parser.getServerConfig()); // if is valid setup webserver with config files parameters
////		webServer.run(); //start webserv
//	} catch (std::exception &e) {
//		std::cerr << e.what() << std::endl;
//	}
//	return EXIT_SUCCESS;
//}


bool in_array(const std::string &value, const std::vector<std::string> &array) {
	return std::find(array.begin(), array.end(), value) != array.end();
}

bool isEmptyFile(std::ifstream &inputFile) {
	return inputFile.peek() == std::ifstream::traits_type::eof();
}

void openFile(const std::string &filePath) {
	std::ifstream file;

	file.open(filePath.c_str(), std::ifstream::in);
	if (!file.is_open()) {
		throw std::runtime_error("Config error: Configuration file failed to open.");
	}
	if (isEmptyFile(file)) {
		throw std::runtime_error("Config error: Empty file.");
	}
	file.close();
}

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
		openFile(configFile); // testing if a file could be opened

		// listado de comandos validos
		std::vector<std::string> validCommands;
		validCommands.push_back("listen");
		validCommands.push_back("location");
		validCommands.push_back("server_name");
		validCommands.push_back("error_page");
		validCommands.push_back("client_max_body_size");
		validCommands.push_back("index");
		validCommands.push_back("root");
		validCommands.push_back("allowed_methods");
		validCommands.push_back("autoindex");
		validCommands.push_back("cgi");
		validCommands.push_back("upload");
		validCommands.push_back("return");

		std::ifstream file;
		std::string line;
		file.open(configFile);

		char c;
		std::string command;
		bool comment = false;
		std::string test;
		std::stack<char> curlyBraces;
		std::stack<std::string> directives;


		// loop getting single characters
		while (file.get(c)) {
			switch (c) {
				case '{':
					std::cout << "curly: " << command << std::endl;
					command = "";
					directives.push(command);
					curlyBraces.push(c);
					// function to validate insert command and curly brace in the stack
					test = "§"; // only testing
					break;
				case ';':
					std::cout << "semicolon: " << command << std::endl;
					test += "$"; // only testing
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
				case ' ':
				case '\t':
				case '\v':
				case '\f':
				case '\r':
					if (in_array(command, validCommands)) {
						std::cout << "in array: " << command << std::endl;
						command = "";
					}
					break;
				case '}':
					// disparador
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

//		if (test == "§$" ){
//			std::cout<< "aqui";
//			break;
//		}
		}
		if (!curlyBraces.empty()) {
			throw std::runtime_error("Config error: unbalanced curly braces.");
		}
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return EXIT_SUCCESS;
}


// error: extraneous closing brace ('}')
