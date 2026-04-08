@echo off

set ENGINE=build\Hynobius.exe
set FEN="8/8/4p3/4PpP1/8/6r1/7K/k5r1 w - f6 0 1"
set DEPTH=1

%ENGINE% --fen %FEN% --stats %DEPTH%

pause