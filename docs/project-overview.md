# project overview

## Design Goal

Hynobius is an engine for people to understand how an engine actually works. Thus, there are several goals this project would like to achieve:

- Use **easy** and **readable** codes to implement. (So NO complecated bitboard and bit tricks)
- Write **detailed** comments.
- Write `tutorals/` documents to store **basic knowledge to build an engine**.
- Write `git_usage/` documents to understand how `git` works and why it is important.
- Write `optimization/` documents to store **how can an engine improve itself** by scientific tests. (such as A/B tests)

## Current Documents Label

| Documents | Content |
| - | - |
| `engine/` | Store main architectures, function designs. (TODO) |
| `github_usage/` | Store github utils and usage. |
| `img/` | Store images. |
| `optimization/` | Store testing usage and means to improve an engine. |
| `tutorials/` | Store basic knowledge. |

## Features

### Search

- Alpha-beta prunting with iterative deepeing.
- Move ordering (PV move, TT move, killer moves).
- Transportation table (TT).
- Aspiration window.
- Late move reduction (LMR).
- Quietscene search with check evasions.

### Evaluation

- Material scores.
- Basic Piece-square tables (PST).

### Scientific Testing

- Automatic AB testing with **[Cute Chess](https://github.com/cutechess/cutechess)** and GitHub actions.
- Elo tracking.
- Self-hosted runner.

### Web Analysis

- WASM-compiled engine running is browser.
- Interface board and PGN analysis.

## System Architecture

## Data Flow

### UCI

(todo)

### CLI

(todo)

## Performance

- **[Go see and play with HynobiusChess BOT right here](https://lichess.org/@/HynobiusChess)**.
- **~1700 elo** on bliz (based on Lichess elo).

## Roadmap

- Improve time management.
- Improve move ordering and search speed.
