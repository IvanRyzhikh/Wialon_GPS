cmake_minimum_required(VERSION 3.24)
project(ubus)

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
	set(UBUS_UNIX_SOCKET "/var/run/ubus/ubus.sock")
	set(UBUS_MAX_MSGLEN 1048576)

	CompileDefinitions(
		UBUS_UNIX_SOCKET="${UBUS_UNIX_SOCKET}"
		UBUS_MAX_MSGLEN=${UBUS_MAX_MSGLEN}
	)

	FindTedixLibrary(ubox stable 1.0.0-2-g71300bb)
	FindTedixLibrary(blobmsg_json stable 1.0.0-2-g71300bb)
endif ()

