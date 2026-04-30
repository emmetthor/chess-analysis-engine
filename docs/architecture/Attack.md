# Architecture: Attack

## Implementation at

`Attack.h` and `Attack.cpp`.

---

## Purpose

Check whether a square is attacked.

Count how much time a square is attacked.

---

## Why This Architechture?

- Due to two-dimensional board design, one must enumerate every square that a square can be attacked from.
- By separating counting and checking system, it will be faster to use `isSquareAttacked()` than `countSquareAttacks()`.

---

## Scope

### Includes

- Count how many pieces are attacking the square.
- Check whether the  square is attacked.

### Excludes

- Get which piece is attacking the square.

---

## Data Flow

### isSquareAttacked()

```txt
isSquareAttacked(board, pos, player)

=== ↓ ===

isPawnAttacking(board, pos, player)
isKnightAttacking(board, pos, player)
isDiagonalAttacking(board, pos, player)
isStraightAttacking(board, pos, player)
isKingAttacking(board, pos, player)

=== ↓ ===

return bool
```

### countSquareAttacks()

```txt
isSquareAttacked(board, pos, player)

=== ↓ ===

countPawnAttacks(board, pos, player)
countKnightAttacks(board, pos, player)
countDiagonalAttacks(board, pos, player)
countStraightAttacks(board, pos, player)
countKingAttacks(board, pos, player)

=== ↓ ===

return counts
```
