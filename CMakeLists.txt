cmake_minimum_required(VERSION 3.13)
project(semaphore_hello_world C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

include_directories(.)

add_executable(semaphore_hello_world main.c)