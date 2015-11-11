#include <iostream>
#include "chess_board.h"
// This file just initializes the game and contains a num parser to handle everything


using namespace std;

int fib(int n);

int main() {
	for (auto i = 1; i < 10; i++) {
		cout << fib(i) << endl;
	}
	ChessBoard* myboard = new ChessBoard();
	myboard->printBoardState();
	return 0;
}

int fib(int n) {
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	
	return fib(n-1) + fib(n-2);
}