cmake_minimum_required(VERSION 3.10)
project(samp-keys VERSION 1.0.0 LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем MinHook как подмодуль или скачиваем
include(FetchContent)
FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        origin/master
)
FetchContent_Declare(
    memwrapper
    GIT_REPOSITORY https://github.com/The-Musaigen/memwrapper.git
    GIT_TAG        origin/master
)
FetchContent_MakeAvailable(minhook)
FetchContent_MakeAvailable(memwrapper)

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

# Линкуем MinHook к нашей библиотеке
target_link_libraries(samp-keys PRIVATE minhook)

if(WIN32)
    set_target_properties(samp-keys PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    target_compile_definitions(samp-keys PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        BUILD_DLL
    )
endif()

# Включаем директории MinHook и наши
target_include_directories(samp-keys PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${minhook_SOURCE_DIR}/include  # Директория с MinHook.h
)

set_target_properties(samp-keys PROPERTIES
    OUTPUT_NAME "samp-keys"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
