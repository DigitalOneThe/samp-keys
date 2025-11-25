cmake_minimum_required(VERSION 3.10)
project(samp-keys VERSION 1.0.0 LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# MinHook
FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG        master
)

# Memwrapper
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

# Ищем только ВАШИ исходные файлы, исключая папки зависимостей
set(MY_SOURCE_DIRS 
    "."
    "Sync" 
    "Sync/rakclient"
)

# Собираем список исходных файлов вручную для каждого каталога
set(ALL_SOURCES "")

foreach(DIR ${MY_SOURCE_DIRS})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR})
        file(GLOB DIR_SOURCES 
            "${DIR}/*.cpp"
            "${DIR}/*.c"
            "${DIR}/*.cxx"
            "${DIR}/*.cc"
        )
        list(APPEND ALL_SOURCES ${DIR_SOURCES})
    endif()
endforeach()

# Альтернативный вариант - явное перечисление файлов
# set(ALL_SOURCES
#     samp-keys/BitStream.cpp
#     samp-keys/CHooks.cpp
#     samp-keys/CKeyboard.cpp
#     samp-keys/CMessages.cpp
#     samp-keys/dllmain.cpp
#     samp-keys/pch.cpp
#     # Добавьте другие ваши файлы...
# )

if(NOT ALL_SOURCES)
    message(FATAL_ERROR "No source files found in project directories!")
endif()

message(STATUS "Found project source files:")
foreach(SRC ${ALL_SOURCES})
    message(STATUS "  ${SRC}")
endforeach()

add_library(samp-keys SHARED ${ALL_SOURCES})

# Линкуем библиотеки
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

# Включаем директории
target_include_directories(samp-keys PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/samp-keys  # Если ваши файлы в папке samp-keys
    ${CMAKE_CURRENT_SOURCE_DIR}/Sync
    ${CMAKE_CURRENT_SOURCE_DIR}/Sync/rakclient
    ${minhook_SOURCE_DIR}/include
    ${memwrapper_SOURCE_DIR}/include
)

set_target_properties(samp-keys PROPERTIES
    OUTPUT_NAME "samp-keys"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
