cmake_minimum_required(VERSION 3.10)
project(samp-keys VERSION 1.0.0 LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Ищем все исходные файлы рекурсивно
file(GLOB_RECURSE SOURCES "*.cpp" "*.c")
file(GLOB_RECURSE HEADERS "*.h" "*.hpp")

# Фильтруем только исходные файлы
set(ALL_SOURCES ${SOURCES})

if(NOT ALL_SOURCES)
    message(FATAL_ERROR "No source files found!")
endif()

message(STATUS "Found source files: ${ALL_SOURCES}")

add_library(samp-keys SHARED ${ALL_SOURCES})

if(WIN32)
    set_target_properties(samp-keys PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    target_compile_definitions(samp-keys PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        BUILD_DLL
    )
endif()

# Автоматически включаем все директории где есть исходные файлы
get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
if(PARENT_DIR STREQUAL "samp-keys")
    # Если мы в подпапке samp-keys, включаем корень
    target_include_directories(samp-keys PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
else()
    # Обычный случай
    target_include_directories(samp-keys PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set_target_properties(samp-keys PROPERTIES
    OUTPUT_NAME "samp-keys"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
