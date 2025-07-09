cmake_minimum_required(VERSION 3.24)
project(ubox)

include(DeclareTarget)

file(GLOB sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

DeclareTarget(
        HIDE
        TARGET_NAME ${PROJECT_NAME}
        TARGET_TYPE ar
	RESOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/avl.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/avl-cmp.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/blob.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/blobmsg.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/uloop.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/usock.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/ustream.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/ustream-fd.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/vlist.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/utils.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/safe_list.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/runqueue.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/md5.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/kvlist.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/ulog.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/base64.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/udebug.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/udebug-remote.c
)

DeclareTarget(
        HIDE
        TARGET_NAME blobmsg_json
        TARGET_TYPE ar
        RESOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/blobmsg_json.c
)
