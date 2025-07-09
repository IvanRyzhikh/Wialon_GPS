cmake_minimum_required(VERSION 3.24)
project(law)

include(DeclareTarget)
include(LinkLibraries)
include(FindTedixLibrary)

file(GLOB sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*/*/*.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/inc/law/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/law/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/law/*/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/law/*/*/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/law/*/*/*/*/*.h"
)

DeclareTarget(
        HIDE
        TARGET_NAME ${PROJECT_NAME}
        TARGET_TYPE ar
        RESOURCES ${sources}
)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        FindTedixLibrary(etl stable 20.35.14-5-gcc0d7e3)
        FindTedixLibrary(ctre stable 3.7.2-5-ge3e1713)
        FindTedixLibrary(utils stable 1.0.1-2-g6980857)
        FindTedixLibrary(errcode stable 1.2.0-1-g0a5b8a2)

        LinkLibraries(LIBS rt pthread)
endif ()
