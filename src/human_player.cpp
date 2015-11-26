#include "human_player.h"

HumanPlayer::HumanPlayer(std::string team) {
	std::cout << "Player with " << team << " pieces, what is your name?" << std::endl;
	std::cout << "Name: ";
	std::string name;
	std::cin >> name;
	if (name.length() > 0) {
		this->name = name;
	} else {
		this->name = "Player " + team;
	}
}

HumanPlayer::~HumanPlayer() {

}

Move HumanPlayer::get_move(ChessBoard* board) {

	while (1) {
		std::cout << name << "@ChessGame> ";
		std::string input;
		std::getline(std::cin, input);
		std::cout << std::endl;

        if (input.length() == 0)
            continue;

		std::stringstream stream(input);
		std::string buffer;
		std::vector<std::string> tokens;
		while (stream >> buffer) {
			tokens.push_back(buffer);
		}

		std::string command = tokens[0];

		if (command.compare("MOVE") == 0) {
            if (tokens.size() < 3 || tokens[1].length() != 2 ||
                    tokens[2].length() != 2) {
                std::cout << "Improper Coordinates Specified" << std:: endl;
                continue;
            }
			return Move(Move::MOVE, tokens[1], tokens[2]);
		}

		else if (command.compare("CASTLE") == 0) {
            if (tokens.size() < 3 || tokens[1].length() != 2 ||
                    tokens[2].length() != 2) {
                std::cout << "Improper Coordinates Specified" << std:: endl;
                continue;
            }
			return Move(Move::CASTLE, tokens[1], tokens[2]);
		}

        else if (command.compare("PROMOTE") == 0) {
            if (tokens.size() < 4 || tokens[1].length() != 2 ||
                    tokens[2].length() != 2 ||
                    tokens[3].length() != 1) {
                std::cout << "Improper coordinates/piece - see MOVES" << std::endl;
                continue;
            }
            return Move(Move::PROMOTE, tokens[1], tokens[2], tokens[3]);
        }

		else if (command.compare("MOVES") == 0) {
			board->print_available_moves();
		}

		else if (command.compare("PRINT") == 0) {
			board->printBoardState();
		}
        
        else if (command.compare("EXIT") == 0) {
            exit(0);
        }

		else if (command.compare("HELP") == 0) {
            std::cout << "CASTLE <coord1> <coord2> - Perform a castling move if possible," << std::endl;
            std::cout << "                         - where coord1 is the king and coord2 the rook" << std::endl;
			std::cout << "HELP - show this dialog" << std::endl;
			std::cout << "MOVES - prints a list of available moves" << std::endl;
			std::cout << "MOVE <coord1> <coord2> - prints a list of available moves" << std::endl;
			std::cout << "                         Ex: MOVE 4a 4b" << std::endl;
			std::cout << "PRINT - print board state" << std::endl;
            std::cout << "PROMOTE <coord1> <coord2> <piece> - Promote a pawn" << std::endl;
            std::cout << "                - Piece is one of [Q,R,B,N]" << std::endl;
			std::cout << "EXIT - Exit game" << std::endl;
		}

#ifdef DEBUG
        else if (command.compare("INTEG") == 0) {
            std::cout << "Performing integrity check..." << std::endl;
            board->check_integrity();
        }
#endif

		else {
			std::cout << "Not a valid move - type HELP for possible inputs" << std::endl;
		}

	}
}
