@echo off
echo Running Perft...

python ./tests/perft/Perft_Test.py --engine build/Hynobius.exe --file tests/perft/Perft_Tests.txt --max-depth 4

echo ===
echo Running Regression...

python tests\regression_test.py --engine build/Hynobius.exe --file tests/regression/regressions.txt --detail

echo ===
echo Running Checkmate test...

python tests\checkmate.py --engine build/Hynobius.exe --file tests/checkmate/Mate_In_Two.txt --mate-depth 2 --detail