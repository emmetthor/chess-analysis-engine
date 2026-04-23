# 2026-02-08_Check&Piece_CR

## 修正重點

- 更新簡易函數為 `inline`。
- 修正其餘檔案中有關於 `Piece` 修正的錯誤。


## 問題發現
- `Check.cpp` 中每次都需每次O(64) 掃一遍棋盤，建議用陣列紀錄棋子位置。
- `Piece.cpp` 應該包含由 `Player` 加上棋子轉成 `Piece` 的函式，但其放在 `Pgn_Transformer` 並使用 `map` 降速。已更改。
- 常用並簡潔的函式應使用 `inline` 清楚語意。
- `enum` 應使用 `class`，並加上 `sstatic_cast<>()` 清楚語意且增加安全性。