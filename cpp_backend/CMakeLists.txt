cmake_minimum_required(VERSION 3.15)
project(BankManagementSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings per code quality requirements
if(MSVC)
    add_compile_options(/W4 /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include local headers directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Locate PostgreSQL libpq C library headers and binary library
find_path(LIBPQ_INCLUDE_DIR NAMES libpq-fe.h
    PATHS
    "$ENV{USERPROFILE}/tools/pgsql/include"
    "C:/Program Files/PostgreSQL/16/include"
    "C:/Program Files/PostgreSQL/15/include"
    "C:/vcpkg/installed/x64-windows/include"
    "/usr/include"
    "/usr/local/include"
)

find_library(LIBPQ_LIBRARY NAMES pq libpq libpq.lib
    PATHS
    "$ENV{USERPROFILE}/tools/pgsql/lib"
    "C:/Program Files/PostgreSQL/16/lib"
    "C:/Program Files/PostgreSQL/15/lib"
    "C:/vcpkg/installed/x64-windows/lib"
    "/usr/lib"
    "/usr/local/lib"
)

if(LIBPQ_INCLUDE_DIR AND LIBPQ_LIBRARY)
    message(STATUS "Found libpq include: ${LIBPQ_INCLUDE_DIR}")
    message(STATUS "Found libpq library: ${LIBPQ_LIBRARY}")
    include_directories(${LIBPQ_INCLUDE_DIR})
else()
    message(WARNING "libpq not found automatically by CMake. Ensure PostgreSQL/vcpkg libpq is installed.")
endif()

# Target 1: Main Bank Management Application Executable
add_executable(bank_app
    src/main.cpp
    src/Database.cpp
    src/Bank.cpp
    src/Utils.cpp
)

if(LIBPQ_LIBRARY)
    target_link_libraries(bank_app PRIVATE ${LIBPQ_LIBRARY})
endif()

# Target 2: DB-Free Unit Tests Executable
add_executable(test_algorithms
    tests/test_algorithms.cpp
    src/Bank.cpp
    src/Utils.cpp
    src/Database.cpp
)

if(LIBPQ_LIBRARY)
    target_link_libraries(test_algorithms PRIVATE ${LIBPQ_LIBRARY})
endif()
