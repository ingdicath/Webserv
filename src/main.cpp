//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream> // std::cerr
#include <cstdlib> // for EXIT_FAILURE
#include "settings.hpp" // for DEFAULT_CONFIG_FILE

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>


int main(int argc, char **argv) {
//	Parser parser; // contains all checkers to validate config file
//	WebServer webServer; //contains methods to run webserv
	std::string configFile;

	if (argc > 2) {
		std::cerr << "Invalid number of arguments." << std::endl;
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return EXIT_FAILURE;
	}
	// Your program has to take a configuration file as argument, or use a default path.
	if (argc == 1) {
		std::cout << argv[0] << " - I'm calling the default config file!" << std::endl; // test, delete
		configFile = DEFAULT_CONFIG_FILE;
	} else if (argc == 2) {
		configFile = argv[1];
		std::cout << configFile << " - I am the config file entered by the user!" << std::endl; // test, delete
	}
	try {
//		parser.check(configFile); // validate config file
//		webServer.initialize(parser.getServerConfig()); // if is valid setup webserver with config files parameters
//		webServer.run(); //start webserv
		std::cout << " Inside try!" << std::endl; // test, delete
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
