cmake_minimum_required(VERSION 3.17..3.20)
project(cpp_socket_tcp
        VERSION 1.0
        DESCRIPTION "TCP Server-Client Implementation for IPv4"
        LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified: setting build type to 'Debug'")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
include_directories(lib/include)
add_executable(client client-app.cpp)
add_executable(server server-app.cpp)

target_link_libraries(client src)
target_link_libraries(server src)

install(TARGETS client DESTINATION bin)
install(TARGETS server DESTINATION bin)