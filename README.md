![](https://github.com/ingdicath/images_md/blob/main/Webserver_Banner.png)


# **Webserv**
CODAM project Curriculum.


This project is about writing an HTTP server using C++98 standard.  HTTP is one of the most used protocols on the internet.

This is a group project developed by [A. Heister](https://github.com/aheister70), [H. Lin](https://github.com/Hz-Lin) & [D. Salamanca](https://github.com/ingdicath)

----
## Requirements

Some requirements for this project included:
- Your program has to take a configuration file as argument, or use a default path.
- It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O
  operations between the client and the server (listen included).
- A request to your server should never hang forever.
- You server must have default error pages if none are provided.
- You must be able to serve a fully static website.
- Clients must be able to upload files.
- You need at least GET, POST, and DELETE methods.
- Your server must be able to listen to multiple ports:
  - Choose the port and host of each "server". 
  - Set up the server_names or not.
  - The first server for a `host:port` will be the default for this `host:port`
  - Setup default error pages.
  - Limit client body size.
  - Setup routes with one or multiple of the following rules/configuration (routes won't
       be using regexp):
    - Define a list of accepted HTTP methods for the route.
    - Define an HTTP redirection.
    - Define a directory or a file from where the file should be searched.
    - Turn on or off directory listing.
    - Set a default file to answer if the request is a directory.
    - Execute CGI based on certain file extension (for example .php).

----
## Prerequisites

Be sure you have `siege` installed in your computer for the stress tests.


---
## How to build :hammer:

1. Clone the repository `git clone git@github.com:ingdicath/Webserv.git`
2. Go into folder ```cd Webserv```
3. Type in the terminal ```make```

### Usage of _webserver_

<br >1. Without printing out request and response headers:
   - Using a configuration file included by default in the program:  `./webserv`
   - Using other configuration file: `./webserv [configuration file]`

<br >2. Printing out request and response headers on the terminal using a debug flag:
   - For the default configuration file:
   `./webserv --debug`
   - For other configuration file:
      `./webserv [configuration file] --debug`
