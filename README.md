# onyxchess
A C++ chess game featuring a Negamax engine with alpha-beta pruning.
This website gives a tutorial of how i made onyxchess in c++ [Live Link](https://onyxchessweb.vercel.app/)<br/>


### Technologies used:
1. C++17
2. C++ Standard Library (STL)
3. Clang (C++ Compiler)
4. Makefile (Build Automation)


### Functionality
- Play chess through a C++ chess engine.
- Generate and validate legal chess moves.
- Handle special moves such as castling, en passant, and pawn promotion.
- Evaluate board positions using the PeSTO evaluation function.
- Calculate optimal moves using Minimax and Negamax algorithms.
- Optimize search using alpha-beta pruning.
- Prioritize captures using MVV-LVA move ordering.
- Automatically select and execute computer moves.

### How to Run Locally

Step 1 - Clone the repository in a new directory:
```bash
git clone https://github.com/sarthak-wadhawan/onyxchess.git
```

Step 2 - Navigate to the cloned repository:
``` bash 
cd OnyxChess
```


Step 3 – Compile the C++ source code:
``` bash
make
```


Step 4 – Run the chess engine:
``` bash
./chess
```


Step 5 – Enter chess moves using coordinate notation, for example:
``` bash
e2e4
```


Step 6 – To exit the game, enter:
``` bash
quit
```
