//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream>
#include "settings.hpp"

int main(int argc, char **argv) {
//	WebServer webServer; //contains methods to run webserv
	std::string configFile;

	switch (argc) {
		case 1:
			configFile = DEFAULT_CONFIG_FILE;
			std::cout << argv[0] << " - I'm calling the default config file!" << std::endl; // test, delete
			break;
		case 2:
			configFile = argv[1];
			std::cout << configFile << " - I am the config file entered by the user!" << std::endl; // test, delete
			break;
		default:
			std::cerr << "Invalid number of arguments." << std::endl;
			std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
			return EXIT_FAILURE;
	}
	try {
//		webServer.check(configFile); // validate config file
//		webServer.initialize(parser.getServerConfig()); // if is valid setup webserver with config files parameters
//		webServer.run(); //start webserv
		std::cout << " Inside try!" << std::endl; // test, delete
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
