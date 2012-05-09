CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(DDDCPU16)
ENABLE_LANGUAGE(C)

SET(CMAKE_C_FLAGS "-ansi -pedantic -Wall -Wextra -Wmain -Wswitch-default -Wmissing-include-dirs -Wmissing-declarations -Wfloat-equal -Wundef -Wcast-align -Wredundant-decls -Winit-self -Wshadow -Wno-unused-parameter")
SET(CMAKE_C_FLAGS_DEBUG "-g")
SET(CMAKE_C_FLAGS_RELEASE "-s -O2")

IF(NOT DEFINED DDD_BUILD_EMULATOR)
  SET(DDD_BUILD_EMULATOR TRUE)
ENDIF(NOT DEFINED DDD_BUILD_EMULATOR)

IF(DDD_BUILD_EMULATOR)
  SUBDIRS(${CMAKE_SOURCE_DIR}/emulator/)
ENDIF(DDD_BUILD_EMULATOR)
