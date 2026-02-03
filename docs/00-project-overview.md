# project overview

## 1. 設計目標

- 可分析棋局，並給予著法回饋如：「此著是 ??大漏著 ，因為下一步你的 Queen 就要被 Fork 了」
- 可簡單下棋，與使用者對戰
- 可在網頁上呈現最佳著法，並以箭頭表示
- 有明確功能分區，讓程式碼不混亂

## 2. 模組劃分

### `board/`

| 檔案名稱 | 功能 |
| - | - |
| Attack | 全盤攻擊圖 (`AttackMap`)、檢查格子攻擊 (`countSquareAttacks`) |
| Board | 棋盤主體 |
| Check | 檢查玩家是否被將軍 (`isInCheck`)|
| Generate_Move | 生成目前盤面上所有合法著法 (`generateAllLegalMoves`)、生成所有合法吃子 (`generateLegalCaptureMoves`) |
| Generate_Position | 生成特定棋子可移動位置 (`generatePiecePosFromPos`)、生成可吃子位置 (`generatePieceCaptureFromPos`) |
| Move | 移動主體，執行與反悔移動 (`makeMove & undoMove`)、檢查移動合法性 (`isMoveLegal`) |
| Piece | 定義棋子、檢查棋子顏色 (`isWhite & isBlack & isSameColor`) |

### `evaluate/`

| 檔案名稱 | 功能 |
| - | - |
