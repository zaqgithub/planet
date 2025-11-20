cmake_minimum_required(VERSION 3.23)

if(POLICY CMP0169)
	cmake_policy(SET CMP0169 OLD)
endif()

project(Planet VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

include(FetchContent)

option(GLFW_BUILD_EXAMPLES "Disable GLFW examples" OFF)
option(GLFW_BUILD_TESTS "Disable GLFW tests" OFF)
option(GLFW_BUILD_DOCS "Disable GLFW docs" OFF)
option(GLFW_INSTALL "Disable GLFW install targets" OFF)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v5.4.3
)

FetchContent_MakeAvailable(glm glfw assimp)

FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
)

FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
	FetchContent_Populate(stb)
endif()

if(NOT TARGET stb::stb)
	add_library(stb_headers INTERFACE)
	target_include_directories(stb_headers INTERFACE ${stb_SOURCE_DIR})
	add_library(stb::stb ALIAS stb_headers)
endif()

set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad-generated)
add_library(glad STATIC ${GLAD_DIR}/src/gl.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
set_target_properties(glad PROPERTIES FOLDER "thirdparty")

add_library(compiler_options INTERFACE)
target_compile_features(compiler_options INTERFACE cxx_std_17)
if(MSVC)
	target_compile_options(compiler_options INTERFACE /permissive- /Zc:preprocessor /W4 /EHsc)
else()
	target_compile_options(compiler_options INTERFACE -Wall -Wextra -Wpedantic)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(planet ${CMAKE_SOURCE_DIR}/src/asterriod.cpp)

target_link_libraries(
	planet
	PRIVATE
		dependencies
		compiler_options
)

target_precompile_headers(planet PRIVATE ${CMAKE_SOURCE_DIR}/pch/precompile.h)