project (Dragon)
cmake_minimum_required(VERSION 2.8)

set(SOURCES
  src/lexer.cpp
)

add_definitions (-std=c++11)

add_library(dragon ${SOURCES})

add_executable(test test/main.cpp)
target_link_libraries(test dragon)