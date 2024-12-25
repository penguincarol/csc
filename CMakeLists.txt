cmake_minimum_required(VERSION 3.22)
project(csc)

set(CMAKE_CXX_STANDARD 23)

# Add Kissat directory
set(KISSAT_DIR ${CMAKE_SOURCE_DIR}/kissat_folder/kissat)

# Custom target to build Kissat
#add_custom_target(kissat_build
#        COMMAND ./configure && make
#        WORKING_DIRECTORY ${KISSAT_DIR}
#        COMMENT "Building Kissat solver"
#)

include_directories(${KISSAT_DIR}/src)
include_directories(${KISSAT_DIR}/kissat)

# Add executable
add_executable(csc main.cpp AdjacencyMatrix.cpp
        Ede.cpp
        GraphToSAT.h
        GraphToSAT.cpp
        KemenySolver.h
        KemenySolver.cpp)

# Link Kissat library
target_link_libraries(csc PRIVATE ${KISSAT_DIR}/build/libkissat.a)

# Ensure Kissat is built before linking to the main program
#add_dependencies(csc kissat_build)
