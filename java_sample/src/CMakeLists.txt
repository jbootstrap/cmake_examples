# https://cmake.org/cmake/help/v3.5/module/UseJava.html

cmake_minimum_required(VERSION 3.5)

find_package(Java 1.7 COMPONENTS Runtime Development)
include(UseJava)

enable_testing()

project (HelloWorld)

set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.6" "-target" "1.6")

add_jar(HelloWorld
        ENTRY_POINT HelloWorld
#        INCLUDE_JARS "E:/@Projects/cmake_examples/java_sample/lib/commons-io-2.5.jar"
        INCLUDE_JARS commons-io-2.5.jar
        SOURCES HelloWorld.java
        )

get_target_property(_jarFile HelloWorld JAR_FILE)
get_target_property(_classDir HelloWorld CLASSDIR)

message(STATUS "Jar file ${_jarFile}")
message(STATUS "Class compiled to ${_classDir}")

add_test(NAME TestHelloWorld1 COMMAND ${Java_JAVA_EXECUTABLE} -cp E:/@Projects/cmake_examples/java_sample/lib/commons-io-2.5.jar -jar ${_jarFile})
add_test(NAME TestHelloWorld2 COMMAND ${Java_JAVA_EXECUTABLE} -cp E:\\@Projects\\cmake_examples\\java_sample\\lib\\commons-io-2.5.jar -jar ${_jarFile})
