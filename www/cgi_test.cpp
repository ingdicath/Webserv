#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <map>

int execute_cgi(char *arg, int tmp_fd, char *env[])
{
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	execve(arg, NULL, env);
    // if file cannot be executed than show errorpage
    return (1);
}

// which input does the cgi need to fill the environment variables:
// input from server AND from request;
// the env are filled depending on the input of the request. 
// If it is a POST request: CONTENT_TYPE and CONTENT_LENGTH are filled
// If it is a GET request: QUERY_STRING is filled
// find out where path can be found to execute file: is that the root + cgi??

std::map<std::string, std::string> fill_env_map(void)
{
    std::string file_path = getcwd(NULL, 0);
    file_path = file_path + "/www/" ;
    std::cout << file_path << std::endl;

    std::map<std::string, std::string> env_map;
    
    env_map["GATEWAY_INTERFACE"] = "CGI/1.1";                       //
//if method = POST
    //env_map["CONTENT_TYPE"] = "";                                 //
    //env_map["CONTENT_LENGTH"] = "";                               //

    env_map["PATH_INFO"] = "";                                      //path suffix, if appended to URL after program name and a slash
    env_map["PATH_TRANSLATED"] = "";                                //

//if (method = "GET")
    env_map["QUERY_STRING"] = "num1=4&&num2=7";                     //

    env_map["REMOTE_ADDR"] = "";
    env_map["REMOTE_IDENT"] = "";                                   //IP address of the client (dot-decimal)
    env_map["REQUEST_URI"] = "/cgi-bin/add.py?num1=4&&num2=7";      //the part of URL after ? character
    env_map["REQUEST_METHOD"] = "GET";                              //
    env_map["SCRIPT_NAME"] = "/cgi-bin/add.py";                     //relative path to the program, like /cgi-bin/script.cgi
    env_map["SCRIPT_FILENAME"] = file_path;                         //
    env_map["SERVER_SOFTWARE"] = "webserv/1.0";                     //name/version of HTTP server
    env_map["SERVER_PROTOCOL"] = "HTTP/1.1";                        //HTTP/version
    env_map["SERVER_NAME"] = "127.0.0.1";                           //host name of the server, may be dot-decimal IP address
    env_map["SERVER_PORT"] = "80";                                  //TCP port (decimal)

    return (env_map);
}

char **fill_env(void)
{
    int i;
    char **envp;
    std::string env_var;
    std::map<std::string, std::string> env_map;

    envp = NULL;
    env_map = fill_env_map();
    envp = (char **)malloc((env_map.size() + 1) * sizeof(char *));
    // walk through map
    i = 0;
    for (std::map<std::string, std::string>::const_iterator it = env_map.begin(); it != env_map.end(); ++it)
    {
        envp[i] = strdup((it->first + "=" + it->second).c_str());
		if (envp[i] == NULL)
			throw("error happened while mallo in set_env() to cgi");
		i++;
	}
	envp[i] = NULL;
    return (envp);
}

// execve prints in the stdout: that means that the response part should pick it up there

int main(int argc, char *argv[])
{
	int pid;
	int tmp_fd;
    //char *path;
    (void)argc;

    pid = 0;
	tmp_fd = dup(STDIN_FILENO);
    pid = fork();
    if (pid == 0) // this is the child
    {
        //path = argv[1];
        if (execute_cgi(argv[1], tmp_fd, fill_env()))
            return (1);
    }
    else
    {
        close(tmp_fd);
        while (waitpid(-1, NULL, WUNTRACED) != -1)
            ;
        tmp_fd = dup(STDIN_FILENO);
    }
    close(tmp_fd);
    return (0);
}
