CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

add_library(pch ${CMAKE_SOURCE_DIR}/pch/precompile.cpp)
if (MSVC)
   set_target_properties(pch PROPERTIES COMPILE_FLAGS "/Yuprecompile.h")
   set_source_files_properties(precompile.cpp PROPERTIES COMPILE_FLAGS "/Ycprecompile.h")
endif(MSVC)