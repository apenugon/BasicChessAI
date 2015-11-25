#include <iostream>
#include <string>
#include <omp.h>

#include "game_handler.h"
// This file just initializes the game and contains a num parser to handle everything


using namespace std;

int fib(int n);

int main(int argc, char* args[]) {
    bool is_player1_human = true;
    bool is_player2_human = true;
    std::cout << argc << std::endl;
    if (argc == 3) {
        is_player1_human = args[1][0] == 'h';
        is_player2_human = args[2][0] == 'h';
    }
    omp_set_num_threads(omp_get_max_threads());
	GameHandler* myGameHandler;;
	while(1) {
		myGameHandler = new GameHandler(is_player1_human, is_player2_human);
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
