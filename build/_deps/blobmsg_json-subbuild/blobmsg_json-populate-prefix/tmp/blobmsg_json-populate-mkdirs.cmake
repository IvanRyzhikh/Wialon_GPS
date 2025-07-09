# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/user/work/mon-service/build/_deps/blobmsg_json-src"
  "/home/user/work/mon-service/build/_deps/blobmsg_json-build"
  "/home/user/work/mon-service/build/_deps/blobmsg_json-subbuild/blobmsg_json-populate-prefix"
  "/home/user/work/mon-service/build/_deps/blobmsg_json-subbuild/blobmsg_json-populate-prefix/tmp"
  "/home/user/work/mon-service/build/_deps/blobmsg_json-subbuild/blobmsg_json-populate-prefix/src/blobmsg_json-populate-stamp"
  "/home/user/work/mon-service/build/_deps/blobmsg_json-subbuild/blobmsg_json-populate-prefix/src"
  "/home/user/work/mon-service/build/_deps/blobmsg_json-subbuild/blobmsg_json-populate-prefix/src/blobmsg_json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/user/work/mon-service/build/_deps/blobmsg_json-subbuild/blobmsg_json-populate-prefix/src/blobmsg_json-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/user/work/mon-service/build/_deps/blobmsg_json-subbuild/blobmsg_json-populate-prefix/src/blobmsg_json-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
