cmake_minimum_required(VERSION 3.1)

project(function_caller_thread)

add_executable(test_program
   function_caller_thread.h
   thread_message_loop.h
   thread_message_loop.cpp
   main.cpp
   msg_loop_libraray.h
   test_msg_loop_lib.cpp
   )

 if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)
