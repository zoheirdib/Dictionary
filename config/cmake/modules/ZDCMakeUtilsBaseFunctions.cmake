
# ------------------------------------------------------------------------------
# Grouping
# ------------------------------------------------------------------------------

function( ZD_Source_Group_Custom __remove_until __prefix ) # + input list
    foreach( source ${ARGN} )
        string( REGEX REPLACE "^.*${__remove_until}/?" "" group_name ${source} )    # remove until
        string( REGEX REPLACE "/?[^/]+$" "" group_name ${group_name} )              # remove filename
        string( REPLACE / \\ group_name "${__prefix}/${group_name}" )
        source_group( "${group_name}" FILES ${source} )
    endforeach()
endfunction()


#-----------------------------------------------------------------------------
# To sort files by folders inside project 
# http://blog.audio-tk.com/2015/09/01/sorting-source-files-and-projects-in-folders-with-cmake-and-visual-studioxcode/
#-----------------------------------------------------------------------------
function(ZD_Source_Group_By_Folder target)
  SET(SOURCE_GROUP_DELIMITER "/")
  SET(last_dir "")
  SET(files "")
  FOREACH(file ${${target}_SRC} ${${target}_HEADERS})
    file(RELATIVE_PATH relative_file "${ZD_ROOT_SOURCE_DIR}/${target}/" ${ZD_ROOT_SOURCE_DIR}/${target}/${file})
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
      IF (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      ENDIF (files)
      SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
  ENDFOREACH(file)
  IF (files)
    SOURCE_GROUP("${last_dir}" FILES ${files})
  ENDIF (files)
endfunction()



