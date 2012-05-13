SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)
FILE(GLOB_RECURSE source_files
  adv.c
  globals.c
  init.c
  main.c
  nb_instr.c
  opcodes.c
  values.c
  )
ADD_EXECUTABLE(dddcpu16 ${source_files})
