cmake_minimum_required(VERSION 2.6)

project(arduinoSCARA)

add_executable(axis4SCARA axis4SCARA.c arduino-serial-lib.c)

target_link_libraries(axis4SCARA m)
