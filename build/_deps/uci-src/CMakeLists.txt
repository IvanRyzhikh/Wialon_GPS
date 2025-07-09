cmake_minimum_required(VERSION 3.24)
project(uci)

include(DeclareTarget)
include(FindTedixLibrary)
include(CompileDefinitions)

file(GLOB sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

DeclareTarget(
        HIDE
        TARGET_NAME ${PROJECT_NAME}
        TARGET_TYPE ar
        RESOURCES ${sources}
)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	FindTedixLibrary(ubox stable 1.0.0-2-g71300bb)
endif ()
