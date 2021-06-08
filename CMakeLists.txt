cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)

#-----------------------------------------------------------------------------
#Set project name
project(Dictionary)

#-----------------------------------------------------------------------------
# Set CMake support directory
set(ZD_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/config/cmake)

#-----------------------------------------------------------------------------
# Add cmake utils and documentation utils
include( ${ZD_CMAKE_DIR}/ZDCMakeUtils.cmake ) 

add_subdirectory(Projects)



