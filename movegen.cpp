#include "movegen.h"
#include "board.h"
#include <cstdlib>

// we will add moves for all then checks for moves then a fn to remove the moves that leae the king in check

static void add_pawn_moves(const Board& b, std::vector<Move>& moves)
{
int color = b.side_to_move;
int pawn = (color == white) ? wP:bP;
int dir = (color == white)? 1:-1;
int start_rank = (color == white) ? 1:6;
int promo_rank = (color == white) ? 7:0;

for(int sq =0 ; sq<64; sq++){
    if (b.squares[sq]!= pawn)continue;
int rank = rank_of(sq), file = file_of(sq);
int to_sq = sq_of(rank + dir, file);

if (b.squares[to_sq]== empty){

    if (rank_of(to_sq)== promo_rank)
    {
        // promote to all 4 piece types, use a for loop to increment from knight upwards
        int base = (color == white) ? wN :bN ;
        for (int promo = base; promo <=base+3 ; promo ++)
        {
            moves.push_back({sq, to_sq, promo, empty, 3});
        }

    }
    else 
    {
        moves.push_back({sq, to_sq, empty, empty,0});
    }

    // double push from the starting rank
    if (rank == start_rank){
        int to_sq = sq_of(rank + 2*dir, file);
        if (b.squares[to_sq]== empty){
            moves.push_back({sq, to_sq, empty, empty,0});
        }
    }
}


// handle diagnol moves including en passant
for (int df = -1; df <=1; df+=2){
    int f2 = df + file;
    if(f2<0||f2>7)continue;
    int cap_sq = sq_of( rank +dir, f2);
    int target = b.squares[cap_sq];
    if (target!= empty && piece_color(target) != color){
        if (rank_of(cap_sq) == promo_rank)
        {

          int base = (color == white) ? wN :bN ;
        for (int promo = base; promo <= base+3 ; promo ++)
        {
            moves.push_back({sq, to_sq, promo, empty, 3});
        }
  
        }
        else{
            moves.push_back({sq, cap_sq, target, empty, 0});
        }
        if (cap_sq == b.ep_square_number){
            int ep_captured = (color == white)? wP:bP;
            moves.push_back({sq, to_sq, ep_captured, empty, 2});

        }
    }

}
}
}
static void add_knight_moves(const Board& b, std::vector<Move>& moves){
    int color = b.side_to_move;
    int knight = (color ==white) ? wN: bN;
    // all 8 possible knight moves
    int knight_offsets[8][2]={{-2,1},{-2,-1}, {-1,2}, {1,-2},{-1,-2},{1,2},{2,-1},{2,1}};
    for (int sq =0 ; sq< 64; sq++){
        if(b.squares[sq]!= knight)continue;
        int rank= rank_of(sq), file = file_of(sq);
         for (auto& off: knight_offsets){
            int r = rank + off[0], f = file + off[1];
            if (r<0 || r> 7 || f<0 || f> 7) continue;
            int to_sq = sq_of(r,f);
            int target = b.squares[to_sq];
            // can move to empty squares or squares or squares with enemy
            if (target == empty || piece_color(target)!= color){
                moves.push_back({sq,to_sq,target, empty, 0});
            }
         }
    }
}
static void add_sliding_moves(const Board& b, std::vector <Move>& moves, 
    int piece, const int dirs[][2], int num_dirs)
{

    int color = b.side_to_move;
    for (int sq= 0; sq < 64; sq++){
        if (b.squares[sq]!= piece) continue;
        int rank = rank_of(sq), file = file_of(sq);
        // cast a ray in each one of the directions
        for (int d =0; d < num_dirs; d++)
{
    int r = rank + dirs[d][0], f = file + dirs [d][1];
    while (r >= 0 && r < 8 && f >= 0 && f < 8){
        int to_sq = sq_of(r,f);
        int target = b.squares[to_sq];
        if (target== empty){
            moves.push_back({sq, to_sq, empty, empty, 0});

        }
        else{
            // capture
            if (piece_color(target)!= color){
                moves.push_back({sq,to_sq, target,empty,0});
            }
            break;
        }
        r+=dirs[d][0]; f+=dirs[d][1];

}
    }
}
}

// king moves like a queen but only 1 square it also castles.
static void add_king_moves(const Board& b, std:: vector<Move>& moves)
{
int color = b.side_to_move;
int king_sq = find_king(b, color);
if (king_sq == -1) return;
int rank = rank_of(king_sq), file = file_of(king_sq);
for ( int dr =-1; dr <=1; dr++){
    for (int df= -1; df<=1; df++){
        if (dr==0 && df== 0)continue;
        int r = rank +dr,  f = file + df;
        if (r < 0 || r > 7 || f < 0 || f < 7)continue;
        int to_sq= sq_of(r,f);
        int target = b.squares[to_sq];
        if (target==empty || piece_color(target)!= color){
            moves.push_back({king_sq, to_sq, target, empty,0});
        }
    }
}
int base_rank = (color== white) ? 0:7;
int opponent = 1- color;
if (!is_sq_attacked(b,king_sq,opponent)){
    //  castle kingside
    int ks_flag= (color == white)? castle_wk : castle_bk;
    if(b.castling_rights & ks_flag){
        int f5 = sq_of(base_rank,5), g1 = sq_of(base_rank,6);
        if (b.squares[f5] == empty && b.squares[g1] == empty
        && 
    !is_sq_attacked(b,f5, opponent)&&
    !is_sq_attacked(b,g1, opponent)){
        moves.push_back({king_sq,g1,empty,empty,1});
    }
    }
    // queenside castling
    int qs_flag = (color == white)?castle_wq:castle_bq;
    if (b.castling_rights& qs_flag){
        int d1 = sq_of(base_rank,3), c1 = sq_of(base_rank,2), b1 = sq_of(base_rank,1);
        if (b.squares[d1]==empty && b.squares[c1]==empty && b.squares[b1]==empty&&
    !is_sq_attacked(b,d1, opponent)&&
    !is_sq_attacked(b,c1, opponent)){
        moves.push_back({king_sq,c1,empty,empty,1});
    }
    }
}
}
std::vector<Move> generate_pseudo_legal_moves(const Board& b){
    std::vector<Move> moves;
    // this avoids repeated memory allocations.
    moves.reserve(256); // 256 slots are reserved here
    int color = b.side_to_move;
    
    //  generate moves for each piece type
    add_pawn_moves(b,moves);
    add_knight_moves(b,moves);
    
    // direction tables for sliding pieces 
    int bishop_dirs[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
    int rook_dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    int all_dirs[8][2] = {{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}};
    
    int bishop = (color == white) ? wB : bB;
    int rook = (color == white) ? wR : bR;
    int queen = (color == white) ? wQ : bQ;
    add_sliding_moves(b,moves,bishop,bishop_dirs,4);
    add_sliding_moves(b,moves,rook,rook_dirs,4);
    add_sliding_moves(b,moves,queen,all_dirs,8);
    add_king_moves(b,moves);
    return moves;
}

std::vector<Move> generate_legal_moves(const Board& b) {
    auto pseudo_moves = generate_pseudo_legal_moves(b);
    std::vector<Move> legal;
    legal.reserve(pseudo_moves.size()); // set capacity using reserve from std
// filter, keep moves where the king is not in check.
for(const auto& m:pseudo_moves){
    Board copy= b;
    make_move(copy,m);
    if (!is_in_check(copy, b.side_to_move)){
        legal.push_back(m);
    }
}
return legal;
}
// The generateLegalMoves function takes each pseudo-legal move, copies the board, 
// applies the move, and checks if the moving side's king is in check. 
// If it is, that move is discarded. 
// This is the copy-make approach. It is simpler than make/unmake and fast enough for depth 4-5 search.


// The generatePseudoLegalMoves function calls all four helpers and collects every possible move. 
// It reserves 256 slots upfront to avoid repeated memory allocations.