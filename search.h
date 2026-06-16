#ifndef SEARCH_H
#define SEARCH_H
#include "types.h"
// stores the result of a search: best move, its score and how many positions were evaluated

struct search_result{
    Move best_move;
    int score;
    int nodes_searched;
};
search_result find_best_move(const Board& b, int depth);
#endif
