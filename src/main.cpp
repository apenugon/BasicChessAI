#include <iostream>
#include <string>

#include "chess_board.h"
// This file just initializes the game and contains a num parser to handle everything


using namespace std;

int fib(int n);

int main() {
	for (auto i = 1; i < 10; i++) {
		cout << fib(i) << endl;
	}
	ChessBoard* myboard = new ChessBoard();
	//myboard->printBoardState();
	cout << "Generated first states" << endl;
	cout << "Let's make a move!" << endl;
	myboard->printBoardState();
	myboard->print_available_moves();
	myboard->check_integrity();
	ChessBoard* nextboard = myboard->makeMove(ChessBoard::MOVE, std::make_pair(3, 4), std::make_pair(4, 4));
	//Should now be Black's turn
	nextboard->check_integrity();
	nextboard->printBoardState();
	nextboard->print_available_moves();
	ChessBoard* blackMove = nextboard->makeMove(ChessBoard::MOVE, std::make_pair(8, 5), std::make_pair(6, 5));
	blackMove->printBoardState();
	blackMove->print_available_moves();
	auto WhiteMove = blackMove->makeMove(ChessBoard::MOVE, "2a", "3a");
	WhiteMove->check_integrity();
	WhiteMove->printBoardState();
	WhiteMove->print_available_moves();
	delete blackMove;
	delete nextboard;
	delete myboard;
	//delete nextboard;
	return 0;
}

int fib(int n) {
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	
	return fib(n-1) + fib(n-2);
}