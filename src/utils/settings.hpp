/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   settings.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: diana <diana@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 08:55:10 by diana         #+#    #+#                 */
/*   Updated: 2022/10/25 08:55:10 by diana         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/** configuration file path **/
#define DEFAULT_CONFIG_FILE "config_files/default.conf"
#define ERROR "[ERROR]"

/** colors **/
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define ITALIC  "\033[3m"
#define PURPLE "\033[35;5;95m"
#define YELLOW  "\033[33m"
#define GRAY "\033[2m\033[37m"

/** Default range port values **/
#define MIN_PORT_NUMBER 1
#define MAX_PORT_NUMBER 65535

/** Default server values **/
#define DEFAULT_PORT 80
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_CLIENT_MAX_BODY_SIZE 10000000 // 10 Megabytes
#define DEFAULT_ROOT "/www"
#define DEFAULT_INDEX "index.html"
#define DEFAULT_SERVER_NAME ""
#define DEFAULT_BACKLOG 1000
#define DEFAULT_TIMEOUT 60 // added: 60 seconds is default nginx timeout
#define MAXLINE 4096
#define CGI_BUFSIZE 65536

/** Global variable for DEBUG **/
extern int DEBUG;
