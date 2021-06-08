# ZDEngine/Projects/CMakeLists.txt

project( Projects )

option( BUILD_Dictionary "Build Dictionary project" ON )


# HelloWorldAPI
if(BUILD_Dictionary)
  add_subdirectory(Dictionary)
endif()




















