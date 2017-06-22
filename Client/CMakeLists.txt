cmake_minimum_required(VERSION 3.7)
project(Client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        SocketConnection.cpp SocketConnection.h
        ClientConnection.cpp ClientConnection.h
        TunnelCommand.cpp TunnelCommand.h
        TunnelCommandResult.cpp TunnelCommandResult.h)
add_executable(Client ${SOURCE_FILES})

if(MINGW)
    target_link_libraries(Client ws2_32)
endif()