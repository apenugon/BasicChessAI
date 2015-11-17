#include <iostream>
#include <string>

#include "game_handler.h"
// This file just initializes the game and contains a num parser to handle everything


using namespace std;

int fib(int n);

int main() {
    
	GameHandler* myGameHandler;;
	while(1) {
		myGameHandler = new GameHandler(true, true);
		myGameHandler->play_game();
		delete myGameHandler;
	}

    return 0;
}
