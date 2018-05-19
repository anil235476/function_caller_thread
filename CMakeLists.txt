cmake_minimum_required(VERSION 3.1)

project(function_caller_thread)

add_executable(test_program
   function_caller_thread.h
   main.cpp
   )

 if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)
