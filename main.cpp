#include "types.h"
#include "board.h"
#include "movegen.h"
#include "eval.h"
#include "search.h"
#include <iostream>
#include<string>
#include <chrono>

int main ()
{
    Board board;
    initialize_board(board);
    std::cout<< "=== Chess Engine ===\n";
    std::cout<< "enter moves as e2e4 ( from square towards to square \n";
    std::cout<< "Promotions: e78w (append piece: q/r/b/n) \n";
    std::cout<< "type 'quit' to exit.\n\n";

    const int engine_depth =4;
    while (true){
    print_board(board);
    auto legal_moves = generate_legal_moves(board);

    if (legal_moves.empty()){

        if ( is_in_check(board, board.side_to_move)){
            std::cout << (board.side_to_move == white ? "black":"white")<<"wins by checkmate!\n";
        } else {
            std::cout<< "stalemate! its a draw.\n";
        }
        break;
    }
    if (board.side_to_move==white){
        std::cout<<"your move: ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "quit") break;

            if (input.length() < 4) {
                std::cout << "Invalid input. Use format: e2e4\n";
                continue;
    }

    int from = algebaraic_to_sq(input.substr(0, 2));
    int to = algebaraic_to_sq(input.substr(2, 2));

    if (from == -1 || to == -1) {
                std::cout << "Invalid square.\n";
                continue;
            }
            //  find matching legal move

    Move player_move{0,0,0,0,0};
    bool found = false;
    for (const auto& m: legal_moves){
        if(m.from == from && m.to == to){
            if (m.flags ==3){// promote
                char promo_char = (input.length()>4)?input[4]:'q';
                int promo_base = wN; // white pieces
                int promo_piece = wQ;
                switch(promo_char){
                    case 'n': promo_piece = promo_base;     break;
                    case 'b': promo_piece = promo_base + 1; break;
                    case 'r': promo_piece = promo_base + 2; break;
                    case 'q': promo_piece = promo_base + 3; break;
                }
                if (m.promoted == promo_piece){
                    player_move = m;
                    found = true;
                    break;
                }
                }else{
                    player_move = m;
                    found = true;
                    break;
                }
            }
        }
        if(!found){
            std::cout<< "illegal move, please try again. \n";
            continue;
        }
        make_move(board,player_move);
    } else {
        std::cout << " engine thinking (depth "<< engine_depth<<")...\n";
        // measure how long the seach takes
        auto start = std::chrono::high_resolution_clock::now();
        auto result = find_best_move(board,4);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
        std::cout<< "engine plays : " << sq_to_algebraic(result.best_move.from) << "to" << sq_to_algebraic(result.best_move.to)<<"\n";
        if (result.best_move.flags==3){
        int pt = piece_type(result.best_move.promoted);
        const char promo_chars[]= "pnbrqk";
        std::cout << promo_chars[pt];
        }
        std::cout << "(score : " << result.score
                  << ", nodes: " << result.nodes_searched
                  << ", time: " << ms << "ms) \n\n";
                  make_move(board, result.best_move);
            
    }
}
    return 0;
}
