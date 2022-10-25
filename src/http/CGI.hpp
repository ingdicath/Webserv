/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 13:09:04 by aheister      #+#    #+#                 */
/*   Updated: 2022/10/20 11:31:33 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <map>
#include "Response.hpp"
#include "../utils/utils.hpp"

enum e_method
{
	GET,
	POST
};

class CGI {
public:
	CGI(const e_method method, HttpData &httpData, std::string path);
	CGI(CGI const & src);
	CGI& operator=(CGI const & rhs);
	virtual ~CGI(void);

	int					execute_GET(std::string queryString);
	int					execute_POST(std::string type, std::string requestBody);

	//getters
	const std::string	&getBody() const;
	const int			&getErrorCode() const;

private:
	CGI(void);

	e_method		_method;
	HttpData 		_httpData;
	std::string		_path;
	std::string		_queryString;
	std::string		_requestBody;
	std::string 	_type;
	std::string		_length;

	int         	_errorCode;
	std::string		_body;

	void			prepare_execution(void);
	char			**create_envp(void);
	char			**create_args(void);
	char 			**convert_map_to_array(std::map<std::string, std::string> env_map);
	int 			execute_cgi(int fdIn, int fdOut);

	//setters
	void    		setQueryString(std::string queryString);
	void    		setType(std::string type);
	void			setLength(long length);
	void    		setRequestBody(std::string requestBody);
};
