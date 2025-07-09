cmake_minimum_required(VERSION 3.24)
project(mon)

include(LinkLibraries)
include(DeclareTarget)
include(FindTedixLibrary)
include(CompileDefinitions)

add_link_options(-static)

file(GLOB sources CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/app/*.cpp"
        "${CMAKE_SOURCE_DIR}/app/*/*.cpp"

        "${CMAKE_SOURCE_DIR}/app/*.h"
        "${CMAKE_SOURCE_DIR}/app/*/*.h"
        "${CMAKE_SOURCE_DIR}/app/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/app/*/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/app/*/*/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/app/*/*/*/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/app/*/*/*/*/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/app/*/*/*/*/*/*/*/*.h"
)

file(GLOB test_sources CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/test/*.h"
        "${CMAKE_SOURCE_DIR}/test/*/*.h"
        "${CMAKE_SOURCE_DIR}/test/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/test/*/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/test/*/*/*/*/*.h"
        "${CMAKE_SOURCE_DIR}/test/*/*/*/*/*/*.h"

        "${CMAKE_SOURCE_DIR}/test/*.cpp"
        "${CMAKE_SOURCE_DIR}/test/*/*.cpp"
        "${CMAKE_SOURCE_DIR}/test/*/*/*.cpp"
        "${CMAKE_SOURCE_DIR}/test/*/*/*/*.cpp"
        "${CMAKE_SOURCE_DIR}/test/*/*/*/*/*.cpp"
        "${CMAKE_SOURCE_DIR}/test/*/*/*/*/*/*.cpp"
        "${CMAKE_SOURCE_DIR}/test/*/*/*/*/*/*/*.cpp"
)

DeclareTarget(
	#        LIBCXX
        # SANITIZE
        TARGET_NAME ${PROJECT_NAME}
        TARGET_TYPE bin
        RESOURCES ${sources} "${CMAKE_SOURCE_DIR}/app/main.cpp"
)

#DeclareTarget(
#        LIBCXX
#        TARGET_NAME ${PROJECT_NAME}-test
#        TARGET_TYPE bin
#        RESOURCES ${sources} ${test_sources} "${CMAKE_SOURCE_DIR}/test/TestInit.cpp"
#)

CompileDefinitions(
        FMT_HEADER_ONLY
)

FindTedixLibrary(ctre stable 3.7.2-4-g0ff26e7)

FindTedixLibrary(fmt stable 9.1.0-4-g2bfa190)
#FindTedixLibrary(LIB rapidjson stable 1.1.0)

FindTedixLibrary(etl stable 20.35.14-4-gc44843a)
FindTedixLibrary(EXPOSE_SRC LIB utils stable 1.0.1-1-gc2d5527)
FindTedixLibrary(LIB static_string stable 1.0.0-3-g1c8f8c0)

FindTedixLibrary(EXPOSE_SRC LIB errcode stable 1.1.0-10-gc84861d)
#FindTedixLibrary(LIB more_concepts stable v0.1.0-2-g6c9b40d)
FindTedixLibrary(LIB magic_enum stable v0.9.5-2-ge627f78)

#FindTedixLibrary(EXPOSE_SRC LIB confparse stable 0.4.0-4-g8a8f175)
#FindTedixLibrary(EXPOSE_SRC LIB mmap-cb testing 0.0.1-11-g289205b)

#FindTedixLibrary(EXPOSE_SRC LIB dns stable 0.1-14-g176ac3d)
#FindTedixLibrary(EXPOSE_SRC LIB wic unstable 0.2.1-8-gc4c54b8)
FindTedixLibrary(EXPOSE_SRC LIB law unstable 1.5.0-5-g6ee6d85)

FindTedixLibrary(EXPOSE_SRC LIB logger testing 1.1.0-11-ge55f4c2)

FindTedixLibrary(LIB blobmsg_json stable 1.0.0-2-g71300bb)
FindTedixLibrary(LIB ubox stable 1.0.0-2-g71300bb)
#FindTedixLibrary(LIB ubus stable 1.0.0-1-gbae57fc)
FindTedixLibrary(LIB ubus stable 1.0.0-4-g7097d1e)

FindTedixLibrary(LIB uci stable 1.0.0-3-gac50adc)

LinkLibraries(LIBS rt)

#LinkLibraries(
#        TARGET_TYPES test
#        LIBS gtest gmock gtest_main gmock_main
#)

