#include <sstream>
#include "player.h"

class HumanPlayer: public Player {
public:
	HumanPlayer(std::string team);
	~HumanPlayer();

	Move get_move(ChessBoard* board);
private:
	std::string name;
};
