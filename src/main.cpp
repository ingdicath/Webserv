
#include "Webserver.hpp"
#include "Server.hpp"
#include "settings.hpp"

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << GREEN "\b\bWebserver stopped" RESET << std::endl;
		exit(EXIT_SUCCESS);
	}
}

int     main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << RED "\nInvalid number of arguments" RESET << std::endl;
		std::cout << "Run webserver by typing in: ./webserve [configuration_file]\n" << std::endl;
		return EXIT_FAILURE;
	}
	else {
		Webserver webserver;

		try {
			signal(SIGINT, signal_handler);
			webserver.loadConfiguration();
			webserver.createConnection();
			webserver.runWebserver();
		}
		catch (std::exception &e) {
			std::cout << RED << e.what() << RESET << std::endl;
			webserver.clear();
		}
	}
	return EXIT_SUCCESS;
}