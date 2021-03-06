cmake_minimum_required(VERSION 3.5)

set(TARGET_NAME myapp)
project(${TARGET_NAME})

## Unicode
add_definitions(-DUNICODE -D_UNICODE)

## Boost

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost)
if(NOT Boost_FOUND)
  message(SEND_ERROR "Boost is not found!!")
endif()

## BUILD

file(GLOB TARGET_SOURCES *(.c|*.h|.cc|*.hh|.cpp|.hpp|*.cxx|*.hxx))
add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${GENERATED_SOURCES})
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX _d)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES})
