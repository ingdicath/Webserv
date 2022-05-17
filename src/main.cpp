//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream>
#include "ConfigChecker.hpp"
#include "settings.hpp"

int main(int argc, char **argv) {
	std::string configFile;

	switch (argc) {
		case 1:
			configFile = DEFAULT_CONFIG_FILE;
			std::cout << argv[0] << " " <<DEFAULT_CONFIG_FILE << " - I'm calling the default config file!" << std::endl; // test, delete
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
		// WebServer webServer; //contains methods to run webserv
		ConfigChecker conf(configFile);
		conf._readFile();

//		webServer.check(configFile); // validate config file
//		webServer.initialize(parser.getServerConfig()); // if is valid setup webserver with config files parameters
//		webServer.run(); //start webserv
		std::cout << " Inside try!" << std::endl; // test, delete
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
