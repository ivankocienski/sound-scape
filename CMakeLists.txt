
#set(CMAKE_VERBOSE_MAKEFILE ON)

# our exe name
project (speech-noise)

cmake_minimum_required (VERSION 2.6)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -g -std=c++11")

find_package(PkgConfig REQUIRED)
pkg_search_module(SNDFILE REQUIRED sndfile)
include_directories(${SNDFILE_INCLUDE_DIRS})

#find_package (Threads REQUIRED)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_SYSTEM ON)
find_package (Boost COMPONENTS thread system REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package (Portaudio REQUIRED)


# source files. sure, there's a better way of doing this
set (SRC
  src/main.cc

	src/audio/exception.cc
	src/audio/service.cc
	src/audio/sample.cc
)


link_libraries(
  ${PTHREAD_LIBRARY}
  ${Boost_LIBRARIES}
  ${PORTAUDIO_LIBRARIES}
  ${SNDFILE_LIBRARIES}
)

add_executable(speech-noise ${SRC} )

add_custom_command(TARGET speech-noise POST_BUILD 
  COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/data COMMENT "removing stale data directory"
  COMMAND ln -sf ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data COMMENT "linking data directory"
)
