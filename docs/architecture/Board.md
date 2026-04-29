# Architecture: Board

## Implementation at

`Board.h` and `Board.cpp`.

---

## Purpose

Store nessesary information to play chess.

---

## Why This Architechture?

For code readability, no `bitboard` is used because it is too hard to implement and understand.

---

## Scope

### Includes

- 8x8 board array.
- every pieces' positions (`piecePos[][]`).
- every pieces' count on the board (`pieceCnt[]`).
- material score.
- PST score.
- side to play (`player`).
- castling rights.
- zobrist key for the current board.

### Excludes

TODO.

---

## Data Flow

TODO.
