
#include <csignal>
#include <cstdlib>

#include "webserver/Webserver.hpp"
#include "utils/settings.hpp"

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << GREEN "\b\bWebserver stopped" RESET << std::endl;
		exit(EXIT_SUCCESS);
	}
}

int checkFlagDebug(char **argv, int *debug){
    *debug = 0;
    if (strcmp(argv[0], "--debug") == 0){
        *debug = 1;
        return *debug;
    } else{
        std::cerr << RED ERROR " Invalid argument for debug. Usage: --debug" RESET << std::endl;
        exit(EXIT_FAILURE);
    }
}

int     main(int argc, char* argv[]) {
    int debug;
	std::string configFile;
	Webserver webserver;

	switch (argc) {
		case 1:
			configFile = DEFAULT_CONFIG_FILE;
			break;
		case 2:
			configFile = argv[1];
			break;
        case 3:
            configFile = argv[1];
            debug = checkFlagDebug(&argv[2], &debug);
            break;
		default:
			std::cerr << RED "\nInvalid number of arguments." RESET << std::endl;
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
