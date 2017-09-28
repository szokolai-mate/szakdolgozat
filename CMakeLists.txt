#TODO: cleanup cmake - linked libraries, test target
cmake_minimum_required(VERSION 3.6)

project(Szakdolgozat)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

subdirs(src)
subdirs(test)

include_directories(inc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_custom_target(doc
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
COMMAND echo "Generating documentation."
COMMAND doxygen Doxyfile_en
COMMAND doxygen Doxyfile_hu
)
set_target_properties(doc PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)

add_custom_target(cleanup
WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
COMMAND find . ! \\\( -name 'Mixer' -o -name 'Test' \\\) -type f -exec rm -f {} +
COMMAND find . ! \\\( -name bin -o -name . -o -name .. \\\) -type d -exec rm -rf {} +
)