cmake_minimum_required(VERSION 3.12)
project(tsg2)

# Enable grouping targets into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Option to enable/disable SDL
option(SDL_GAME "Enable SDL3 support" OFF)
option(GLFW_GAME "Enable GLFW support" ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build the engine first
add_subdirectory(engine)

# Build the tests
add_subdirectory(test)

# Build the externals
add_subdirectory(externals)

# ============================================================================
# Installation Configuration
# ============================================================================

include(GNUInstallDirs)

# Set default install prefix if not specified
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix" FORCE)
endif()

# Install the engine library (for SDK/development use)
install(TARGETS engine
    ARCHIVE DESTINATION sdk/${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION sdk/${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION sdk/${CMAKE_INSTALL_BINDIR}
    COMPONENT sdk
)

# Install engine headers (for SDK/development use)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/engine/
    DESTINATION sdk/${CMAKE_INSTALL_INCLUDEDIR}/tsg2
    COMPONENT sdk
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

# ============================================================================
# CPack Configuration (for creating installers/archives)
# ============================================================================

set(CPACK_PACKAGE_NAME "tsg2")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "TSG2 Game Engine")
set(CPACK_PACKAGE_VENDOR "Giacomo Matzeu")
set(CPACK_PACKAGE_CONTACT "giacominomatzeu@gmail.com")
set(CPACK_CONFIGURATION_TYPES "Debug;Release")

# Create ZIP archive on Windows and NSIS installer, TGZ on Linux
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

# Component-based packaging configuration
set(CPACK_COMPONENTS_ALL arrow_and_bubbles shader_test physics_test sdk)
set(CPACK_COMPONENT_ARROW_AND_BUBBLES_DISPLAY_NAME "Arrow and Bubbles Demo")
set(CPACK_COMPONENT_SHADER_TEST_DISPLAY_NAME "Shader Test")
set(CPACK_COMPONENT_PHYSICS_TEST_DISPLAY_NAME "Physics Test")
set(CPACK_COMPONENT_SDK_DISPLAY_NAME "Engine SDK (Development)")

# Include runtime dependencies (Windows DLLs)
include(InstallRequiredSystemLibraries)

include(CPack)
