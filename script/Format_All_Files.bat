@echo off
setlocal enabledelayedexpansion

set COUNT=0
echo Formatting tracked files...

for /f "delims=" %%f in ('git ls-files "*.cpp" "*.cc" "*.h" "*.hpp"') do (
    echo Formatting %%f
    clang-format -i "%%f"
    set /a COUNT+=1
)

echo.
echo Formatted !COUNT! files.
pause