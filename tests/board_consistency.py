# generate from ChatGPT
from __future__ import annotations

import argparse
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List
from cmd_output import Cmd
from uci_engine import UciEngine

cmd = Cmd()


@dataclass
class MateCase:
    fen: str = ""

def load_mate_test(path: Path, mate_depth: int) -> List[MateCase]:
    cases: List[MateCase] = []

    with path.open("r", encoding="utf-8") as f:
        for line_number, raw in enumerate(f, start=1):
            line = raw.strip()

            # empty line
            if not line:
                continue

            if line.startswith("#"):
                continue
            
            cases.append(MateCase(fen=line))
    
    return cases

def run_test_cases(engine_path: Path, cases : List[MateCase], depth: int, detail: bool) -> bool:
    print(f"Loaded {len(cases)} mate test cases")
    print(f"Testing using depth {depth}")
    print()

    pass_count = 0

    with UciEngine(engine_path) as engine:
        for index, case in enumerate(cases, start = 1):
            if (detail):
                print(f"[{index}] {case.fen}")

            engine.set_position(case.fen)
            result = engine.go_depth(depth)

            cmd.print_pass(f"pass with {result.nodes} nodes")

    print("Test completed")

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
        help="Path to board consistency test file. Note that only fen is allowed.",
    )
    parser.add_argument(
        "--depth",
        required=True,
        help="What depth to test these test cases.",
    )
    parser.add_argument(
        "--detail",
        action="store_true",
        help="Output detailed result.",
    )

    args = parser.parse_args()

    engine_path = Path(args.engine)
    file_path = Path(args.file)
    depth = int(args.depth)
    detail = bool(args.detail)

    if not engine_path.exists():
        raise FileNotFoundError(f"engine not found: {engine_path}")
    if not file_path.exists():
        raise FileNotFoundError(f"test file not found: {file_path}")
    if depth <= 0:
        raise ValueError(f"mate depth should be greater than one: {depth}")
    
    cases = load_mate_test(file_path, depth)

    if not cases:
        raise ValueError(f"no mate test cases found: {file_path}")

    ok = run_test_cases(engine_path, cases, depth, detail)

    return 0 if ok else 1

if __name__ == "__main__":
    raise SystemExit(main())