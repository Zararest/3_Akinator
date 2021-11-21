cmake_minimum_required(VERSION 3.0.0)
project(Akinator VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Akinator src/main.cpp src/headers/Akinator.hpp src/headers/Debug.hpp src/headers/Tree.hpp src/Akinator.cpp src/Debug.cpp src/Tree.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
