#include "game_handler.h"

GameHandler::GameHandler(bool isHumanP1, bool isHumanP2) {
	if (isHumanP1)
		this->One = new HumanPlayer("WHITE");
	if (isHumanP2)
		this->Two = new AIPlayer("BLACK");
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
    std::cin.ignore();
    std::cout << std::endl;
	while (!chess_board->is_game_over()) {
		chess_board->printBoardState();
		auto move = current_player->get_move(chess_board);
		ChessBoard* next_board = chess_board->makeMove(move);
		#ifdef DEBUG
		chess_board->check_integrity();
		#endif
		if (next_board == NULL) {
			std::cout << "Invalid Move - type MOVES to see all possible moves" << std::endl;
		} else {
			delete chess_board;
			chess_board = next_board;
			current_player = current_player == One ? Two : One;
			#ifdef DEBUG
			chess_board->check_integrity();
			#endif
		}
	}
}
