import subprocess
import sys
import re
from pathlib import Path

BASE_DIR = Path(__file__).parent
ENGINE_PATH = r"./build/Hynobius.exe"
REGRESSION_FILE = BASE_DIR / "regressions.txt"
SEARCH_DEPTH = 8

def parse_regression_file(path: str):
    text = Path(path).read_text(encoding="utf-8")
    lines = text.splitlines()

    cases = []
    current_name = ""
    current_fen = None
    current_bestmoves = None

    for raw_line in lines:
        line = raw_line.strip()

        if not line:
            if current_fen and current_bestmoves:
                cases.append({
                    "name": current_name or f"case_{len(cases)+1}",
                    "fen": current_fen,
                    "bestmoves": current_bestmoves,
                })
            current_name = ""
            current_fen = None
            current_bestmoves = None
            continue

        if line.startswith("#"):
            current_name = line[1:].strip()
            continue

        if line.startswith("fen:"):
            current_fen = line[len("fen:"):].strip()
            continue

        if line.startswith("bestmove:"):
            moves_str = line[len("bestmove:"):].strip()
            current_bestmoves = moves_str.split()
            continue

    # 檔尾最後一筆沒有空行時也要補進去
    if current_fen and current_bestmoves:
        cases.append({
            "name": current_name or f"case_{len(cases)+1}",
            "fen": current_fen,
            "bestmoves": current_bestmoves,
        })

    return cases


def run_engine(engine_path: str, fen: str, depth: int):
    cmd = [
        engine_path,
        "--fen", fen,
        "--search-with-info", str(depth),
    ]

    proc = subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        encoding="utf-8",
    )
    return proc.returncode, proc.stdout, proc.stderr


def parse_bestmove(stdout: str):
    patterns = [
        r"bestmove\s*=\s*([a-h][1-8][a-h][1-8][qrbn]?)",
        r"\bbestmove\s+([a-h][1-8][a-h][1-8][qrbn]?)",
    ]

    for pattern in patterns:
        m = re.search(pattern, stdout)
        if m:
            return m.group(1)

    return None


def main():
    cases = parse_regression_file(REGRESSION_FILE)

    if not cases:
        print("No regression cases found.")
        return 1

    print(f"Loaded {len(cases)} regression cases from {REGRESSION_FILE}")
    print("=" * 60)

    passed = 0

    for idx, case in enumerate(cases, start=1):
        name = case["name"]
        fen = case["fen"]
        expected_moves = case["bestmoves"]

        code, stdout, stderr = run_engine(ENGINE_PATH, fen, SEARCH_DEPTH)
        actual_move = parse_bestmove(stdout)

        ok = (code == 0) and (actual_move in expected_moves)

        print(f"[{idx}/{len(cases)}] {'PASS' if ok else 'FAIL'} - {name}")
        print(f"  FEN      : {fen}")
        print(f"  Expected : {' '.join(expected_moves)}")
        print(f"  Actual   : {actual_move}")
        print(f"  ExitCode : {code}")

        if not ok:
            if stdout.strip():
                print("  STDOUT:")
                for line in stdout.strip().splitlines():
                    print(f"    {line}")
            if stderr.strip():
                print("  STDERR:")
                for line in stderr.strip().splitlines():
                    print(f"    {line}")
        else:
            passed += 1

        print("-" * 60)

    print(f"Summary: {passed}/{len(cases)} passed")
    return 0 if passed == len(cases) else 1


if __name__ == "__main__":
    sys.exit(main())