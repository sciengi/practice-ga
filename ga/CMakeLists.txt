
cmake_minimum_required(VERSION 3.30)

project(practice-ga     VERSION 0.1
                        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

include_directories("inc")
file(GLOB SOURCES "src/*.cpp")

add_library(gae STATIC ${SOURCES})
add_executable(demo main.cpp)
target_link_libraries(demo PUBLIC gae)

