/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 13:09:04 by aheister      #+#    #+#                 */
/*   Updated: 2022/09/19 15:29:56 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <map>
#include "Response.hpp"

class CGI {
public:
	CGI(void);
	//CGI(CGI const & src);
	//CGI& operator=(CGI const & rhs);
	virtual ~CGI(void);

	void								addPath(std::string fileName, Location ServerLocation);
	std::string							execute(void);
	std::map<std::string, std::string>	fill_env_map(void);
	char 								**fill_env(void);
	int 								execute_cgi(char *arg, int tmp_fd, char *env[]);

private:
	std::string		_file;
	Location		_serverLocation;
};
