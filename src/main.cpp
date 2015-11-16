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
	//ChessBoard* nextboard = myboard->makeMove(ChessBoard::MOVE, std::make_pair(4, 3), std::make_pair(4, 4));
	//nextboard->printBoardState();
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