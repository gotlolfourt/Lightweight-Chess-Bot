<div align="center">

```
╔══════════════════════════════════════════════════════════════════╗
║   ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜     L I G H T W E I G H T     ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜   ║
║   ♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟       C H E S S   B O T        ♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟   ║
║                              I N   C                              ║
║   ♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙    ═══════════════════════    ♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙   ║
║   ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖                               ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖   ║
╚══════════════════════════════════════════════════════════════════╝
```

# ♟️ Lightweight Chess Bot in C

**A fully-featured terminal chess engine powered by classic data structures and Minimax + Alpha-Beta pruning.**

![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge)
![Algorithm](https://img.shields.io/badge/Algorithm-Minimax%20%2B%20Alpha--Beta-blueviolet?style=for-the-badge)
![License](https://img.shields.io/badge/License-Educational-green?style=for-the-badge)

</div>

---

## 🎯 Overview

This project is a **single-file chess engine** written in pure C — no external libraries, no GUI, just raw systems programming and classic algorithms applied to the royal game of chess.

It was built as a demonstration of how fundamental **Computer Science data structures** can drive a fully functional, AI-powered chess game.

---

## ♟️ Board Preview

```
    a b c d e f g h
  +-----------------+
8 | r n b q k b n r | 8      r = rook    ♜     R = Rook    ♖
7 | p p p p p p p p | 7      n = knight  ♞     N = Knight  ♘
6 | . . . . . . . . | 6      b = bishop  ♝     B = Bishop  ♗
5 | . . . . . . . . | 5      q = queen   ♛     Q = Queen   ♕
4 | . . . . . . . . | 4      k = king    ♚     K = King    ♔
3 | . . . . . . . . | 3      p = pawn    ♟     P = Pawn    ♙
2 | P P P P P P P P | 2
1 | R N B Q K B N R | 1      Uppercase = White   Lowercase = Black
  +-----------------+
    a b c d e f g h
```

---

## ✨ Features

| Feature | Description |
|---|---|
| ♟️ **Human vs AI** | Play as White against a computer opponent (Black) |
| 🧠 **Minimax + Alpha-Beta** | Game tree search with aggressive branch pruning |
| 🏰 **Castling** | Kingside & queenside, with full legality checks |
| ⚡ **En Passant** | Full en passant capture support |
| 👑 **Pawn Promotion** | Choose Queen, Rook, Bishop, or Knight |
| ↩️ **Undo** | Revert the last full turn at any time |
| ⚖️ **Check Detection** | Real-time check, checkmate & stalemate detection |
| 🎛️ **Configurable Depth** | AI search depth from 1 (fast) to 4 (strongest) |
| 📊 **Move Transparency** | Prints nodes evaluated and top candidate moves after each AI turn |

---

## 🧠 Data Structures at a Glance

The engine is built around four hand-crafted data structures, each with a specific role in the chess pipeline:

```
+-------------------------------------------------------------------+
|                       CHESS ENGINE CORE                           |
|                                                                   |
|  +--------------+  generates  +-------------------------------+  |
|  |   Position   |------------>|  Linked List  (MoveList)      |  |
|  |  (64-square  |             |  Dynamic legal move storage   |  |
|  |   board)     |             +-------------------------------+  |
|  +------+-------+                           |                    |
|         | feeds                             | feeds              |
|         v                                   v                    |
|  +--------------+             +-------------------------------+  |
|  |  Stack       |             |  N-ary Tree  (GameTreeNode)   |  |
|  |  (MoveStack) |             |  Minimax + Alpha-Beta search  |  |
|  |  Undo / LIFO |             +-------------------------------+  |
|  +--------------+                           |                    |
|                                             | top moves          |
|                                             v                    |
|                             +-------------------------------+    |
|                             |  Queue  (MoveQueue)  FIFO     |    |
|                             |  Candidate move display       |    |
|                             +-------------------------------+    |
+-------------------------------------------------------------------+
```

| Structure | Type | Role |
|---|---|---|
| `MoveList` | **Linked List** | Dynamically stores all legal moves — no fixed-size array limits |
| `MoveStack` | **Stack (LIFO)** | Stores board snapshots for the `undo` command |
| `MoveQueue` | **Queue (FIFO)** | Enqueues top candidate moves after AI search for display |
| `GameTreeNode` | **N-ary Tree** | Represents every explored board state in the Minimax search tree |

---

## 🤖 How the AI Works

```
         +-------------------------------------+
         |      Your move triggers the AI...   |
         +------------------+------------------+
                            |
                            v
         +-------------------------------------+
         |  1. Generate all legal moves         |
         |     stored in a Linked List          |
         +------------------+------------------+
                            |
                            v
         +-------------------------------------+
         |  2. Build N-ary Game Tree            |
         |     up to the chosen search depth    |
         +------------------+------------------+
                            |
                            v
         +-------------------------------------+
         |  3. Minimax with Alpha-Beta pruning  |
         |     White = MAX    Black = MIN       |
         |     Prune branches that cannot       |
         |     affect the final decision        |
         +------------------+------------------+
                            |
                            v
         +-------------------------------------+
         |  4. Evaluate each leaf position      |
         |     Material count +                 |
         |     Centrality bonus per piece       |
         +------------------+------------------+
                            |
                            v
         +-------------------------------------+
         |  5. Enqueue top moves into Queue     |
         |     Print for full transparency      |
         +-------------------------------------+
```

### 🏆 Piece Values

| Piece | Symbol | Value |
|---|:---:|---:|
| Pawn   | ♙ / ♟ | 100    |
| Knight | ♘ / ♞ | 320    |
| Bishop | ♗ / ♝ | 330    |
| Rook   | ♖ / ♜ | 500    |
| Queen  | ♕ / ♛ | 900    |
| King   | ♔ / ♚ | 20,000 |

> Each piece also earns a **centrality bonus** (+0 to +3) to encourage active, center-controlling play.

---

## 🚀 Getting Started

### Option 1 — `play.bat` (Windows + TCC)

The included batch script compiles and launches the game using the **Tiny C Compiler (TCC)**.

> 📦 Download TCC: [bellard.org/tcc](https://bellard.org/tcc/)
> Expected path: `C:\Users\<you>\.tcc\tcc\tcc.exe`

```bat
play.bat
```

### Option 2 — GCC (Any Platform)

```bash
gcc -O2 -o chessbot chessbot.c
./chessbot          # Linux / macOS
chessbot.exe        # Windows
```

### Option 3 — Clang

```bash
clang -O2 -o chessbot chessbot.c
./chessbot
```

---

## 🕹️ How to Play

**Step 1 — Choose AI depth:**

```
Choose AI depth (1-4, recommended 2 or 3): 3
```

| Depth | Speed | Strength |
|:---:|---|---|
| `1` | Instant | Beginner |
| `2` | Fast | Decent *(casual play)* |
| `3` | Moderate | Strong *(recommended)* |
| `4` | Slow | Strongest |

**Step 2 — Enter moves as `source destination`:**

```
Your move: e2 e4      <- pawn advances to e4
Your move: g1 f3      <- knight hops to f3
Your move: e1 g1      <- kingside castle (king slides two squares)
Your move: e5 d6      <- en passant capture
```

**Step 3 — Pawn promotion prompt:**

```
Promote pawn to (Q=Queen, R=Rook, B=Bishop, N=Knight) [default Q]: N
Pawn promoted to N!
```

**Special commands:**

| Command | Action |
|---|---|
| `undo` | Reverts the last full turn (your move + AI reply) |
| `quit` | Exits the game |

---

## 📐 Special Move Guide

### 🏰 Castling

Move the **king two squares** toward a rook — the engine moves the rook automatically.

```
White kingside:   e1 g1   -> king e1->g1, rook h1->f1
White queenside:  e1 c1   -> king e1->c1, rook a1->d1
Black kingside:   e8 g8
Black queenside:  e8 c8
```

Castling is **illegal** if:
- The king is currently in check
- The king passes through an attacked square
- The king or involved rook has previously moved

### ⚡ En Passant

After an opponent's double pawn push, capture it *as if it only moved one square* — on the very next move only.

```
Black plays: d7 d5     <- double pawn push
White plays: e5 d6     <- en passant! The d5 pawn is removed.
```

### 👑 Pawn Promotion

When your pawn reaches the far rank you will be prompted:

| Input | Result |
|:---:|---|
| `Q` or Enter | Queen *(default — strongest)* |
| `R` | Rook |
| `B` | Bishop |
| `N` | Knight |

---

## 📁 Project Structure

```
Lightweight-Chess-Bot/
├── chessbot.c      <- Single-file engine: move gen, AI, UI, all data structures
├── play.bat        <- Windows build & run script (TCC)
└── README.md       <- You are here
```

---

## ⚠️ Known Limitations

- No time control or move clock
- No FEN import / export
- No opening book or endgame tablebase

---

## 📄 License

Released for **educational purposes**. Feel free to use, study, fork, and learn from it.

---

<div align="center">

*Built with pure C — no dependencies, no compromises.*

</div>
