# Find engine3
#
#  IDLC_BIN_DIR - where to find idlc
#  ENGINE3_INCLUDE_DIR - where to find engine headers, etc.
#  ENGINE3_LIBRARIES   - list of libraries when using engine3
#  ENGINE3_FOUND       - true if engine3 found.

IF (ENGINE3_INCLUDE_DIR)
  # Already in cache, be silent
  SET(ENGINE3_FIND_QUIETLY TRUE)
ENDIF (ENGINE3_INCLUDE_DIR)

FIND_PATH(IDLC_BIN_DIR idlc
  /opt/engine3/bin
  /opt/engine3pub/bin
  ../MMOEngine/bin
  NO_DEFAULT_PATH
)

FIND_PATH(ENGINE3_INCLUDE_DIR engine/engine.h
  /opt/engine3/include
  /opt/engine3pub/include
  ../MMOEngine/include
  ../MMOEngine/src
)

SET(ENGINE3_NAMES engine3)
FIND_LIBRARY(ENGINE3_LIBRARY
  NAMES ${ENGINE3_NAMES}
  PATHS /opt/engine3/lib /opt/engine3pub/lib ../MMOEngine/lib/unix
)

IF (IDLC_BIN_DIR AND ENGINE3_INCLUDE_DIR AND ENGINE3_LIBRARY)
  SET(ENGINE3_FOUND TRUE)
  SET(ENGINE3_LIBRARIES ${ENGINE3_LIBRARY})
  SET(IDL_DIRECTIVES -od autogen -cp ${ENGINE3_INCLUDE_DIR} -sd ${CMAKE_SOURCE_DIR}/src anyadEclipse)
ELSE ()
  SET(ENGINE3_FOUND FALSE)
  SET(ENGINE3_LIBRARIES)
  SET(IDL_DIRECTIVES)
ENDIF ()

IF (ENGINE3_FOUND)
  IF (NOT ENGINE3_FIND_QUIETLY)
    MESSAGE(STATUS "Found idlc ${IDLC_BIN_DIR}")
    MESSAGE(STATUS "Found engine3 includes: ${ENGINE3_INCLUDE_DIR}")
    MESSAGE(STATUS "Found engine3 library: ${ENGINE3_LIBRARY}")
  ENDIF (NOT ENGINE3_FIND_QUIETLY)
ELSE (ENGINE3_FOUND)
  IF (ENGINE3_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for engine3 libraries named ${ENGINE3_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find engine3 library")
  ENDIF (ENGINE3_FIND_REQUIRED)
ENDIF (ENGINE3_FOUND)

MARK_AS_ADVANCED(
  ENGINE3_LIBRARY
  ENGINE3_INCLUDE_DIR
)
