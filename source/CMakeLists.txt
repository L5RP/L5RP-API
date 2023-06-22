# Headers
include_directories(${CMAKE_SOURCE_DIR}/deps/RageMP)
include_directories(${CMAKE_SOURCE_DIR}/deps/JSON)
include_directories(${CMAKE_SOURCE_DIR}/deps/SimpleINI)
include_directories(${CMAKE_SOURCE_DIR}/deps/hashlib2plus)
include_directories(${CMAKE_SOURCE_DIR}/deps/spdlog)
include_directories(${CMAKE_SOURCE_DIR}/deps/xxHash)
include_directories(${CMAKE_SOURCE_DIR}/deps/curl/inc)

# L5RP.dll
file(GLOB L5RP_API_SOURCES
     *.cpp
     )

if (MSVC)

    list(REMOVE_ITEM L5RP_API_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/api.cpp)

    ADD_MSVC_PRECOMPILED_HEADER(api.h api.cpp L5RP_API_SOURCES)

endif (MSVC)

add_library(L5RP_API STATIC ${L5RP_API_SOURCES})

if (WIN32)

    target_link_libraries(L5RP_API crypt32.lib ws2_32.lib dnsapi.lib ${CMAKE_SOURCE_DIR}/deps/curl/lib_win/libcurl_imp.lib HASH_LIB)

endif (WIN32)

if (UNIX)

    target_link_libraries(L5RP_API /usr/lib/gcc/x86_64-linux-gnu/7/libstdc++.a /usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a ${CMAKE_SOURCE_DIR}/deps/curl/lib_linux/libcurl.so HASH_LIB)

endif (UNIX)