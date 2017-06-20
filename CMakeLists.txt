cmake_minimum_required(VERSION 3.6)
project(szakdolgozat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

LINK_DIRECTORIES(/usr/include /usr/include/c++/6 /usr/local/include /lib /usr/lib /usr/local/lib)

set(SOURCE_FILES main.cpp dummyLoader.cpp)
add_executable(szakdolgozat ${SOURCE_FILES} dummyLoader.cpp)
TARGET_LINK_LIBRARIES(szakdolgozat ogg vorbis portaudio pthread)
