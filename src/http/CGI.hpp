/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 13:09:04 by aheister      #+#    #+#                 */
/*   Updated: 2022/09/22 16:01:16 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <map>
#include "Response.hpp"

enum e_method
{
	GET,
	POST
};

class CGI {
public:
	CGI(const e_method method, std::string path);
	CGI(CGI const & src);
	CGI& operator=(CGI const & rhs);
	virtual ~CGI(void);

	std::string		execute_GET(std::string queryString);
	std::string		execute_POST(std::string type, std::string requestBody);

private:
	CGI(void);

	e_method		_method;
	std::string		_path;
	std::string		_queryString;
	std::string		_requestBody;
	std::string 	_type;
	char**			_arg;
	long			_length;

	std::string		execute(void);
	char			**create_envp(void);
	char			**create_args(void);
	char 			**convert_map_to_array(std::map<std::string, std::string> env_map);
	int 			execute_cgi(char *args[], int tmp_fd, char *env[]);

	//setters
	void    setQueryString(std::string queryString);
	void    setType(std::string type);
	void	setLength(long length);
	void    setRequestBody(std::string requestBody);
};
