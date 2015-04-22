# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (lab_project)

# CMakeLists modified from: http://www.opengl-tutorial.org/download/

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

#Source: http://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  #-Wno-comment Will disable a commom warning when you are using GLM lib
  #-Wno-variadic-macros Is to permit the use of ... on the debug macros
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Wno-long-long -Wno-comment -pedantic -Wno-variadic-macros")
endif()



include_directories(
	external/glfw-3.0.3/include/
	external/glm/
	external/glew-1.9.0/include/
	project/engine/headers/
	project/flood-fill/headers/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_303
	GLEW_190
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(
        main

        ### Flood Fill Files ###
        #Source Files
        project/flood-fill/source/main.cpp
        #project/flood-fill/source/main_level.cpp
        project/flood-fill/source/global_variables.cpp
        project/flood-fill/source/player.cpp
        project/flood-fill/source/ground_plane.cpp
        project/flood-fill/source/debug_player.cpp
        project/flood-fill/source/demo_level.cpp
        project/flood-fill/source/box.cpp
        project/flood-fill/source/level_design.cpp
        project/flood-fill/source/border.cpp


        #Header Files
        #project/flood-fill/headers/main_level.hpp
        project/flood-fill/headers/global_variables.hpp
        project/flood-fill/headers/player.hpp
        project/flood-fill/headers/ground_plane.hpp
        project/flood-fill/headers/debug_player.hpp
        project/flood-fill/headers/demo_level.hpp
        project/flood-fill/headers/box.hpp
        project/flood-fill/headers/level_design.hpp
        project/flood-fill/headers/border.hpp

        ### Engine Files ###
        #Source Files
        project/engine/source/tiny_obj_loader.cc
        project/engine/source/mesh.cpp
        project/engine/source/shader.cpp
        project/engine/source/object.cpp
        project/engine/source/camera.cpp
        project/engine/source/scene.cpp
        project/engine/source/load_manager.cpp
        project/engine/source/director.cpp
        project/engine/source/game_object.cpp
        project/engine/source/time_manager.cpp
        project/engine/source/bounding_box.cpp
        project/engine/source/collision_manager.cpp
        project/engine/source/collision_object.cpp
        project/engine/source/light.cpp
        project/engine/source/material.cpp
        project/engine/source/render_engine.cpp
        project/engine/source/material_manager.cpp
        project/engine/source/image.cpp
        project/engine/source/texture.cpp

        #Header Files
        project/engine/headers/GLIncludes.h
        project/engine/headers/tiny_obj_loader.h
        project/engine/headers/debug_macros.h
        project/engine/headers/mesh.hpp
        project/engine/headers/shader.hpp
        project/engine/headers/object.hpp
        project/engine/headers/camera.hpp
        project/engine/headers/scene.hpp
        project/engine/headers/load_manager.hpp
        project/engine/headers/director.hpp
        project/engine/headers/game_object.hpp
        project/engine/headers/time_manager.hpp
        project/engine/headers/bounding_box.hpp
        project/engine/headers/collision_manager.hpp
        project/engine/headers/collision_object.hpp
        project/engine/headers/light.hpp
        project/engine/headers/material.hpp
        project/engine/headers/render_engine.hpp
        project/engine/headers/material_manager.hpp
        project/engine/headers/uniform_3d_grid.hpp
        project/engine/headers/image.hpp
        project/engine/headers/texture.hpp

        #Shader Files
        project/engine/shaders/vertex.glsl
        project/engine/shaders/fragment.glsl

        project/engine/shaders/bounding_box_vertex.glsl
        project/engine/shaders/bounding_box_fragment.glsl
)
target_link_libraries(main
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(main PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/project/")
create_target_launcher(main WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/")
create_default_target_launcher(main WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/")



SOURCE_GROUP(Shaders "project/engine/shaders/.*glsl$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
#Copying new resources to the build folder
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/engine/shaders" "${CMAKE_CURRENT_BINARY_DIR}/engine/shaders"
)
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/objects" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/objects"
)
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/textures" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/textures"
)

#Copying the compiled project to the project folder, out of the build directory
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/main${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/project/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

