# executable GLFW - specific 
if(GLFW_GAME)
	file(GLOB_RECURSE ARROW_AND_BUBBLES_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

	add_executable(arrow_and_bubbles ${ARROW_AND_BUBBLES_SOURCE})

	# Set the folder for Visual Studio
	set_target_properties(arrow_and_bubbles PROPERTIES FOLDER "test")

	# Include directories
	include_directories(${CMAKE_SOURCE_DIR}/externals/tsg/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/include)
	include_directories(${CMAKE_SOURCE_DIR}/externals/)
	include_directories(${CMAKE_SOURCE_DIR}/externals/GLFW/deps)

	# Set up source groups for Visual Studio filters
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source" FILES ${ARROW_AND_BUBBLES_SOURCE})

	# Linking engine libs
	target_link_libraries(arrow_and_bubbles PRIVATE engine)
	target_link_libraries(arrow_and_bubbles PRIVATE tsg)
	# Linking GLFW libs
	target_link_directories(arrow_and_bubbles PRIVATE "${CMAKE_SOURCE_DIR}/externals/GLFW/lib/")
	target_link_libraries(arrow_and_bubbles PRIVATE glfw3.lib)

	# Settings compiler options
	target_compile_features(arrow_and_bubbles PUBLIC cxx_std_23)
	if(MSVC)
		target_compile_options(arrow_and_bubbles PRIVATE /W4)
	else()
		target_compile_options(arrow_and_bubbles PRIVATE -Wall -Wextra)
	endif()

	add_custom_command(TARGET arrow_and_bubbles POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/assets
		$<TARGET_FILE_DIR:arrow_and_bubbles>/assets
		COMMENT "Copying assets files to build directory"
	)
	add_custom_command(TARGET arrow_and_bubbles POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/engine/glfw_port/shaders
        $<TARGET_FILE_DIR:arrow_and_bubbles>/shaders
		COMMENT "Copying shaders files to build directory"
	)

	# Install targets - standalone folder for this executable
	set(ARROW_INSTALL_DIR arrow_and_bubbles)
	install(TARGETS arrow_and_bubbles
		RUNTIME DESTINATION ${ARROW_INSTALL_DIR}
		COMPONENT arrow_and_bubbles
	)
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets/
		DESTINATION ${ARROW_INSTALL_DIR}/assets
		COMPONENT arrow_and_bubbles
	)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/engine/glfw_port/shaders/
		DESTINATION ${ARROW_INSTALL_DIR}/shaders
		COMPONENT arrow_and_bubbles
		FILES_MATCHING PATTERN "*.glsl"
	)
endif()



