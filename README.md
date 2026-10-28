# ♟️ Chess Bot in C

A terminal-based chess engine written in pure C, featuring a playable AI opponent powered by classic data structures and the **Minimax algorithm with Alpha-Beta pruning**.

This project was built as a demonstration of fundamental Computer Science data structures applied to a real-world problem — a fully functional chess game.

---

## 🎮 Features

- **Play as White** against an AI opponent (Black)
- **Minimax + Alpha-Beta Pruning** via an N-ary Game Tree
- **Undo moves** at any time using a LIFO Stack
- **Configurable AI depth** (1–4 plies; depth 2–3 recommended)
- Detects **Checkmate**, **Stalemate**, and **Check**
- Automatic **pawn promotion** to queen
- Clean, readable **ASCII board display**

---

## 🧠 Data Structures Used

| Structure | Role |
|---|---|
| **Linked List** | Dynamic legal move generation (`MoveList`) |
| **Stack (LIFO)** | Move history for undo functionality (`MoveStack`) |
| **Queue (FIFO)** | Candidate move display after AI search (`MoveQueue`) |
| **N-ary Tree** | Game tree for Minimax search (`GameTreeNode`) |

---

## 📁 Project Structure

```
chess/
├── chessbot.c   # Full source code (single-file)
├── play.bat     # Windows build & run script (uses TCC)
└── README.md
```

---

## 🚀 Getting Started

### Option 1: Using the included `play.bat` (Windows + TCC)

The `play.bat` script compiles and launches the game using the **Tiny C Compiler (TCC)**.

> Make sure TCC is installed. The script expects it at:
> `C:\Users\<you>\.tcc\tcc\tcc.exe`
>
> Download TCC from: https://bellard.org/tcc/

Double-click `play.bat` or run it from a terminal:

```bat
play.bat
```

### Option 2: Using GCC (Any Platform)

```bash
gcc -O2 -o chessbot chessbot.c
./chessbot        # Linux / macOS
chessbot.exe      # Windows
```

### Option 3: Using Clang

```bash
clang -O2 -o chessbot chessbot.c
./chessbot
```

---

## 🕹️ How to Play

1. Run the program and choose an **AI depth** (1–4):
   - `1` — Very fast, weak AI
   - `2` — Fast, decent AI *(recommended for beginners)*
   - `3` — Moderate speed, stronger AI *(recommended)*
   - `4` — Slow, strongest AI

2. The board is displayed with White pieces as **uppercase** (`P N B R Q K`) and Black pieces as **lowercase** (`p n b r q k`).

3. Enter your move as **source square → destination square**:
   ```
   e2 e4
   g1 f3
   ```

4. Special commands:
   | Command | Action |
   |---|---|
   | `undo` | Reverts the last full turn (your move + AI's reply) |
   | `quit` | Exits the game |

---

## 📐 Board Representation

```
    a b c d e f g h
  +-----------------+
8 | r n b q k b n r | 8
7 | p p p p p p p p | 7
6 | . . . . . . . . | 6
5 | . . . . . . . . | 5
4 | . . . . . . . . | 4
3 | . . . . . . . . | 3
2 | P P P P P P P P | 2
1 | R N B Q K B N R | 1
  +-----------------+
    a b c d e f g h
```

- `.` = Empty square
- Uppercase = White pieces
- Lowercase = Black pieces

---

## ⚙️ How the AI Works

1. **Move Generation** — All legal moves are generated into a dynamic **Linked List**, ensuring no memory overflows.
2. **Game Tree** — An **N-ary tree** is built up to the chosen depth, with each node representing a board position.
3. **Minimax** — White maximizes the score, Black minimizes it.
4. **Alpha-Beta Pruning** — Branches that cannot affect the final decision are pruned, significantly reducing the number of nodes evaluated.
5. **Evaluation** — Each leaf position is scored by material count (piece values) plus a small **centrality bonus** to encourage active play.
6. **Move Queue** — After the search, the top candidate moves are enqueued into a **FIFO Queue** and printed for transparency.

### Piece Values

| Piece | Value |
|---|---|
| Pawn | 100 |
| Knight | 320 |
| Bishop | 330 |
| Rook | 500 |
| Queen | 900 |
| King | 20000 |

---

## ⚠️ Known Limitations

- No **castling** support
- No **en passant** support
- Pawn promotes to **queen only**
- No time control or move clock
- No FEN import/export

---

## 📄 License

This project is released for educational purposes. Feel free to use, modify, and learn from it.
