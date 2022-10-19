/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 12:28:01 by aheister      #+#    #+#                 */
/*   Updated: 2022/10/19 11:23:20 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include <stdio.h>
#include <cstring>
#include "../webserver/Location.hpp"

// TODO:	- add a check for select to the cgi

CGI::CGI(const e_method method, HttpData &httpData, const std::string path) 
	: 	_method(method),
		_httpData(httpData),
		_path(path),
		_errorCode(200)	{
}

CGI::CGI(CGI const &src) {
	*this = src;
}

CGI& CGI::operator=(CGI const & rhs) {
	if (this != &rhs) {
		_method = rhs._method;
		_httpData = rhs._httpData;
		_path = rhs._path;
		_errorCode = rhs._errorCode;
		if (_method == GET)
			_queryString = rhs._queryString;
		if (_method == POST) {
			_requestBody = rhs._requestBody;
			_type = rhs._type;
			_length = rhs._length;
		}
	}
	return *this;
}

CGI::~CGI() {
}

char **CGI::convert_map_to_array(std::map<std::string, std::string> env_map)
{
	int i;
	char **envp;
	std::string env_var;

	envp = NULL;
	envp = (char **)malloc((env_map.size() + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	for (std::map<std::string, std::string>::const_iterator it = env_map.begin(); it != env_map.end(); ++it)
	{
		envp[i] = strdup((it->first + "=" + it->second).c_str());
		if (envp[i] == NULL)
			exit (1);
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

char **CGI::create_envp(void)
{
	std::map<std::string, std::string> env_map;

	if (this->_method == GET) {
		env_map["QUERY_STRING"] = this->_queryString;
		env_map["REQUEST_METHOD"] = "GET";
	}
	else if (this->_method == POST) {
		env_map["CONTENT_TYPE"] = this->_type;
		env_map["CONTENT_LENGTH"] = this->_length;
		env_map["REQUEST_METHOD"] = "POST";
	}
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["PATH_INFO"] = "/cgi-bin/" + this->_path.substr(this->_path.find_last_of("/"));
	env_map["PATH_TRANSLATED"] = "/cgi-bin/" + this->_path.substr(this->_path.find_last_of("/"));
	env_map["REMOTE_ADRESS"] = "127.0.0.1";
	env_map["SCRIPT_NAME"] = "/cgi-bin/" + this->_path.substr(this->_path.find_last_of("/"));
	env_map["SERVER_SOFTWARE"] = "Connecting kittens/1.0";
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["SERVER_NAME"] = _httpData.getServerName();
	std::stringstream streamPort;  
    streamPort << _httpData.getPort();
	env_map["SERVER_PORT"] = streamPort.str();

	return (convert_map_to_array(env_map));
}

char **CGI::create_args(void) {
	char **args;

	args = NULL;
	args = (char **)malloc(2 * sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = strdup(_path.c_str());
	args[1] = NULL;
	return(args);
}

int	CGI::execute_GET(std::string queryString) {

	setQueryString(queryString);
	execute();
	return (this->_errorCode);
}

int	CGI::execute_POST(std::string type, std::string requestBody) {
	setType(type);
	setLength(requestBody.size());
	setRequestBody(requestBody);
	execute();
	return (this->_errorCode);
}

int CGI::execute_cgi(int fdIn, int fdOut)
{
	char	**env;
	char	**args;

	if (dup2(fdIn, STDIN_FILENO) < 0 || dup2(fdOut, STDOUT_FILENO) < 0)
		exit(1);
	env = create_envp();
	args = create_args();
	if (!args || !env)
		exit(1);
	execve(args[0], args, env);
	exit(2);
}

void CGI::execute(void) {
	int         ret;
	pid_t       pid;
	int         fd[2];
	std::string body;

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);

	// tmpfile() creates a temporary binary file, with a filename guaranteed to be different from any other existing file.
	// The temporary file created is automatically deleted when the stream is closed (fclose).
	FILE	*fileIn = tmpfile();
	FILE	*fileOut = tmpfile();
		
	// The fileno() function returns the integer file descriptor associated with the stream pointed to by stream.
	int	fdIn = fileno(fileIn);
	int	fdOut = fileno(fileOut);

	if (fd[0] < 0 || fd[1] < 0 || fdIn < 0 || fdOut < 0) {
		_errorCode = 500;
		return;
	}

	write(fdIn, _requestBody.c_str(), _requestBody.size());
	// Set the offset (2nd argument in bites) so in this case to the beginning.
	lseek(fdIn, 0, SEEK_SET);

	pid = fork();
	if (pid < 0) {
		_errorCode = 500;
		return;
	}
	if (pid == 0) {
		execute_cgi(fdIn, fdOut);
	}
	else
	{
		char		buffer[CGI_BUFSIZE] = {0};
		pid_t		status;

		while (waitpid(-1, &status, WUNTRACED) != -1)
			;
		int code = WEXITSTATUS(status);
		if (code == 1)
			_errorCode = 500;
		else if (code == 2)
			_errorCode = 502;
		ret = 1;
		lseek(fdOut, 0, SEEK_SET);
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
			if (ret < 0)
				_errorCode = 500;
			_body += buffer;
		}
	}
	if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(fd[1], STDOUT_FILENO) < 0)
		_errorCode = 500;
	if (fclose(fileIn) < 0 || fclose(fileOut) < 0)
		_errorCode = 500;
	if (close(fd[0]) < 0 || close(fd[1]) < 0)
		_errorCode = 500;
	// fdIn and fdOut are already 'closed' in the child process (fd's are shared amongst threads).
}

// setters
void    CGI::setQueryString(std::string queryString) {
	this->_queryString = queryString;
}

void    CGI::setType(std::string type) {
	this->_type = type;
}

void    CGI::setLength(long length) {
	std::stringstream stream;  
    stream << length;  
	this->_length = stream.str();
}

void    CGI::setRequestBody(std::string requestBody) {
	this->_requestBody = requestBody;
}

// getters
const std::string	&CGI::getBody() const {
	return (_body);
}

const int	&CGI::getErrorCode() const {
	return (_errorCode);
}
