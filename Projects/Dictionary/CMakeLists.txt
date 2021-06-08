# ZDEngine/Dictionary/CMakeLists.txt

project(Dictionary)

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  include_directories( ${CMAKE_CURRENT_BINARY_DIR})
  include_directories( ${EXTERNAL_LIBS_INCLUDE_DIR} )
  
  file( GLOB_RECURSE header_list_Dictionary 	"${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" )
  file( GLOB_RECURSE source_list_Dictionary   	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
  
  set( Dictionary_sources
		${header_list_Dictionary}
		${source_list_Dictionary}
	 )

#-----------------------------------------------------------
# source_group
#-----------------------------------------------------------

ZD_Source_Group_Custom( ${CMAKE_CURRENT_SOURCE_DIR} "Header Files" ${header_list_Dictionary} )
ZD_Source_Group_Custom( ${CMAKE_CURRENT_SOURCE_DIR} "Source Files" ${source_list_Dictionary} )
ZD_Source_Group_Custom( ${CMAKE_CURRENT_SOURCE_DIR} "QT-UI Files" ${ui_list_Dictionary} )

ZD_Source_Group_By_Folder (Dictionary)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Lexico/Lexico.txt ${CMAKE_CURRENT_BINARY_DIR}/Lexico.txt COPYONLY)

add_executable(${PROJECT_NAME} ${Dictionary_sources})

set_target_properties (Dictionary PROPERTIES FOLDER Projects)
