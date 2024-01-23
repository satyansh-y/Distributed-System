cmake_minimum_required(VERSION 3.16)
project(Distributed_System)

set(CMAKE_CXX_STANDARD 17)

add_executable(Distributed_System main.cpp
        machine.h
        machine.cpp
        membership.cpp)
