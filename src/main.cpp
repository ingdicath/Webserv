/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: diana <diana@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 09:00:11 by diana         #+#    #+#                 */
/*   Updated: 2022/10/25 09:00:11 by diana         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdlib>
#include "webserver/Webserver.hpp"
#include "utils/settings.hpp"

int DEBUG = 0;

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << GREEN "\b\bWebserver stopped" RESET << std::endl;
		exit(EXIT_SUCCESS);
	}
}

int checkFlagDebug(const std::string &str) {
	if (str == "--debug") {
		return 1;
	} else {
		return 0;
	}
}

int     main(int argc, char* argv[]) {
	std::string configFile = DEFAULT_CONFIG_FILE;
	Webserver webserver;
	DEBUG = 0;

	switch (argc) {
		case 1:
			break;
		case 2:
			DEBUG = checkFlagDebug(argv[1]);
			if (!DEBUG)
				configFile = argv[1];
			break;
		case 3:
			configFile = argv[1];
			DEBUG = checkFlagDebug(argv[2]);
			if (DEBUG){
				break;
			}
			break;
		default:
			std::cerr << RED ERROR "\nInvalid number of arguments." << std::endl;
			std::cerr << "Usage: ./webserv [configuration file]\n" << std::endl;
			return EXIT_FAILURE;
	}

	try {
		signal(SIGINT, signal_handler);
		webserver.loadConfiguration(configFile);
		webserver.createConnection();
		webserver.runWebserver();
	}
	catch (std::exception &e) {
		std::cout << RED << e.what() << RESET << std::endl;
		webserver.clear();
//			while (true){} // check leaks, delete later
	}
	return EXIT_SUCCESS;
}
