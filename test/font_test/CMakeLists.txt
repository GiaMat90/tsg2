# executable GLFW - specific 
if(GLFW_GAME)
	set(PROJECT_NAME text_test)
	file(GLOB GLSL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
	source_group("source\\shaders" FILES ${GLSL_FILES})
	file(GLOB_RECURSE TEXT_TEST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${GLSL_FILES})

	add_executable(${PROJECT_NAME} ${TEXT_TEST})

	# Set the folder for Visual Studio
	set_target_properties(text_test PROPERTIES FOLDER "test")

	# Include directories
	include_directories(${CMAKE_SOURCE_DIR}/externals/)
	include_directories(${CMAKE_SOURCE_DIR}/externals/tsg/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/deps)	

	# Set up source groups for Visual Studio filters
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source" FILES ${TEXT_TEST})

	# Linking engine libs
	target_link_libraries(${PROJECT_NAME} PRIVATE engine)
	target_link_libraries(${PROJECT_NAME} PRIVATE tsg)
	# Linking GLFW lib needed by TSG2
	target_link_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/externals/GLFW/lib/")
	target_link_libraries(${PROJECT_NAME} PRIVATE glfw3.lib)
	# Linking FreeType lib needed by TSG2
	# Determine the architecture-specific library path
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		# 64-bit build
		set(FREETYPE_LIB_DIR "${CMAKE_SOURCE_DIR}/externals/freetype/lib/Win64")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		# 32-bit build
		set(FREETYPE_LIB_DIR "${CMAKE_SOURCE_DIR}/externals/freetype/lib/Win32")
	else()
		message(FATAL_ERROR "Unsupported architecture")
	endif()
	target_link_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_LIB_DIR})
	target_link_libraries(${PROJECT_NAME} PRIVATE freetype.lib)

	# Settings compiler options
	target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
	if(MSVC)
		target_compile_options(${PROJECT_NAME} PRIVATE /W4)
	else()
		target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
	endif()	
	
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/engine/glfw_port/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
		COMMENT "Copying shaders files to build directory"
	)
	add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/engine/glfw_port/fonts
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts
		COMMENT "Copying shaders files to build directory"
	)

	# Install targets - standalone folder for this executable
	set(TEXT_TEST_INSTALL_DIR ${PROJECT_NAME})
	install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION ${TEXT_TEST_INSTALL_DIR}
		COMPONENT ${PROJECT_NAME}
	)
	# Install engine shaders
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/engine/glfw_port/shaders/
		DESTINATION ${TEXT_TEST_INSTALL_DIR}/shaders
		COMPONENT ${PROJECT_NAME}
		FILES_MATCHING PATTERN "*.glsl"
	)
	# Install test-specific shaders (overwrite/add to engine shaders)
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/
		DESTINATION ${TEXT_TEST_INSTALL_DIR}/shaders
		COMPONENT ${PROJECT_NAME}
		FILES_MATCHING PATTERN "*.glsl"
	)
endif()


