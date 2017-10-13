find_library(LIB_PTHREAD pthread)
find_library(LIB_PORTAUDIO portaudio)
find_library(LIB_OGG ogg)
find_library(LIB_VORBIS vorbis)
find_library(LIB_VORBISENC vorbisenc)

file(GLOB SRC_FILES "*.cpp")

add_executable(Mixer ${SRC_FILES})

target_link_libraries(Mixer "${LIB_PTHREAD}")
target_link_libraries(Mixer "${LIB_PORTAUDIO}")
target_link_libraries(Mixer "${LIB_OGG}")
target_link_libraries(Mixer "${LIB_VORBIS}")
target_link_libraries(Mixer "${LIB_VORBISENC}")