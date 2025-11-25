cmake_minimum_required(VERSION 3.10)
project(samp-keys VERSION 1.0.0 LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        master
)

FetchContent_Declare(
    memwrapper
    GIT_REPOSITORY https://github.com/The-Musaigen/memwrapper.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(minhook memwrapper)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

file(GLOB_RECURSE SOURCES "*.cpp" "*.c")
file(GLOB_RECURSE HEADERS "*.h" "*.hpp")

set(ALL_SOURCES ${SOURCES})

if(NOT ALL_SOURCES)
    message(FATAL_ERROR "No source files found!")
endif()

message(STATUS "Found source files: ${ALL_SOURCES}")

add_library(samp-keys SHARED ${ALL_SOURCES})

target_link_libraries(samp-keys PRIVATE minhook memwrapper)

if(WIN32)
    set_target_properties(samp-keys PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    target_compile_definitions(samp-keys PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        BUILD_DLL
    )
endif()

target_include_directories(samp-keys PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${minhook_SOURCE_DIR}/include
    ${memwrapper_SOURCE_DIR}/include
)

set_target_properties(samp-keys PROPERTIES
    OUTPUT_NAME "samp-keys"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
