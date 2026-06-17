#include "board.h"
#include<iostream>


// 
// int    x;    // type = int,    name = x
// string name; // type = string, name = name
// Board  b;    // type = Board,  name = b
// Board& b;    // type = Board reference, name = b

void initialize_board(Board& b)
{int start_pos[64] = {
     wR, wN, wB, wQ, wK, wB, wN, wR,
        wP, wP, wP, wP, wP, wP, wP, wP,
        empty, empty, empty, empty, empty, empty, empty, empty,
        empty, empty, empty, empty, empty, empty, empty, empty,
        empty, empty, empty, empty, empty, empty, empty, empty,
        empty, empty, empty, empty, empty, empty, empty, empty,
        bP, bP, bP, bP, bP, bP, bP, bP,
        bR, bN, bB, bQ, bK, bB, bN, bR
};
for (int i= 0; i<=64; i++) b.squares[i] = start_pos[i];
// You might wonder why not just do b.squares = startPos. 
// In C++, you can't directly assign one raw array to another — you have to copy element by element

b.side_to_move = white;
b.castling_rights = castle_wk | castle_wq | castle_bk | castle_bq;
b.ep_square_number = -1;
b.half_move_clock = 0;
b.full_move_number = 1;

};

void print_board(const Board& b)
{
// Unicode symbols indexed by piece code (0=empty, 1-6=white, 7-12=black)
    const std::string symbols[] = 
    {
        ".", "\u2659", "\u2658", "\u2657", "\u2656", "\u2655", "\u2654",
             "\u265F", "\u265E", "\u265D", "\u265C", "\u265B", "\u265A"
};

    std::cout << "\n";

for (int rank =7; rank >= 0; rank--) {
    std:: cout<< (rank + 1) << " ";
    for (int file = 0; file <8; file++)
    {
        int piece = b.squares[sq_of(rank, file)];
        std::cout<< symbols[piece] << " ";
    }
        std::cout << "\n";
std::cout << "   a b c d e f g h\n\n";
}

}

std :: string sq_to_algebraic(int sq) {
     // Convert square index to algebraic notation (e.g., 0 -> "a1")
     std :: string s;
     s+=(char) ('a' + file_of(sq));
     s+=(char) ('1' + rank_of(sq));
     return s;

}

int algebaraic_to_sq(const std:: string& s){
    if (s.length() < 2 ) return -1;
    int file = s[0] - 'a';
    int rank = s[1] - '1';
    if (file<0 || file > 7 || rank<0 || rank>7) return -1;
    return sq_of(rank, file);
}
int find_king(const Board& b, int color) {
    int king = (color== white) ? wK: bK;
    //  if color == white int king = wk else int king ==bk;
    for (int i = 0; i <64 ; i++){
        if (b.squares[i]==king) return i;
    }
    return -1;
}

bool is_sq_attacked(const Board& b, int sq, int by_color){
    int rank = rank_of(sq), file = file_of(sq);

    // check for knight attacks first.
    // 8 possible moves for each night. 8 rows 2 cols (left and right), rank and file pair
    int knight_offsets[8][2] = {{-2,-1},{-2,1},{2,-1},{2,1},{-1,-2},{-1,2},{1,2},{1,-2}};
    int enemy_knight = (by_color == white) ? wN : bN;
    for (auto& off : knight_offsets){
int r = rank + off[0], f= file + off[1];
if (r>=0&& r<8 && f>=0 && f<8){
    if (b.squares[sq_of(r,f)]== enemy_knight) return true;
}
    }


//  pawns, just go 1 up and left and right

if (by_color == white){
    if (rank >0){
        if (file>0 && b.squares[sq_of(rank-1,file-1)]==wP) return true;
        if (file<7 && b.squares[sq_of(rank-1,file+1)]==wP) return true;
    }
}
else{
    if(rank<7){
                if (file>0 && b.squares[sq_of(rank+1,file-1)]==bP) return true;
                if (file<7 && b.squares[sq_of(rank+1,file+1)]==bP) return true;


    }
}

// check for king, check all 4 sides
int enemy_king = (by_color==white) ? wK:bK;
for (int dr = -1; dr <=1; dr++){
    for (int df = -1; df<=1; df++){
        if (dr == 0 && df == 0) continue;
        int r = rank + dr;
        int f = file + df;
        if (r>=0 && r<8 && f>=0 && f<8){
        if (b.squares[sq_of(r,f)] == enemy_king) return true;
        }
    }
}

// sliding pieces on diagnols
int enemy_queen= (by_color==white) ? wQ:bQ;
int enemy_bishop= (by_color==white) ? wB:bB;
int enemy_rook= (by_color==white) ? wR:bR;

int diag_dirs[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
for (auto& dir : diag_dirs){
    int r = rank + dir[0], f = file + dir[1];
    while (r >= 0 && r < 8 && f>=0 && f<8){
        int p = b.squares[sq_of(r,f)];
        if (p!= empty){
            if (p == enemy_bishop || p == enemy_queen) return true;
            break;
        }
        r+= dir[0], f+= dir[1];
    }
}



//  rook and queen sliding
//  goes up+ down left plus right 0+1 and 0-1 and vice versa
int straight_dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
for (auto& dir : straight_dirs){
    int r = rank + dir[0], f = file + dir[1];
        while (r >= 0 && r < 8 && f>=0 && f<8){
            int p = b.squares[sq_of(r,f)];
            if (p!= empty ){
                if (p == enemy_rook || p == enemy_queen )return true;
                break;
            }
            r+= dir[0], f+= dir[1];

}

}
return false;

}


//  check detection and board logic
bool is_in_check(const Board& b, int color){
    int king_square = find_king(b,color);
    return is_sq_attacked(b, king_square, 1-color);
}

// 
void make_move(Board& b, const Move& m){
    int piece = b.squares[m.from];
    // handle en passant and remove the pawn from its actual position.
    if (m.flags==2){
        int captured_pawn_sq = sq_of(rank_of(m.from), file_of(m.to));
        b.squares[captured_pawn_sq] = empty;
    }
b.squares[m.to]= piece;
b.squares[m.from] = empty;

//  promotion
if (m.flags==3){
    b.squares[m.to] = m.promoted;
}

// castling rights
if (m.flags==1){
    if (m.to == sq_of(0,6)){
        b.squares[sq_of(0,5)]= wR;
        b.squares[sq_of(0,7)] = empty;}
    
    else if (m.to == sq_of(0, 2)) 
    { 
        b.squares[sq_of(0, 3)] = wR; 
        b.squares[sq_of(0, 0)] = empty; 
    }
    
    else if (m.to == sq_of(7, 6)) 
    { 
        b.squares[sq_of(7, 5)] = bR; 
        b.squares[sq_of(7, 7)] = empty; }
    
    else if (m.to == sq_of(7, 2)) 
    {
         b.squares[sq_of(7, 3)] = bR; 
         b.squares[sq_of(7, 0)] = empty; }
    }

    b.ep_square_number = -1;
    if ((piece == wP || piece == bP) && std::abs(rank_of(m.to) - rank_of(m.from)) == 2 )
    {
        b.ep_square_number = sq_of((rank_of(m.from) + rank_of(m.to)) / 2,file_of(m.from));
    }

    // Update castling rights when king or rook moves
    if (piece == wK) b.castling_rights &= ~(castle_wk | castle_wq);
    if (piece == bK) b.castling_rights &= ~(castle_bk | castle_bq);
    if (m.from == sq_of(0, 0) || m.to == sq_of(0, 0)) b.castling_rights &= ~castle_wq;
    if (m.from == sq_of(0, 7) || m.to == sq_of(0, 7)) b.castling_rights &= ~castle_wk;
    if (m.from == sq_of(7, 0) || m.to == sq_of(7, 0)) b.castling_rights &= ~castle_bq;
    if (m.from == sq_of(7, 7) || m.to == sq_of(7, 7)) b.castling_rights &= ~castle_bk;

// Update half-move clock (resets on pawn move or capture)
if (piece == wP || piece == bP || m.captured != empty) {
    b.half_move_clock = 0;
} else {
    b.half_move_clock++;
}
// Switch side to move and increment full move number after Black
if (b.side_to_move == black) b.full_move_number++;
b.side_to_move = 1 - b.side_to_move;

// close fn make move
}