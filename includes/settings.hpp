//
// Created by Diana catherine Salamanca leguizamon on 5/12/22.
//

#pragma once

/** configuration file path **/
#define DEFAULT_CONFIG_FILE "config_files/default.conf"

/** colors **/
#define RESET   "\033[0m"
#define ITALIC  "\033[3m"
# define GREEN_2 "\033[38;5;10m"
#define BLACK   "\033[30m"
#define CYAN_L  "\033[0;36m"
#define RED     "\033[31m"
# define ORANGE "\033[38;5;172m"
# define PURPLE "\033[35;5;95m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

/** Default server values **/
#define DEFAULT_CLIENT_MAX_BODY_SIZE 10000000 // 10MB
#define DEFAULT_LISTEN 80
#define DEFAULT_SERVER_NAME ""
#define DEFAULT_ERROR_PAGE 404
#define MAX_PORT_NUMBER 65535
#define OFF 0
#define ON 1
