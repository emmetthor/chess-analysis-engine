from __future__ import annotations
from pathlib import Path
import argparse
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List
from cmd_output import Cmd
from uci_engine import UciEngine

@dataclass
class RegressionCase:
    name: str
    fen: str
    depth: int
    bestmoves: List[str] = field(default_factory=List)


def load_regression_file(path: str | Path) -> List[RegressionCase]:
    lines = Path(path).read_text(encoding="utf-8").splitlines()

    cases: List[RegressionCase] = []

    current_name = ""
    current_fen: str | None = None
    current_bestmoves: List[str] = []
    current_depth: int = 0

    def flush_case(line_no: int) -> None:
        nonlocal current_name, current_fen, current_bestmoves, current_depth

        # Empty block: ignore.
        if not current_name and current_fen is None and not current_bestmoves:
            return

        if current_fen is None:
            raise ValueError(f"missing fen before line {line_no}")
        
        if not current_depth:
            raise ValueError(f"missing depth before line {line_no}")

        if not current_bestmoves:
            raise ValueError(f"missing bestmove before line {line_no}")

        case_name = current_name or f"case_{len(cases) + 1}"

        cases.append(
            RegressionCase(
                name=case_name,
                fen=current_fen,
                depth=current_depth,
                bestmoves=current_bestmoves,
            )
        )

        current_name = ""
        current_fen = None
        current_depth = 0
        current_bestmoves = []

    for line_no, raw_line in enumerate(lines, start=1):
        line = raw_line.strip()

        if not line:
            flush_case(line_no)
            continue

        if line.startswith("#"):
            current_name = line[1:].strip()
            continue

        if line.startswith("fen:"):
            current_fen = line[len("fen:"):].strip()
            continue

        if line.startswith("depth:"):
            current_depth = int(line[len("depth:"):].strip())
            continue

        if line.startswith("bestmove:"):
            moves_str = line[len("bestmove:"):].strip()
            current_bestmoves = moves_str.split()
            continue

        raise ValueError(f"unknown line format at line {line_no}: {line}")

    # Flush last case if file does not end with an empty line.
    flush_case(len(lines) + 1)

    return cases

cmd = Cmd()

def run_test_cases(engine_path: Path, cases : List[RegressionCase], detail: bool) -> bool:
    print(f"Loaded {len(cases)} regression cases")
    print()

    pass_count = 0

    with UciEngine(engine_path) as engine:
        for index, case in enumerate(cases, start = 1):
            if (detail):
                print(f"[{index}] {case.fen} with depth {case.depth}")

            engine.set_position(case.fen)
            result = engine.go_depth(case.depth)

            if result.bestmove in case.bestmoves:
                pass_count += 1
                if (detail):
                    cmd.print_pass("pass")
            else:
                if (detail):
                    cmd.print_fail("fail")
                    cmd.print_fail(f"expected moves: {case.bestmoves}")
                    cmd.print_fail(f"actual mate score: {result.mate_score}")
                    cmd.print_fail(f"actual score cp: {result.cp_score}")
                    cmd.print_fail(f"actual move: {result.bestmove}")

    print(f"Test completed with [{pass_count} / {len(cases)}], pass rate: {pass_count / len(cases) * 100}")

    return True

def main() -> int:
    parser = argparse.ArgumentParser(description="Run perft tests against engine CLI")
    parser.add_argument(
        "--engine",
        required=True,
        help="Path to engine executable, e.g. build/Hynobius.exe",
    )
    parser.add_argument(
        "--file",
        required=True,
        help="Path to regresstion test file. Note that only fen is allowed.",
    )
    parser.add_argument(
        "--detail",
        action="store_true",
        help="Output detailed result.",
    )

    args = parser.parse_args()

    engine_path = Path(args.engine)
    file_path = Path(args.file)
    detail = bool(args.detail)

    if not engine_path.exists():
        raise FileNotFoundError(f"engine not found: {engine_path}")
    if not file_path.exists():
        raise FileNotFoundError(f"test file not found: {file_path}")
    
    cases = load_regression_file(file_path)

    if not cases:
        raise ValueError(f"no mate test cases found: {file_path}")

    ok = run_test_cases(engine_path, cases, detail)

    return 0 if ok else 1

if __name__ == "__main__":
    raise SystemExit(main())