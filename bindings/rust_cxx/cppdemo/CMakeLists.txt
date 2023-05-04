cmake_minimum_required(VERSION 3.16.3)

project(cppdemo)

add_library( cppdemo demo.cpp )
add_executable ( cppdemorun main.cpp )
target_link_libraries( cppdemorun cppdemo )
