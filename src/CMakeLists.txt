find_library(LIB_PTHREAD pthread)

file(GLOB SRC_FILES "*.cpp")

add_executable(Mixer ${SRC_FILES})

target_link_libraries(Mixer "${LIB_PTHREAD}")