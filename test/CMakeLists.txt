# Get all subdirectories in the current directory
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(child ${children})
    # Check if it's a directory and contains a CMakeLists.txt
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt)
            add_subdirectory(${child})
        endif()
    endif()
endforeach()