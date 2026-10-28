@echo off
cd /d "%~dp0"
echo Compiling chessbot.c...
"C:\Users\leita\.tcc\tcc\tcc.exe" chessbot.c -o chessbot.exe
if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b 1
)
chessbot.exe
pause
