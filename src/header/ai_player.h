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

    std::pair<int, Move> evaluate_node(ChessBoard* board, int depth, int alpha, int beta);
    int evaluate_board(ChessBoard* board);
};
