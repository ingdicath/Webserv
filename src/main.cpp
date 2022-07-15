
#include "Webserver.hpp"
#include "Server.hpp"
#include "settings.hpp"

int     main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << RED "\nInvalid number of arguments" RESET << std::endl;
		std::cout << "Run webserver by typing in: ./webserve [configuration_file]\n" << std::endl;
		return EXIT_FAILURE;
	}
	else {
		Webserver webserver;

		try {
			webserver.loadConfiguration();
			webserver.createConnection();
			webserver.runWebserver();
		}
		catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}
	}
	return 0;
}