cmake_minimum_required(VERSION 3.16)

# ═══════════════════════════════════════════════════════════════════════════
#  Detect MSYS2 UCRT64 and force its toolchain (works from PowerShell too)
# ═══════════════════════════════════════════════════════════════════════════
set(MSYS2_UCRT64 "C:/msys64/ucrt64")
if(EXISTS "${MSYS2_UCRT64}")
    message(STATUS "Found MSYS2 UCRT64 at ${MSYS2_UCRT64}")

    # Force MSYS2 UCRT64 compiler (MUST be set before project())
    set(CMAKE_C_COMPILER   "${MSYS2_UCRT64}/bin/gcc.exe"   CACHE FILEPATH "" FORCE)
    set(CMAKE_CXX_COMPILER "${MSYS2_UCRT64}/bin/g++.exe"   CACHE FILEPATH "" FORCE)

    # Tell CMake where to find Qt6
    list(APPEND CMAKE_PREFIX_PATH "${MSYS2_UCRT64}")

    # ⚠ Critical: Qt DLLs must be on PATH so CMake can run moc.exe during AUTOMOC
    # moc.exe depends on Qt6Core.dll which lives in C:/msys64/ucrt64/bin
    set(ENV{PATH} "${MSYS2_UCRT64}/bin;${MSYS2_UCRT64}/share/qt6/bin;$ENV{PATH}")
endif()

project(MatrixGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# ── Find Qt6 ──
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# ── Include the core library headers (project root) ──
include_directories(${CMAKE_SOURCE_DIR}/..)

# ── Source files ──
set(SOURCES
    main.cpp
    mainwindow.cpp
    matrix_widget.cpp
)

set(HEADERS
    mainwindow.h
    matrix_widget.h
)

add_executable(matrix_gui ${SOURCES} ${HEADERS})

target_link_libraries(matrix_gui PRIVATE Qt6::Widgets)
