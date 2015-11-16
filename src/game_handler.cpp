#include "game_handler.h"

GameHandler::GameHandler(bool isHumanP1, bool isHumanP2) {
	if (isHumanP1)
		this->One = new HumanPlayer("white");
	if (isHumanP2)
		this->Two = new HumanPlayer("black");
	this->chess_board = new ChessBoard();
}

GameHandler::~GameHandler() {
	delete One;
	delete Two;
	delete chess_board;
}

void GameHandler::play_game() {
	Player* current_player = One; //Start with white
	// should be while game not over
	while (!chess_board->is_game_over()) {
		chess_board->printBoardState();
		auto move = current_player->get_move(chess_board);
		ChessBoard* next_board = chess_board->makeMove(move);
		if (next_board == NULL) {
			std::cout << "Invalid Move" << std::endl;
		} else {
			delete chess_board;
			chess_board = next_board;
			current_player = current_player == One ? Two : One;
		}
	}
}