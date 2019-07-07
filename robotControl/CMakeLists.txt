cmake_minimum_required(VERSION 2.6)

project(arduinoSCARA)

add_executable(link2SCARA link2SCARA.c arduino-serial-lib.c)

target_link_libraries(link2SCARA m)
