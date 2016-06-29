# https://cmake.org/cmake/help/v3.5/module/UseJava.html

cmake_minimum_required(VERSION 3.5)

find_package(Java REQUIRED)
include(UseJava)

enable_testing()

project (HelloWorld)

set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.6" "-target" "1.6")

add_jar(HelloWorld
        ENTRY_POINT HelloWorld
        SOURCES HelloWorld.java)

get_target_property(_jarFile HelloWorld JAR_FILE)
get_target_property(_classDir HelloWorld CLASSDIR)

message(STATUS "Jar file ${_jarFile}")
message(STATUS "Class compiled to ${_classDir}")

add_test(NAME TestHelloWorld1 COMMAND ${Java_JAVA_EXECUTABLE} -cp ${_jarFile} HelloWorld)
add_test(NAME TestHelloWorld2 COMMAND ${Java_JAVA_EXECUTABLE} -jar ${_jarFile})
