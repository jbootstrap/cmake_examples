cmake_minimum_required(VERSION 3.5)

project(multi_exe)

## Unicode
add_definitions(-DUNICODE -D_UNICODE)

## BUILD

add_executable(app1 app1.cpp unicode_dummy.cpp)
set_target_properties(app1 PROPERTIES DEBUG_POSTFIX _d)

add_executable(app2 app2.cpp unicode_dummy.cpp)
set_target_properties(app2 PROPERTIES DEBUG_POSTFIX _d)
