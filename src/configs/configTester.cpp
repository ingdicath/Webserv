//
// Created by Diani on 04/08/2022.
//

#include "ServerContext.hpp"
#include "../utils/settings.hpp"
#include "Configurator.hpp"
#include "../utils/utils.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
	std::string configFile;
	ServerContext server;

	switch (argc) {
		case 1:
			configFile = DEFAULT_CONFIG_FILE;
			break;
		case 2:
			configFile = argv[1];
			break;
		default:
			std::cerr << RED "\nInvalid number of arguments." RESET << std::endl;
			std::cerr << "Usage: ./webserv [configuration file]\n" << std::endl;
			return EXIT_FAILURE;
	}
	try {
		std::ifstream file;
		config::openFile(file, configFile);
		std::string line;

		std::string command;
		char c;
		bool comment = false;
		std::stack<std::string> block;

		// loop getting single characters
		while (file.get(c)) {
			switch (c) {
				case '{':
					if (!comment) {
						std::cout << "curly: " << command << std::endl;
						command = utils::trim(command);
						if (command == SERVER) {                    // See if it is better define as enum
							std::cout << "New server found!" << std::endl;
						}
						// TODO extraer location
						size_t postPath = command.find_first_of('/'); //what does happen if there is no '/'??
						std::string cmdLocation = command.substr(0, postPath);

						if (cmdLocation == LOCATION) {
							std::cout << "New location found!" << std::endl;
						}
						block.push(command); // validar location antes del push
						command = "";
					}
					break;
				case ';':
					if (!comment) {
						std::cout << "line that is validated: " << command << std::endl;

//						Directive directive = config::splitDirective(command);
						config::validateAndStore(directive, block.top(), servers); // WATCH OUT! CHECK THIS
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
		std::cout << RED << e.what() << RESET << std::endl;
	}
	return EXIT_SUCCESS;
}