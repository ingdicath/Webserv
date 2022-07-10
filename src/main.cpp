
#include "../includes/Webserver.hpp"
#include "../includes/Server.hpp"

int     main(void) {
	Webserver webserver;

	// TODO: add a check if there is a configuration file added as an argument with the compilation of the program
	try {
		webserver.loadConfiguration();
		webserver.createConnection();
		webserver.runWebserver();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}