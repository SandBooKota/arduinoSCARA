cmake_minimum_required(VERSION 3.1.0)
project(arduinoSCARA)
add_executable (arduinoSCARA main.cpp
                              arduino-serial-lib.cpp
                              RobotCon.cpp
                              RobotKine.cpp)
set_property(TARGET arduinoSCARA PROPERTY CXX_STANDARD 11)
target_link_libraries(arduinoSCARA ${DEPENDENCIES} m )
