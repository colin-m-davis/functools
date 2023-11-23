cmake_minimum_required(VERSION 3.1...3.27)

project(
  fp
  VERSION 1.0
  LANGUAGES CXX)

add_executable(x src/main.cpp)

target_include_directories(x PUBLIC lib)

set_target_properties(x PROPERTIES CXX_STANDARD 20)
