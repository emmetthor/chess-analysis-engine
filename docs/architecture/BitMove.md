# Architecture: Bit Move

## Implementation at

`Move.h`.

---

## Purpose

Uses `uin32_t` and bit operations to compress `Move` into intergers. Uses `Board` to restore to `Move`.

---

## Why This Architechture?

- Intergers are faster than `Struct Move`.
- Easy to store and use by transposition table and move ordering.

---

## Scope

### Includes

- Least needed information to restore a `BitMove` to `Move` by `Board`.
- Fast funtions to get these information from a `BitMove`.
- Function to transform `BitMove` to `Move`.
- Function to transform `Move` to `BitMove`.
- Function to transform `BitMove` to UCI adopted move string.

### Excludes

- Some information that can access using `Board`.

---

## Data Flow

### makeBitMove()

```txt
Square from
Square to
Piece promotePiece
bool isCapture
bool isCastle
bool isEnPassant
bool isPromotion

=== ↓ ===

BitMove res;
 | bit: 00000000000 | 0 | 0 | 0 | 0000 | 000000 | 000000 |
 |
 | usage: usused bits | is promotion | is en passant | is castle | is capture | Promote Piece | Square to | Square from |
```

### bitMovetoOriMove()

```txt
BitMove move

=== ↓ ===

Move res
```

### bitMoveToUCIMove()

```txt
BitMove move

=== ↓ ===

string res = [from square, such as e2][to square, such as e4][piece promoted to]
```
