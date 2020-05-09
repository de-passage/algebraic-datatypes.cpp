cmake_minimum_required(VERSION 3.0.0)
project(category_theory VERSION 0.1.0)

add_executable(algebra main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(CPack)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    target_compile_options(algebra PRIVATE /W3 /WX)
else ()
    target_compile_options(algebra PRIVATE -std=c++17 -Wall -Wextra -pedantic)
endif(MSVC)