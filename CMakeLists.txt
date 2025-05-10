cmake_minimum_required(VERSION 3.14)
project(BD_YATP)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include/utils)
include_directories(include/BD_YATP)

# Source files
file(GLOB MAIN_SRC main/*.cpp)
file(GLOB CORE_SRC src/*.cpp)
file(GLOB UTILS_SRC src/utils/*.cpp)
file(GLOB TEST_SRC test/*.cpp)

add_executable(BD_YATP ${MAIN_SRC} ${CORE_SRC} ${UTILS_SRC})


add_executable(BD_YATP_tests ${TEST_SRC} ${CORE_SRC} ${UTILS_SRC}) 
target_include_directories(BD_YATP_tests PRIVATE include/utils include/BD_YATP)