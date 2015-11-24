#include "player.h"
#include <sstream>
#include <limits.h>

class AIPlayer: public Player {
public:
    AIPlayer(std::string team);
    ~AIPlayer();

    std::tuple<ChessBoard::MoveType, std::string, std::string> get_move(ChessBoard* board);
private:
    std::map<Piece::Types, int> material_scores;
    std::string name;
    int team;

    std::pair<int, std::tuple<ChessBoard::MoveType, std::pair<int,int>, std::pair<int,int>>> evaluate_node(ChessBoard* board, int depth, int alpha, int beta);
    int evaluate_board(ChessBoard* board);
};
