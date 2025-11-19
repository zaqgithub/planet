set(DEPENDENCY_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderClass.cpp
)

add_library(dependencies STATIC ${DEPENDENCY_SOURCES})

target_include_directories(
	dependencies
	PUBLIC
		${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
	dependencies
	PUBLIC
		compiler_options
		glm::glm
		glfw
		assimp
		glad
		stb::stb
)

target_precompile_headers(dependencies PRIVATE ${CMAKE_SOURCE_DIR}/pch/precompile.h)
