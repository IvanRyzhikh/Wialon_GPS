# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/user/work/mon-service/build/_deps/uci-src"
  "/home/user/work/mon-service/build/_deps/uci-build"
  "/home/user/work/mon-service/build/_deps/uci-subbuild/uci-populate-prefix"
  "/home/user/work/mon-service/build/_deps/uci-subbuild/uci-populate-prefix/tmp"
  "/home/user/work/mon-service/build/_deps/uci-subbuild/uci-populate-prefix/src/uci-populate-stamp"
  "/home/user/work/mon-service/build/_deps/uci-subbuild/uci-populate-prefix/src"
  "/home/user/work/mon-service/build/_deps/uci-subbuild/uci-populate-prefix/src/uci-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/user/work/mon-service/build/_deps/uci-subbuild/uci-populate-prefix/src/uci-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/user/work/mon-service/build/_deps/uci-subbuild/uci-populate-prefix/src/uci-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
