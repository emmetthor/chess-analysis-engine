@echo off

echo ===
echo Running Board Consistency test...
echo ===

python tests\board_consistency.py --engine build/Hynobius.exe --file tests/board_consistency/openings_bc.txt --depth 2 --detail
python tests\board_consistency.py --engine build/Hynobius.exe --file tests/board_consistency/middle_game_bc.txt --depth 2 --detail
python tests\board_consistency.py --engine build/Hynobius.exe --file tests/board_consistency/end_game_bc.txt --depth 2 --detail
python tests\board_consistency.py --engine build/Hynobius.exe --file tests/board_consistency/stress_bc.txt --depth 2 --detail

echo ===
echo Running Perft...
echo ===

python ./tests/Perft_Test.py --engine build/Hynobius.exe --file tests/perft/Perft_Tests.txt --max-depth 4

echo ===
echo Running Regression...
echo ===

python tests\regression_test.py --engine build/Hynobius.exe --file tests/regression/regressions.txt --detail

echo ===
echo Running Checkmate test...
echo ===

python tests\checkmate.py --engine build/Hynobius.exe --file tests/checkmate/Mate_In_One.txt --mate-depth 2 --detail

python tests\checkmate.py --engine build/Hynobius.exe --file tests/checkmate/Mate_In_Two.txt --mate-depth 2 --detail

