file(GLOB_RECURSE HASH_LIB_SOURCES
    *.h
    *.cpp
)

add_library(HASH_LIB STATIC ${HASH_LIB_SOURCES})