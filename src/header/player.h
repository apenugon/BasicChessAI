#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <algorithm>
#include <string>
#include "move.h"
#include "chess_board.h"

class Player {
public:
	virtual Move get_move(ChessBoard* board) = 0;
	virtual ~Player() {

	}
};

#endif
