cmake_minimum_required(VERSION 3.7)
project(Client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Client.cpp Client.h SocketConnection.cpp SocketConnection.h)
add_executable(Client ${SOURCE_FILES})

if(MINGW)
    target_link_libraries(Client ws2_32)
endif()