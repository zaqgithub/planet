cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_CXX_STANDARD 11) 
#the module for import the thirdparty library
include(FetchContent)
project(Planet VERSION 1.0 LANGUAGES CXX)

# the diretory for the header file
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

#get the thirdparty library for the project

#GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
)
FetchContent_MakeAvailable(glm)

#STB
FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/stb
)
FetchContent_Populate(stb)

#GLFW
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw
)
FetchContent_MakeAvailable(glfw)


#ASSIMP
FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v5.4.3
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp
)
FetchContent_MakeAvailable(assimp)

##GLAD
FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/zaqgithub/glad4opengl.git
	GIT_TAG v1.0.0
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad
)
FetchContent_Populate(glad)
add_library(glad STATIC ${glad_SOURCE_DIR}/src/glad.c)

target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c PROPERTIES LANGUAGE C)

#define the path of the header file
file(GLOB headerfile "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,Clang,GNU,LCC>")
set(msvx_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# integrate the config fot the compiler
add_library(compile_flag INTERFACE)
target_compile_features(compile_flag INTERFACE cxx_std_17)


add_executable(planet ${CMAKE_SOURCE_DIR}/src/asterriod.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/pch)

message(${glad_SOURCE_DIR}/include)


target_include_directories(
	planet 
PRIVATE 
	${headerfile}
	${glm_SOURCE_DIR}/include
	${stb_SOURCE_DIR}
	${glfw_SOURCE_DIR}/include
	${assimp_SOURCE_DIR}/include
)

target_link_libraries(
	planet 
PRIVATE 
	pch
	dependencies
	compile_flag
	glm
	glfw
	assimp
	glad
)

