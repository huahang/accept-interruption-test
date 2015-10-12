cmake_minimum_required(VERSION 2.8)

project(accept_interruption_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(SOURCE_FILES main.cpp)
add_executable(accept_interruption_test ${SOURCE_FILES})
