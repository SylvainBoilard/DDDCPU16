CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(DDDCPU16)
ENABLE_LANGUAGE(C)

SET(CMAKE_C_FLAGS "-ansi -pedantic -Wall -Wextra -Wmain -Wswitch-default -Wmissing-include-dirs -Wmissing-declarations -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls -Winit-self -Wshadow -Wno-unused-parameter -pthread -D_POSIX_C_SOURCE=200809L")
SET(CMAKE_C_FLAGS_DEBUG "-g")
SET(CMAKE_C_FLAGS_RELEASE "-s -O2")

SET(CMAKE_SHARED_MODULE_PREFIX "")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/emulator/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/plugins/)
