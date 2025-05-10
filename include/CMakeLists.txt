file(GLOB cppforheader "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB pch "../pch/precompile.h")

add_library(dependencies ${cppforheader})
message(STATUS "PCH file: ${pch}")
target_precompile_headers(dependencies PRIVATE ${pch})
target_include_directories(dependencies INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
