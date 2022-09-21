/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 12:28:01 by aheister      #+#    #+#                 */
/*   Updated: 2022/09/21 15:42:15 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "../webserver/Location.hpp"

// TODO:    - Correct arguments execve function
//          - Return error code if file cannot be executed or if other functions in CGI object fail
//              - 500 if system function fails
//              - 403 if program can't be executed
//          - Handle memory management of array env
//          - Find out how to use the other environment variables

CGI::CGI(const e_method method, const std::string path, const std::string queryString) {
    _method = method;
    _file = path;
    _queryString = queryString;
}

CGI::CGI(CGI const &src) {
	*this = src;
}

CGI& CGI::operator=(CGI const & rhs) {
    if (this != &rhs) {
        _method = rhs._method;
        _file = rhs._file;
        _queryString = rhs._queryString;
    }
    return *this;
}

CGI::~CGI() {
}

int CGI::execute_cgi(char *arg, int tmp_fd, char *env[])
{
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
    // TODO: second argument can't be NULL: int execve(const char *pathname, char *const argv[],char *const envp[]);
	execve(arg, NULL, env);
    // if file cannot be executed than show errorpage
    // if it fails the child must exit
    exit (1);
}

// which input does the cgi need to fill the environment variables:
// If it is a POST request: CONTENT_TYPE and CONTENT_LENGTH are filled

char **CGI::create_envp(void)
{
    std::map<std::string, std::string> env_map;

if (this->_method == GET)
    env_map["QUERY_STRING"] = this->_queryString;
//if method = POST
    //env_map["CONTENT_TYPE"] = "";
    //env_map["CONTENT_LENGTH"] = "";  
    env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
    env_map["PATH_INFO"] = "";                                      //path suffix, if appended to URL after program name and a slash
    env_map["PATH_TRANSLATED"] = "";              
    env_map["REMOTE_ADDR"] = "";
    env_map["REMOTE_IDENT"] = "";                                   //IP address of the client (dot-decimal)
    env_map["REQUEST_URI"] = "";                                    //the part of URL after ? character
    env_map["REQUEST_METHOD"] = "GET";                              //
    env_map["SCRIPT_NAME"] = "";                                    //relative path to the program, like /cgi-bin/script.cgi
    env_map["SCRIPT_FILENAME"] = "";                                //
    env_map["SERVER_SOFTWARE"] = "webserv/1.0";                     //name/version of HTTP server
    env_map["SERVER_PROTOCOL"] = "HTTP/1.1";                        //HTTP/version
    env_map["SERVER_NAME"] = "127.0.0.1";                           //host name of the server, may be dot-decimal IP address
    env_map["SERVER_PORT"] = "8085";                               //TCP port (decimal)

    return (convert_map_to_array(env_map));
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
		if (envp[i] == NULL)
			throw("error happened");
		i++;
	}
	envp[i] = NULL;
    return (envp);
}

std::string CGI::execute(void) {
    int         ret;
    int         pid;
	int         tmp_fd;
    int         fd[2];
    std::string body;
    char        **envp;

    pid = 0;
	tmp_fd = dup(STDIN_FILENO);
    pipe(fd);
    pid = fork();
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
        envp = create_envp();
        if (envp)
            execute_cgi(const_cast<char*>(_file.c_str()), tmp_fd, envp);
        //else
            //HIER NOG FOUTMELDING
    }
    else
    {
        char	buffer[CGI_BUFSIZE] = {0};

		ret = 1;
        while(waitpid(-1, NULL, WUNTRACED) != -1)
					;
        close(fd[1]);
        close(tmp_fd);
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fd[0], buffer, CGI_BUFSIZE - 1);
			body += buffer;
		}
        close(fd[0]);
    }
    return (body);
}
