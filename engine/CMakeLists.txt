cmake_minimum_required(VERSION 3.16)
project(chess-analysis-engine)

set(CMAKE_CXX_STANDARD 17)

# 告訴 CMake include 目錄在哪
include_directories(include)

# 目標檔案
set(SOURCE_FILES
    src/board/Piece.cpp
    src/board/Board.cpp
    src/board/Move.cpp

    src/evaluate/Material_Point.cpp
    src/evaluate/Evaluate.cpp

    src/pgn/Pgn_Parser.cpp
    src/pgn/Valid_Piece.cpp
    src/pgn/Pgn_Transformer.cpp

    src/debug.cpp
    src/main.cpp
)

# 建立執行檔
add_executable(chess-analysis-engine ${SOURCE_FILES})

set_target_properties(chess-analysis-engine PROPERTIES
    WIN32_EXECUTABLE OFF
)