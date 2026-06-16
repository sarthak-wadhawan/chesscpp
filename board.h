# ifndef board_h
#define board_h

#include "types.h"

void initialize_board(Board&b);
void print_board(const Board& b);
std:: string sq_to_algebraic(int sq);
int algebaraic_to_sq(const std:: string& s );
void make_move(Board&, const Move& m);
bool is_sq_attacked(const Board& b, int sq, int by_color);
bool is_in_check(const Board& b, int color);
int find_king(const Board& b, int color);




#endif