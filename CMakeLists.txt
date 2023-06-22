cmake_minimum_required(VERSION 3.1)

# MACROS_START
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
    IF(MSVC)
        SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/headers.pch")
        SET(Sources ${${SourcesVar}})

        SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                    PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                    OBJECT_OUTPUTS "${PrecompiledBinary}")
        SET_SOURCE_FILES_PROPERTIES(${Sources}
                                    PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                    OBJECT_DEPENDS "${PrecompiledBinary}")
        # Add precompiled header to SourcesVar
        LIST(APPEND ${SourcesVar} ${PrecompiledSource})
    ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)
# MACROS_END

#creates soliution afaik.
project(L5RP CXX)

#Some C++ stuff

#-fPIC flag
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#no vs c api warnings
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

#Visual Studio C++17
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

add_subdirectory(deps)
add_subdirectory(source)
