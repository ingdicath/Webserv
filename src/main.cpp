//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream> // std::cerr
#include <cstdlib> // macros EXIT_FAILURE, EXIT_SUCCESS

int main(int argc, char **argv) {
	Parser parser; // contains all checkers to validate config file
	WebServer webServer; //contains methods to run webserv
	std::string configFile; // argument


	if (argc > 2) {
		std::cerr << "One argument is expected." << std::endl;
		std::cerr << "Usage 1: ./webserv [configuration file]" << std::endl;
		std::cerr << "Usage 2: ./webserv [default path]" << std::endl;
		return EXIT_FAILURE;
	}
	try {
		// Your program has to take a configuration file as argument, or use a default path.
		if (argc == 1) {
			configFile = "config_files/default.conf";
		}
		parser.check(configFile); // validate config file
		webServer.initialize(parser.getServerConfig()); // if is valid setup webserver with config files parameters
		webServer.run(); //start webserv
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << argv[1] << " Hello, World!" << std::endl;
	return EXIT_SUCCESS;
}
