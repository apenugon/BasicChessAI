#include "player.h"
#include <sstream>
#include <limits.h>

class AIPlayer: public Player {
public:
    AIPlayer(std::string team);
    ~AIPlayer();

    Move get_move(ChessBoard* board);
private:
    std::map<Piece::Types, int> material_scores;
    std::string name;
    int team;

    std::pair<int, Move> evaluate_node(ChessBoard* board, int depth, int alpha, int beta, bool is_quiet);
    
    std::pair<int, Move> q_search(ChessBoard* board, int depth, int alpha, int beta, bool is_quiet);
    int evaluate_board(ChessBoard* board);
    bool is_quiet(Move move, ChessBoard* resulting_board);
};
