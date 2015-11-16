#include <tuple>
#include <algorithm>
#include <string>
#include "chess_board.h"

class Player {
public:
	virtual std::tuple<ChessBoard::MoveType, std::string, std::string> get_move(ChessBoard* board) = 0;
	virtual ~Player() {

	}
};