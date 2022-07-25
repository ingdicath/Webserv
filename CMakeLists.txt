cmake_minimum_required(VERSION 3.22)
project(Webserv)

set(CMAKE_CXX_STANDARD 11)

include_directories(includes)

add_executable(Webserv src/main.cpp
        src/utils.cpp src/Server.cpp
        src/Configurator.cpp src/Location.cpp src/Webserver.cpp includes/Webserver.hpp)

# for testing

enable_testing()
add_subdirectory(tests)