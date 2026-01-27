# executable GLFW - specific 
if(GLFW_GAME)
	file(GLOB GLSL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
	source_group("source\\shaders" FILES ${GLSL_FILES})
	file(GLOB_RECURSE SHADER_TEST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${GLSL_FILES})

	add_executable(shader_test ${SHADER_TEST})

	# Set the folder for Visual Studio
	set_target_properties(shader_test PROPERTIES FOLDER "test")

	# Include directories
	include_directories(${CMAKE_SOURCE_DIR}/externals/)
	include_directories(${CMAKE_SOURCE_DIR}/externals/tsg/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/deps)	

	# Set up source groups for Visual Studio filters
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source" FILES ${SHADER_TEST})

	# Linking engine libs
	target_link_libraries(shader_test PRIVATE tsg)
	target_link_libraries(shader_test PRIVATE engine)
	# Linking GLFW libs
	target_link_directories(shader_test PRIVATE "${CMAKE_SOURCE_DIR}/externals/GLFW/lib/")
	target_link_libraries(shader_test PRIVATE glfw3.lib)
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
	target_link_directories(shader_test PRIVATE ${FREETYPE_LIB_DIR})
	target_link_libraries(shader_test PRIVATE freetype.lib)

	add_compile_definitions(GLFW_INCLUDE_NONE)

	# Settings compiler options
	target_compile_features(shader_test PUBLIC cxx_std_23)
	if(MSVC)
		target_compile_options(shader_test PRIVATE /W4)
	else()
		target_compile_options(shader_test PRIVATE -Wall -Wextra)
	endif()	
	
	add_custom_command(TARGET shader_test POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/engine/glfw_port/shaders
        $<TARGET_FILE_DIR:shader_test>/shaders
		COMMENT "Copying shaders files to build directory"
	)

	# Install targets - standalone folder for this executable
	set(SHADER_TEST_INSTALL_DIR shader_test)
	install(TARGETS shader_test
		RUNTIME DESTINATION ${SHADER_TEST_INSTALL_DIR}
		COMPONENT shader_test
	)
	# Install engine shaders
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/engine/glfw_port/shaders/
		DESTINATION ${SHADER_TEST_INSTALL_DIR}/shaders
		COMPONENT shader_test
		FILES_MATCHING PATTERN "*.glsl"
	)
	# Install test-specific shaders (overwrite/add to engine shaders)
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/
		DESTINATION ${SHADER_TEST_INSTALL_DIR}/shaders
		COMPONENT shader_test
		FILES_MATCHING PATTERN "*.glsl"
	)
endif()


