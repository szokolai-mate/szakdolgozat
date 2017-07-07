find_library(LIB_PTHREAD pthread)
find_library(LIB_PORTAUDIO portaudio)

file(GLOB SRC_FILES "*.cpp")

add_executable(Mixer ${SRC_FILES})

target_link_libraries(Mixer "${LIB_PTHREAD}")
target_link_libraries(Mixer "${LIB_PORTAUDIO}")