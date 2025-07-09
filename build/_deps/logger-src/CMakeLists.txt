cmake_minimum_required(VERSION 3.24)
project(logger)

include(DeclareTarget)
include(FindTedixLibrary)

file(GLOB sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

DeclareTarget(
        HIDE
        TARGET_NAME ${PROJECT_NAME}
        TARGET_TYPE ar
        RESOURCES ${sources}
)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    FindTedixLibrary(fmt stable 9.1.0-4-g2bfa190)
    FindTedixLibrary(etl stable 20.35.14-4-gc44843a)
    FindTedixLibrary(ctre stable 3.7.2-4-g0ff26e7)
    FindTedixLibrary(utils stable 1.0.1-1-gc2d5527)
    FindTedixLibrary(errcode stable 1.1.0-10-gc84861d)
    FindTedixLibrary(LIB law testing 1.4.1-20-g0f1b3db)
endif ()
