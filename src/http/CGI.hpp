/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 13:09:04 by aheister      #+#    #+#                 */
/*   Updated: 2022/09/21 15:32:18 by aheister      ########   odam.nl         */
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

enum e_method
{
	GET,
	POST
};

class CGI {
public:
	CGI(const e_method method, const std::string path, const std::string queryString);
	CGI(CGI const & src);
	CGI& operator=(CGI const & rhs);
	virtual ~CGI(void);

	std::string		execute(void);
	char			**create_envp(void);
	char 			**convert_map_to_array(std::map<std::string, std::string> env_map);
	int 			execute_cgi(char *arg, int tmp_fd, char *env[]);

private:
	CGI(void);

	e_method		_method;
	std::string		_file;
	std::string		_queryString;
	Location		_serverLocation;
};
