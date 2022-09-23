/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 12:28:01 by aheister      #+#    #+#                 */
/*   Updated: 2022/09/23 09:58:27 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "../webserver/Location.hpp"

// TODO:    - Return error code if file cannot be executed or if other functions in CGI object fail
//              - 500 if system function fails
//              - 502 if program can't be executed
//          - Handle memory management of array env
//          - Handle memory management of array args
//          - Find out how to fill the other environment variables based on client and serverinfo
//			- Remove type from result of cgi
//			- Make the errorpages better: with link to homepage
//			- Handle UPLOAD?? (don't know if that is done here)
//			- Think of better example to make execute fail

CGI::CGI(const e_method method, const std::string path) {
	_method = method;
	_path = path;
}

CGI::CGI(CGI const &src) {
	*this = src;
}

CGI& CGI::operator=(CGI const & rhs) {
	if (this != &rhs) {
		_method = rhs._method;
		_path = rhs._path;
		_queryString = rhs._queryString;
	}
	return *this;
}

CGI::~CGI() {
}

int CGI::execute_cgi(char *args[], int tmp_fd, char *env[])
{
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	//(void)args;
	//(void)env;
	execve(args[0], args, env);
	exit(1);
}

char **CGI::create_envp(void)
{
	std::map<std::string, std::string> env_map;

	if (this->_method == GET)
		env_map["QUERY_STRING"] = this->_queryString;
	if (this->_method == POST) {
		env_map["CONTENT_TYPE"] = this->_type;
		env_map["CONTENT_LENGTH"] = _requestBody.size();
	} 
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["PATH_INFO"] = "";                                      //path suffix, if appended to URL after program name and a slash
	env_map["PATH_TRANSLATED"] = "";              
	env_map["REMOTE_ADDR"] = "";
	env_map["REMOTE_IDENT"] = "";                                   //IP address of the client (dot-decimal)
	env_map["REQUEST_URI"] = "";                                    //the part of URL after ? character
	env_map["REQUEST_METHOD"] = "GET";                              //
	env_map["SCRIPT_NAME"] = "";                                    //relative path to the program, like /cgi-bin/script.cgi
	env_map["SCRIPT_FILENAME"] = "";                                //
	env_map["SERVER_SOFTWARE"] = "";                                //name/version of HTTP server
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";                        //HTTP/version
	env_map["SERVER_NAME"] = "127.0.0.1";                           //host name of the server, may be dot-decimal IP address
	env_map["SERVER_PORT"] = "8085";                                //TCP port (decimal)

	return (convert_map_to_array(env_map));
}

char **CGI::create_args(void) {
	char **args;
	int size;

	if (_method == POST)
		size = 3;
	else	
		size = 2;
	args = (char **)malloc(size * sizeof(char *));
	args[0] = const_cast<char*>(_path.c_str());
	if (_method == POST) {
		args[1] = const_cast<char*>(_requestBody.c_str());
		args[2] = NULL;
	}
	else
		args[1] = NULL;
	return(args);
}

char **CGI::convert_map_to_array(std::map<std::string, std::string> env_map)
{
	int i;
	char **envp;
	std::string env_var;

	envp = NULL;
	envp = (char **)malloc((env_map.size() + 1) * sizeof(char *));
	i = 0;
	for (std::map<std::string, std::string>::const_iterator it = env_map.begin(); it != env_map.end(); ++it)
	{
		envp[i] = strdup((it->first + "=" + it->second).c_str());
		// if (envp[i] == NULL)
		// 	throw("error happened");
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

std::string	CGI::execute_GET(std::string queryString) {
	std::string body;

	setQueryString(queryString);
	body = execute();
	return (body);
}

std::string	CGI::execute_POST(std::string type, std::string requestBody) {
	std::string body;

	setType(type);
	setLength(requestBody.size());
	setRequestBody(requestBody);
	body = execute();
	return (body);
}

std::string CGI::execute(void) {
	int         ret;
	int         pid;
	int         tmp_fd;
	int         fd[2];
	std::string body;
	char        **envp;
	char		**args;

	pid = 0;
	envp = create_envp();
	// error if envp = NULL
	args = create_args();
	// error if args = NULL
	tmp_fd = dup(STDIN_FILENO);
	// error if dup fails
	pipe(fd);
	// error if pipe fails
	pid = fork();
	// error if fork fails
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		// error if dup fails
		close(fd[0]);
		close(fd[1]);
		execute_cgi(args, tmp_fd, envp);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};
		int		status;

		ret = 1;
		while(waitpid(-1, &status, WUNTRACED) != -1)
			;
		if (WIFEXITED(status)) {
        	int es = WEXITSTATUS(status);
        	std::cout << "Exit status was " << es << std::endl;
			if (es == 1) {
				body = "500";
				return (body);
			}
    	}
		close(fd[1]);
		close(tmp_fd);
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fd[0], buffer, CGI_BUFSIZE - 1);
			// error if read fails??
			// think about checking fds for select here? Possible?
			body += buffer;
		}
		close(fd[0]);
	}
	return (body);
}

// setters

void    CGI::setQueryString(std::string queryString) {
	this->_queryString = queryString;
}

void    CGI::setType(std::string type) {
	this->_type = type;
}

void    CGI::setLength(long length) {
	this->_length = length;
}

void    CGI::setRequestBody(std::string requestBody) {
	this->_requestBody = requestBody;
}
