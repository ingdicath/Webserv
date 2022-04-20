//
// Created by Diana Salamanca on 20/04/2022.
//

#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2){
		std::cerr << "One argument expected. Usage: ./webserv [configuration file]" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << argv[1] << " Hello, World!" << std::endl;
	return EXIT_SUCCESS;
}
