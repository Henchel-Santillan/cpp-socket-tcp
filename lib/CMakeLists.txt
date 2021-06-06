cmake_minimum_required(VERSION 3.17...3.20)

set(SOURCES
        src/node-ipv4.cpp
        src/tcp-client.cpp
        src/tcp-server.cpp
        include/node-ipv4.h
        include/tcp-client.h
        include/tcp-server.h)

include_directories(include)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
