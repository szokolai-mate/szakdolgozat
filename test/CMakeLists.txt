find_library(LIB_PTHREAD pthread)
find_library(LIB_GTEST gtest)
find_library(LIB_PORTAUDIO portaudio)
find_library(LIB_OGG ogg)
find_library(LIB_VORBIS vorbis)
find_library(LIB_VORBISENC vorbis)

FIND_PROGRAM( GCOV_PATH gcov )
FIND_PROGRAM( LCOV_PATH lcov )
FIND_PROGRAM( GENHTML_PATH genhtml )
FIND_PROGRAM( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/tests)

file(GLOB TEST_SRC_FILES "*.cpp")
file(GLOB SRC_FILES "../src/*.cpp")
list(REMOVE_ITEM SRC_FILES "${PROJECT_SOURCE_DIR}/test/../src/main.cpp")

set(CXX_TESTS_FLAGS "-g -O0 --coverage -fno-elide-constructors -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline")
set(C_TEST_FLAGS "-g -O0 --coverage -fno-elide-constructors -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions -fno-default-inline")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CXX_TESTS_FLAGS}")
SET( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${C_TEST_FLAGS}")

add_executable(Test ${TEST_SRC_FILES} ${SRC_FILES})
set_target_properties(Test PROPERTIES EXCLUDE_FROM_ALL TRUE)

target_link_libraries(Test "${LIB_PTHREAD}")
target_link_libraries(Test "${LIB_GTEST}")
target_link_libraries(Test "${LIB_PORTAUDIO}")
target_link_libraries(Test "${LIB_OGG}")
target_link_libraries(Test "${LIB_VORBIS}")
target_link_libraries(Test "${LIB_VORBISENC}")

add_custom_target(coverage
DEPENDS Test
COMMAND echo "Creating Coverage Info"
COMMAND ${PROJECT_BINARY_DIR}/bin/Test -s
COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/doc/coverage
COMMAND ${LCOV_PATH} --directory ${PROJECT_BINARY_DIR}/test/CMakeFiles/Test.dir/ --base-directory ${PROJECT_SOURCE_DIR} --no-external --capture --output-file ${PROJECT_SOURCE_DIR}/doc/coverage/code_coverage.info -rc lcov_branch_coverage=1
COMMAND genhtml ${PROJECT_SOURCE_DIR}/doc/coverage/code_coverage.info --output-directory ${PROJECT_SOURCE_DIR}/doc/coverage
)