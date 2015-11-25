#include <iostream>
#include <string>
#include <omp.h>

#include "game_handler.h"
// This file just initializes the game and contains a num parser to handle everything


using namespace std;

int fib(int n);

int main() {
    omp_set_num_threads(omp_get_max_threads());
	GameHandler* myGameHandler;;
	while(1) {
		myGameHandler = new GameHandler(true, true);
		myGameHandler->play_game();
		delete myGameHandler;
        std::cout << "The last player won! New game? (y/n)";
        char d;
        std::cin >> d;
        if (d == 'n') {
            break;
        }
	}

    return 0;
}
