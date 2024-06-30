cmake_minimum_required(VERSION 3.10)
project(your_project_name)

set(CMAKE_CXX_STANDARD 17)

# Set the paths for Boost libraries
set(BOOST_ROOT "/home/ditz/boost")
set(BOOST_LIBRARYDIR "/home/ditz/boost/stage/lib")

# Find the required libraries
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads)

# Set the include directories
include_directories(include ${Boost_INCLUDE_DIRS})

# Set the source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.cc")

# Set the header files
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp" "include/*.hh")

# Create the executable
add_executable(program ${SOURCES} ${HEADERS})

# Link the required libraries
target_link_libraries(program ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# Set the compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Enable header dependency tracking
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Custom target for cleaning the build
add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/program
)