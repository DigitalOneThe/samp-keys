cmake_minimum_required(VERSION 3.10)
project(samp-keys VERSION 1.0.0 LANGUAGES C CXX)

# Настройка для создания shared library (DLL)
set(BUILD_SHARED_LIBS ON)

# Настройка стандартов C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включить все предупреждения
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Исходные файлы из корневой директории
file(GLOB ROOT_SOURCES "samp-keys/*.cpp")
file(GLOB ROOT_HEADERS "samp-keys/*.h")

# Исходные файлы из папки Sync (которая в корне)
file(GLOB SYNC_SOURCES "samp-keys/Sync/*.cpp")
file(GLOB SYNC_HEADERS "samp-keys/Sync/*.hpp")

# Исходные файлы из папки Sync/rakclient
file(GLOB RAKCLIENT_SOURCES "samp-keys/Sync/rakclient/*.cpp")
file(GLOB RAKCLIENT_HEADERS "samp-keys/Sync/rakclient/*.h")

# Объединяем все исходные файлы
set(ALL_SOURCES 
    ${ROOT_SOURCES}
    ${SYNC_SOURCES} 
    ${RAKCLIENT_SOURCES}
)

# Проверяем что есть исходные файлы
if(NOT ALL_SOURCES)
    message(FATAL_ERROR "No source files found! Check your project structure.")
endif()

message(STATUS "Found source files: ${ALL_SOURCES}")

# Создаем shared library
add_library(samp-keys SHARED ${ALL_SOURCES})

# Устанавливаем свойства для Windows DLL
if(WIN32)
    set_target_properties(samp-keys PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()

# Включаем директории для заголовочных файлов
target_include_directories(samp-keys PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Sync
    ${CMAKE_CURRENT_SOURCE_DIR}/Sync/rakclient
)

# Для Windows указываем специфичные настройки
if(WIN32)
    target_compile_definitions(samp-keys PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        BUILD_DLL
    )
endif()

# Установка выходного имени файла
set_target_properties(samp-keys PROPERTIES
    OUTPUT_NAME "samp-keys"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
