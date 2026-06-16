#ifndef types_h
#define types_h

#include <vector>
#include <string>
#include <cstdint>

enum Piece : int
{
    empty = 0,
    wP = 1, wN = 2, wB = 3, wR=4, wQ = 5, wK = 6,
    bP = 7,bN = 8,bB = 9,bR = 10,bQ = 11,bK = 12
};

enum Color : int {white = 0, black = 1};

enum {castle_wk = 1, castle_wq = 2, castle_bk = 4, castle_bq = 8};
// this saves memory, instead of using 4 booleans we do it one.

// board state fully and at any point here
struct Board{
    int squares[64];
    int side_to_move;
    int castling_rights;
    int ep_square_number; // en passant target square, -1 if none
    int half_move_clock; // move since last pawn push or capture
    int full_move_number; // black moved to complete a full move(move done by both sides)
};

struct Move
{
    int from; // from square 
    int to; // to square
    int captured; // struct move has this
    int promoted; // struc move has this
    int flags; // 0 = normal, 1 = castling, 2 = en passant, 3 = promoted
};

//  inline functions to get the square. 
//  these are helper functions

inline int rank_of(int sq) {return sq/8;}
inline int file_of(int sq) {return sq% 8;}
inline int sq_of(int rank, int file) {return rank * 8 + file; }

// bool to check piece color
inline bool is_white_piece(int p){return p>=wP && p<=wK; }
inline bool is_black_piece(int p){return p>=bP && p<=bK; }

// piece color

inline int piece_color (int p)
{
    if (p>=wP && p<=wK ) return white;
    if (p>=bP && p<=bK) return black;
    return -1;  // empty or invalid piece
};

inline int piece_type (int p) {
    if (p>=wP && p<=wK) return p;
    if (p>=bP && p<=bK) return p-6;
    return 0;
}

#endif
