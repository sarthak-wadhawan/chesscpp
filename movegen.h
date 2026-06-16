#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "types.h"

// generate all legal moves for the current side.
std::vector <Move>generate_legal_moves(const Board& b);
std::vector <Move>generate_pseudo_legal_moves(const Board& b);
#endif
