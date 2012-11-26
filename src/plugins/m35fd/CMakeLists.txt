FILE(GLOB_RECURSE source_files
  m35fd.c
  )
ADD_LIBRARY(m35fd MODULE ${source_files})
TARGET_LINK_LIBRARIES(m35fd)
