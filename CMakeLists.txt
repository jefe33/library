cmake_minimum_required(VERSION 3.17)
project(Projekt C)

set(CMAKE_C_STANDARD 11)

add_executable(Projekt index.c)

add_executable(losoweKsiazki losoweKsiazki.h)
add_executable(ksiazki ksiazki.h)