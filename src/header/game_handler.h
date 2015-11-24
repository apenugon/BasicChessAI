#include "human_player.h"
#include "ai_player.h"

class GameHandler {
public:
	GameHandler(bool isHumanP1, bool isHumanP2);
	~GameHandler();

	void play_game();
private:
	Player* One;
	Player* Two;
	ChessBoard* chess_board;
};
