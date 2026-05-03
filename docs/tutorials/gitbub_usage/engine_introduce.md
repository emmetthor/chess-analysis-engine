# Chess Engine

## Describe in One Sentence

An AI or an algorithm that plays chess **VERY WELL**.

---

## Introduction

A chess engine is a program designed to play chess or analyze chess.

Unlike human chess players,it doesn't "think" or "understand" chess like we do.
Instead, It **represents the board using data structures, generate legal moves, search for future possibilities, and evaluate positions using math**.

### How it works?

There are basically two kinds of engine.
One uses search and evaluate algorithms, the other uses neural nets to play chess.

#### Search + Evaluation

Traditional chess engines like Stockfish, the most powerful chess engines in the world, uses search algorithms and evaluate algorithms to play chess.

For a board like the picture below:

![startpos by chess.com](../../img/startpos.png)

(The picture comes from chess.com.)

It will **generate moves**, which means the engine will play **every possible moves in its mind**:

![depth1 by chess.com](../../img/depth1.gif)

(The picture comes from chess.com.)

And evaluate them to analyze which move is the best.

Although the GIF only represents every move we can play as white, but the engine **does not only search for these moves**.
The engine will also search from the board when the previous move has made, which means:

![all possible moves by chess.com](../../img/all_possible_moves.png)

(The picture comes from chess.com.)

That is how the engine will recurse itself and keep searching.

#### Neural Net

TODO sorry.

However, a technique called **Efficiently updatable neural network** was invented in around 2018.
It is a neural net but it can update fast, which means **it is stronger than man-made evaluation algorithms**. Thus it has been used in almost every top chess engines.

### Core Components

#### Board

#### Move generation

#### Search

#### Evaluate

---

## Application
