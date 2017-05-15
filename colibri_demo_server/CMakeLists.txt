
# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8.9)

#set c++ standard to 11
SET(CMAKE_CXX_FLAGS "-std=c++11")


#Project's name
project(colibri_demo)

#Specify directories in which to find h files
include_directories(include)


#The file(GLOB...) use all cpp in folder src
file(GLOB SOURCES "src/*.cpp")


# use all the cpp to make exe
add_executable(main ${SOURCES})

# include libs
target_link_libraries(main pthread)
