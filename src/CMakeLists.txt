find_library(LIB_PTHREAD pthread)
find_library(LIB_PORTAUDIO portaudio)
find_library(LIB_OGG ogg)
find_library(LIB_VORBIS vorbis)
find_library(LIB_VORBISENC vorbisenc)

file(GLOB SRC_FILES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} "${LIB_PTHREAD}")
target_link_libraries(${PROJECT_NAME} "${LIB_PORTAUDIO}")
target_link_libraries(${PROJECT_NAME} "${LIB_OGG}")
target_link_libraries(${PROJECT_NAME} "${LIB_VORBIS}")
target_link_libraries(${PROJECT_NAME} "${LIB_VORBISENC}")

file(GLOB HEADERS "../inc/*.h")
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)