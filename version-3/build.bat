@echo off
echo ========================================
echo   File Search Utility - Build Script
echo ========================================

:: Output directory তৈরি করো
if not exist "out" mkdir out

:: সব Java file compile করো
echo Compiling...
javac -d out -sourcepath src src/com/filesearch/main/Main.java

if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed!
    pause
    exit /b 1
)

echo Build successful!
echo.
echo Run করতে লেখো:
echo   java -cp out com.filesearch.main.Main
echo.
pause
