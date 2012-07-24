SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/)
FILE(GLOB_RECURSE source_files
  adv.c
  events.c
  globals.c
  hardware.c
  init.c
  interrupts.c
  main.c
  nb_instr.c
  opcodes.c
  parse.c
  timing.c
  values.c
  )
ADD_EXECUTABLE(dddcpu16 ${source_files})
TARGET_LINK_LIBRARIES(dddcpu16 dl rt)
