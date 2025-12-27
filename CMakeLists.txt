cmake_minimum_required(VERSION 3.10)
project(LinaRuntime C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.10)
project(LinaRuntime C)

# Помогаем CMake найти Python в NixOS, если стандартный поиск подвел
if(DEFINED ENV{PYTHON_INCLUDE_DIR})
    set(Python3_INCLUDE_DIRS $ENV{PYTHON_INCLUDE_DIR})
    set(Python3_LIBRARIES $ENV{PYTHON_LIBRARY})
    set(Python3_Found TRUE)
    message(STATUS "Using NixOS Python paths: ${Python3_INCLUDE_DIRS}")
else()
    find_package(Python3 COMPONENTS Development Embed REQUIRED)
endif()

# 2. Поиск Julia
# В NixOS мы пробросили JULIA_INCLUDE_DIR в shell.nix
if(NOT DEFINED ENV{JULIA_INCLUDE_DIR})
    message(FATAL_ERROR "JULIA_INCLUDE_DIR not found. Please run within nix-shell")
endif()

set(JULIA_INCLUDE_DIR $ENV{JULIA_INCLUDE_DIR})
set(JULIA_LIBRARY_DIR $ENV{JULIA_LIB_DIR})

# 3. Основной исполняемый файл
add_executable(lina_run src/main.c)

# 4. Подключаем заголовки
target_include_directories(lina_run PRIVATE 
    ${Python3_INCLUDE_DIRS} 
    ${JULIA_INCLUDE_DIR}
)

# 5. Линкуем библиотеки
target_link_libraries(lina_run PRIVATE 
    ${Python3_LIBRARIES}
    -L${JULIA_LIBRARY_DIR} -ljulia
)

# Добавляем флаги для удобства отладки
target_compile_options(lina_run PRIVATE -Wall -Wextra -g)