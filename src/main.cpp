//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "ConfigChecker.hpp"
#include "settings.hpp"
#include "utils.hpp"


//int main(int argc, char **argv) {
//	std::string configFile;
//
//
//
//
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


bool in_array(const std::string &value, const std::vector<std::string> &array)
{
	return std::find(array.begin(), array.end(), value) != array.end();
}

int main(){
	std::ifstream file;
	std::string line;

	// listado de comandos validos
	std::vector<std::string> s;
	s.push_back("listen");
	s.push_back("server_name");
	s.push_back("index");

	file.open(DEFAULT_CONFIG_FILE);

	char c;
	std::string command;
	bool comment = false;
	std::string test;
	while (file.get(c)) {

		switch (c) {
			case '{':
				std::cout << "curly: " << command << std::endl ;
				command ="";
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
				if (in_array(command,s)){
					std::cout << "in array: " << command << std::endl;
					command ="";
				}

				break;
			case '}':
				// disparador
				break;
			default:
				if (!comment)
					command = command + c;
				break;
		}  // loop getting single characters

//		if (test == "§$" ){
//			std::cout<< "aqui";
//			break;
//		}


	}




	return 0;

}

