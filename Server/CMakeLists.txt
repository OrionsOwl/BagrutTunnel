cmake_minimum_required(VERSION 3.7)
project(Server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        SocketConnection.cpp SocketConnection.h
		ServerConnection.cpp ServerConnection.h
		TunnelHandler.cpp TunnelHandler.h
        TunnelCommand.cpp TunnelCommand.h
		TunnelCommandResult.cpp TunnelCommandResult.h
        ConnectionHandler.cpp ConnectionHandler.h
        ConnectionHandlers.cpp ConnectionHandlers.h)
#set(SOURCE_FILES main.cpp Server.cpp Server.h SocketConnection.cpp SocketConnection.h SSHConnection.cpp SSHConnection.h)
add_executable(Server ${SOURCE_FILES})

if(MINGW)
    target_link_libraries(Server ws2_32)
endif()