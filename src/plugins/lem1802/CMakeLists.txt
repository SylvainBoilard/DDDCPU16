FIND_PACKAGE(OpenGL QUIET)

IF(OPENGL_FOUND)
  FIND_PACKAGE(CSFML 2.0 QUIET COMPONENTS system window)

  IF(CSFML_FOUND)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${CSFML_INCLUDE_DIR})

    FILE(GLOB_RECURSE source_files
      defaults.c
      lem1802.c
      )
    ADD_LIBRARY(lem1802 MODULE ${source_files})
    TARGET_LINK_LIBRARIES(lem1802 ${OPENGL_gl_LIBRARY} ${CSFML_LIBRARIES})
  ELSE()
    MESSAGE("CSFML 2.0 or superior (modules system and window) not found, graphical lem1802 will not be built.")
  ENDIF()
ELSE()
  MESSAGE("OpenGL not found, graphical lem1802 will not be built.")
ENDIF()
