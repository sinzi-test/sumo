find_program(FOX_CONFIG fox-config)
IF(FOX_CONFIG)
    exec_program(
        ${FOX_CONFIG}
        ARGS --cflags
        OUTPUT_VARIABLE FOX_CXX_FLAGS)
    exec_program(
        ${FOX_CONFIG}
        ARGS --libs
        OUTPUT_VARIABLE FOX_LIBRARY)
endif(FOX_CONFIG)


set(FOX_FOUND 0)
if(FOX_LIBRARY AND FOX_CXX_FLAGS)
    SET(FOX_FOUND TRUE)
else()
    FIND_PATH(FOX_INCLUDE_DIR fox.h)
    FIND_LIBRARY(FOX_LIBRARY NAMES FOXDLL-1.6)
    IF (FOX_INCLUDE_DIR AND FOX_LIBRARY)
        SET(FOX_FOUND TRUE)
    ENDIF (FOX_INCLUDE_DIR AND FOX_LIBRARY)
endif()

if(FOX_FIND_REQUIRED AND NOT FOX_FOUND)
	message(FATAL_ERROR "no fox :-(")
endif()

