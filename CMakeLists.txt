cmake_minimum_required(VERSION 3.30.0)
project(sbd VERSION 0.1.0 LANGUAGES C)
set (CMAKE_C_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG  "-g")

file(GLOB source_files CONFIGURE_DEPENDS "src/*.h" "src/*.c")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -lm -DDEBUG")

add_executable(sbd ${source_files})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)