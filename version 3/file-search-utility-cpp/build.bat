@echo off
:: ═══════════════════════════════════════════════════════════════
::  File Search Utility (C++ / SOLID) — Build Script
::  Requires : MinGW-w64  (g++) on PATH
::  Usage    : Double-click  OR  run in cmd
:: ═══════════════════════════════════════════════════════════════

setlocal

set CXX=g++
set BIN=bin\FileSearch.exe

set SRC=src\main.cpp ^
        src\gui\MainWindow.cpp ^
        src\gui\ResultsListView.cpp ^
        src\search\FileNameSearchStrategy.cpp ^
        src\search\ExtensionSearchStrategy.cpp ^
        src\search\ContentSearchStrategy.cpp ^
        src\utils\FileUtils.cpp

set CXXFLAGS=-std=c++17 -Wall -Wextra -O2 -Iinclude
set LIBS=-lcomctl32 -lshell32 -lole32 -luser32 -lgdi32 -mwindows

if not exist bin   mkdir bin
if not exist build mkdir build

echo.
echo [*] Compiling File Search Utility (C++ / SOLID)...
echo.

%CXX% %CXXFLAGS% %SRC% -o %BIN% %LIBS%

if %ERRORLEVEL% == 0 (
    echo.
    echo [OK]  Build successful^^!
    echo       Output : %BIN%
    echo.
) else (
    echo.
    echo [FAIL] Build failed. See errors above.
    echo.
    exit /b 1
)

endlocal
