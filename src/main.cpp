//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream> // std::cerr
#include <cstdlib> // macros EXIT_FAILURE, EXIT_SUCCESS

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "One argument is expected." << std::endl;
		std::cerr << "Usage 1: ./webserv [configuration file]" << std::endl;
		std::cerr << "Usage 2: ./webserv [default path]" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << argv[1] << " Hello, World!" << std::endl;
	return EXIT_SUCCESS;
}
