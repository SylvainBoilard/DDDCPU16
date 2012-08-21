FILE(GLOB_RECURSE source_files
  clock.c
  )
ADD_LIBRARY(clock MODULE ${source_files})
TARGET_LINK_LIBRARIES(clock)
