#include "search.h"
#include "movegen.h"
#include "eval.h"
#include "board.h"
# include <algorithm>
#include<climits>

// global counter for nodes searched (reset before each search)
static int nodes_count = 0;

// MVV-LVA scoring: prioritize capturing high-value pieces with low-value attackers
static const int victim_score[13] = {0, 100, 200, 200, 300, 400, 500, 100, 200, 200, 300, 400, 500};
static const int attacker_score[13] = {0, 6, 5, 5, 4, 3, 2, 6, 5, 5, 4, 3, 2};


static int score_move(const Move& m, const Board& b) {

    if(m.captured != empty){ 
        return ( victim_score[m.captured] *10 + 6 - attacker_score[b.squares[m.from]]);
    }
    return 0;
}

static void order_moves(std::vector<Move>& moves, const Board& b)
{
    std::sort(moves.begin(), moves.end(), [&b](const Move& a, const Move& b2){
        return score_move(a,b) > score_move(b2,b);
    });


}
// negamax with alpha-beta pruning
// score is always from the perspective of the side to move
static int negamax(Board& b, int depth, int alpha, int beta){
    nodes_count++;
    //  base case: evaluate this position statistically
    if (depth ==0) return evaluate(b);
    auto moves = generate_legal_moves(b);
    // no legal moves means checkmate or stalemate
    if(moves.empty()){
        if(is_in_check(b, b.side_to_move))return -100000 + (100 - depth);
        return 0;
    }
    order_moves(moves, b);
    for (const auto& m : moves){
        Board copy = b;
        make_move(copy,m);
        // negate the score (opponenets best is our worst)
        int score = -negamax(copy, depth-1,-beta,-alpha);
        // beta cutoff, opponent will never allow this position
        if (score >= beta) return beta;
        //  improve alpha if this move is better than our current best
        if (score> alpha ) alpha = score;

    }
    return alpha;
}
search_result find_best_move(const Board& b, int depth){
    nodes_count = 0;
    auto moves = generate_legal_moves(b);
    search_result result;
    result.score = -INT_MAX;
    result.best_move = moves.empty() ? Move{0,0,0,0,0} : moves[0];
        order_moves(moves, b);
    int alpha = -INT_MAX;
    int beta = INT_MAX;
    // try each legal move and keep the one with the highest score
    for (const auto&m : moves){
        Board copy = b;
        make_move(copy,m);
        int score = -negamax(copy, depth -1, -beta, -alpha);

        if (score > result.score){result.score = score;
        result.best_move = m; }
        if (score > alpha ) alpha = score;
    }
    result.nodes_searched = nodes_count;
    return result;


}