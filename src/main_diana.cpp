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
#include "Webserver.hpp"

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


void validarYAlmacenar(const Directive &directive, std::string &block, Webserver *webserver);

void almacenarDirectivaEnServer(const Directive &directive, Server *server);

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
		std::ifstream file;
		openFile(file, configFile);
		std::string line;
		Webserver webserver; // destructor debe liberar recursos - delete

		std::string command;
		char c;
		bool comment = false;
//		std::stack<char> curlyBraces;
		std::stack<std::string> block;
//		Server server;

		// loop getting single characters
		while (file.get(c)) {
			switch (c) {
				case '{':
					if (!comment) {
						std::cout << "curly: " << command << std::endl;
						if (command == SERVER) { // See if it is better define as enum
							webserver._servers.push_back(new Server());
						}
						// TODO extraer location
						size_t postPath = command.find_first_of('/'); //what does happen if there is no '/'??
						std::string cmdLocation = command.substr(0, postPath);
////						std::string locationPath = command.substr(postPath, command.size()-1);
						std::cout << "cmd is: " << cmdLocation << std::endl;
						std::cout << "cmd size is: " << command.size() << std::endl;
////						std::cout << "pos path is: " << postPath << std::endl;

						// option 2
//						std::string cmdLocation1 = LOCATION;
//						std::size_t found = command.find(cmdLocation1);
//						if (found != std::string::npos) {
//							std::cout << "first 'needle' found at: " << found << '\n';
//						}

						if (cmdLocation == LOCATION) {
							Server &server = webserver._servers.back();
							server._locations.push_back(new Location());
						}
						block.push(command); // validar location antes del push
						command = "";
//						curlyBraces.push(c);
					}
					break;
				case ';':
					if (!comment) {
						std::cout << "linea que se valida: " << command << std::endl;
						Directive directive = Configurator::splitDirective(command);
						validarYAlmacenar(directive, block.top(), &webserver);
						command = "";
					}
					break;
				case '}':
					if (!comment) {
						if (block.empty() && c == '}') {
							throw std::runtime_error("Config error: unbalanced curly braces.");
						}
						if (c == '}') {
							block.pop();
						}
					}
					break;
				case '#':
					comment = true;
					break;
				case '\n':
					comment = false;
					break;
				default:
					if (!comment)
						command += c;
					break;
			}
		}
		if (!block.empty()) {
			throw std::runtime_error("Config error: unbalanced curly braces.");
		}
	} catch (std::exception &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return EXIT_SUCCESS;
}

//change name to English
void validarYAlmacenar(const Directive &directive, std::string &block, Webserver *webserver) {
	if (block == "server") {
		almacenarDirectivaEnServer(directive, &webserver->_servers.back());
	}
}

//change name to English
void almacenarDirectivaEnServer(const Directive &directive, Server *server) {
	switch (Configurator::resolveDirective(directive.key)) {
		case Configurator::LISTEN:
			server->validateAndSetListen(directive.value);
			break;
		case Configurator::SERVER_NAME:
			//TODO
			break;
		case Configurator::ERROR_PAGE:
			//TODO
			break;
		case Configurator::BODY_SIZE:
			//TODO
			break;
		case Configurator::ROUTE_LOCATION:
			//TODO
			break;
		case Configurator::ROOT:
			//TODO
			break;
		case Configurator::ACCEPTED_METHODS:
			//TODO
			break;
		case Configurator::INDEX:
			//TODO
			break;
		case Configurator::AUTOINDEX:
			//TODO
			break;
		case Configurator::CGI:
			//TODO
			break;
		case Configurator::UPLOAD:
			//TODO
			break;
		case Configurator::REDIRECTION:
			//TODO
			break;
		case Configurator::INVALID:
			//TODO, error message
			break;
	}
}


//
//void validarYAlmacenar(Directive directive, std::string basicString, Server *server) {
//
//	switch (Configurator::resolveDirective(directive.key)) {
//			case Configurator::LISTEN:
//				server->validateAndSetListen(directive.value);
//				break;
//		}
//}


// set::insert (C++98)
/*

int main() {
	try {
		std::string input1 = "   	allowed_methods   GET	get	POST          DELETE ";
		std::string input = "   	listen   127.0.0.1:8000	8000	80         localhost:8000  	9090";
		std::string input2 = "		  error_page   	 404   /404.html		";
		std::string input3= "		  client_max_body_size   	404g 		";
		std::string input4= "		  root 			root.homl  		";
		std::string input5= "		  index 			pollo.HTMl 		";
		std::string input6= "		  autoindex 		On  		";
		std::string input7= "		  cgi 		.php /users/bin/php-cgi		";

		Directive res = Configurator::splitDirective(input6);
		std::cout << "res key is: " << res.key << std::endl;
		for (size_t i = 0; i < res.value.size(); ++i) {
			std::cout << "res second is: " << res.value.at(i) << std::endl;
		}

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

	} catch (std::exception &e) {
		std::cerr << PURPLE << e.what() << RESET << std::endl;
	}
	return 0;
}
*/
