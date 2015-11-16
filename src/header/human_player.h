#include <sstream>
#include "player.h"

class HumanPlayer: public Player {
public:
	HumanPlayer(std::string team);
	~HumanPlayer();

	std::tuple<ChessBoard::MoveType, std::string, std::string> get_move(ChessBoard* board);
private:
	std::string name;
};