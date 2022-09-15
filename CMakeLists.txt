cmake_minimum_required(VERSION 3.22)
project(Webserv)

set(CMAKE_CXX_STANDARD 11)

include_directories(includes)

add_executable(Webserv src/main.cpp
        src/configs/Parser.cpp
        src/configs/FileUtils.cpp
        src/configs/Parser.hpp
        src/configs/FileUtils.hpp
        src/http/Request.cpp
        src/http/Request.hpp
        src/http/Response.cpp
        src/http/Response.hpp
        src/http/HttpData.cpp
        src/http/HttpData.hpp
        src/utils/settings.hpp
        src/utils/utils.hpp
        src/utils/utils.cpp
        src/webserver/Client.cpp src/webserver/Client.hpp
        src/webserver/Location.cpp src/webserver/Location.hpp
        src/webserver/Server.cpp src/webserver/Server.hpp
        src/webserver/Webserver.cpp src/webserver/Webserver.cpp)
