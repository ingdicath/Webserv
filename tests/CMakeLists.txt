project(tests)

set(CMAKE_CXX_STANDARD 11)

add_executable(tests integration_tests.cpp
        ../src/utils.cpp ../src/Server.cpp
        ../src/Configurator.cpp ../src/Location.cpp)
