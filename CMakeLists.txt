Project(cimgui_sdl)
cmake_minimum_required(VERSION 2.8)
if(WIN32) #to mingw work as all the others
set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif(WIN32)
#general settings


if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cimgui/imgui/backends)
	set(BAKENDS_FOLDER "./thirdparty/cimgui/imgui/backends/")
else()
	set(BAKENDS_FOLDER "./thirdparty/cimgui/imgui/examples/")
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cimgui/imgui/imgui_tables.cpp)
	set(TABLES_SOURCE "./thirdparty/cimgui/imgui/imgui_tables.cpp")
else()
	set(TABLES_SOURCE "")
endif()

include_directories(./thirdparty/cimgui/imgui)
add_definitions("-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")

include_directories(./thirdparty/cimgui/)
set(IMGUI_SOURCES ./thirdparty/cimgui/cimgui.cpp 
./thirdparty/cimgui/imgui/imgui.cpp 
./thirdparty/cimgui/imgui/imgui_draw.cpp 
./thirdparty/cimgui/imgui/imgui_demo.cpp 
./thirdparty/cimgui/imgui/imgui_widgets.cpp
${TABLES_SOURCE} 
)

set(IMGUI_SOURCES_sdl)
set(IMGUI_LIBRARIES )

if (WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
else(WIN32)
    add_definitions("-DIMGUI_IMPL_API=extern \"C\" ")
endif(WIN32)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")

#optional adding freetype
option(IMGUI_FREETYPE "add Freetype2" OFF)

if(IMGUI_FREETYPE)
	FIND_PACKAGE(freetype REQUIRED PATHS ${FREETYPE_PATH})
	list(APPEND IMGUI_LIBRARIES freetype)
	list(APPEND IMGUI_SOURCES ../imgui/misc/freetype/imgui_freetype.cpp)
	add_definitions("-DCIMGUI_FREETYPE=1")
endif(IMGUI_FREETYPE)

#opengl3
list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}imgui_impl_opengl3.cpp)
list(APPEND IMGUI_SOURCES ./cimgui_extras.cpp)
include_directories(../imgui/examples/libs/gl3w)
if(WIN32)
       list(APPEND IMGUI_LIBRARIES opengl32 glu32 glew32)
else(WIN32)#Unix
       list(APPEND IMGUI_LIBRARIES GL)
endif(WIN32)

#sdl2
list(APPEND IMGUI_SOURCES ${BAKENDS_FOLDER}imgui_impl_sdl.cpp)


set(SDL_PATH /c/msys64/mingw64/include/SDL2/)

if(DEFINED SDL_PATH)
	message(STATUS "SDL_PATH defined as " ${SDL_PATH})
	FIND_PACKAGE(SDL2 REQUIRED PATHS ${SDL_PATH})
	get_target_property(SDL_INCLUDE SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
	message(STATUS "sdlinclude is " ${SDL_INCLUDE})
	if ("${SDL_INCLUDE}" STREQUAL "" OR "${SDL_INCLUDE}" STREQUAL "SDL_INCLUDE-NOTFOUND") #if not found latest SDL2 cmake config use older
		message(STATUS "sdlinclude2 is " ${SDL2_INCLUDE_DIRS})
		include_directories(${SDL2_INCLUDE_DIRS})
		set(IMGUI_SDL_LIBRARY ${SDL2_LIBRARIES})
		message(STATUS IMGUI_SDL_LIBRARY ${SDL2_LIBRARIES})
	else()#use new one SDL2 config
		include_directories(${SDL_INCLUDE})
		set(IMGUI_SDL_LIBRARY SDL2::SDL2)
		set(SDL_MAIN SDL2::SDL2main)
		message(STATUS ${SDL_MAIN} ${IMGUI_SDL_LIBRARY})
	endif()
else(DEFINED SDL_PATH)
	message(STATUS "SDL_PATH not defined")
	set(IMGUI_SDL_LIBRARY SDL2)
endif(DEFINED SDL_PATH)

add_library(cimgui_sdl SHARED ${IMGUI_SOURCES})
target_link_libraries(cimgui_sdl ${IMGUI_LIBRARIES} ${IMGUI_SDL_LIBRARY})

#ASSIMP
find_package(assimp REQUIRED)
if (assimp_FOUND)
    set(ASSIMP_LIBRARY "assimp")
    add_library(${ASSIMP_LIBRARY} SHARED IMPORTED)
    SET( ASSIMP_BUILD_ASSIMP_TOOLS ON )
    include_directories("/c/msys64/cmingw64/include/assimp")
    #set_target_properties(${ASSIMP_LIBRARY} PROPERTIES IMPORTED_LOCATION "${ASSIMP_LIBRARY_DIRS}/libassimp.dll")
endif(assimp_FOUND)

#glew
#find_package(GLEW REQUIRED)
#include_directories(${GLEW_INCLUDE_DIRS})
#link_libraries(${GLEW_LIBRARIES})

#using library
include_directories(thirdparty/cimgui/generator/output/)
include_directories(vector)
include_directories(systems)

set(vector_src
  vector/vector.c
  vector/vector.h
)
add_library(vector ${vector_src})

set(systems_src
  systems/window.c
  systems/window.h
  systems/render_device.c
  systems/render_device.h
  systems/key_event.c
  systems/key_event.h
  systems/scene.h
  systems/scene.c
)

set(utils_src
  utils/mesh_loader.c
  utils/mesh_loader.h
  utils/renderer.h
  utils/renderer.c
)

set(components_src
  components/model.c
  components/model.h
  components/world.c
  components/world.h
  components/camera.h
  components/camera.c
)


add_library(systems ${systems_src})
add_library(utils ${utils_src})
add_library(components ${components_src})


add_executable(test_sdl main.c)


target_link_libraries(test_sdl utils)
target_link_libraries(test_sdl systems)
target_link_libraries(test_sdl vector)
target_link_libraries(test_sdl components)



# Target for header-only usage
add_library(${PROJECT_NAME}_headers INTERFACE)
target_include_directories(${PROJECT_NAME}_headers INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}thirdparty/cglm/include/cglm)


if (MINGW)
target_link_options(test_sdl PRIVATE "-mconsole")
endif()
target_link_libraries(test_sdl ${IMGUI_SDL_LIBRARY} cimgui_sdl)
target_link_libraries(test_sdl ${ASSIMP_LIBRARIES})
