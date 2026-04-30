@echo off
echo Running Perft...

python ./tests/perft/Perft_Test.py --engine build/Hynobius.exe --file tests/perft/Perft_Tests.txt --max-depth 4

echo Running Regression...

python tests/regression/regression_test.py --engine build/Hynobius.exe --file tests/regression/regressions.txt --depth 8