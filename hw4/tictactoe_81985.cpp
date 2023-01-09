#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

class Board {
    private:
        int* board;

        int checkWinRows() const {
            int countX = 0, countY = 0;
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    if(this -> board[3 * i + j] == 1) {
                        countX++;
                    }
                    if(this -> board[3 * i + j] == -1) {
                        countY++;
                    }
                }
                if (countX == 3) {
                    return 1;
                }
                if (countY == 3) {
                    return -1;
                }
                countX = countY = 0;
            }    
            return 0;
        }

        int checkWinMainDiagonal() const {
            int countX = 0, countY = 0;
            for(int i = 0; i < 3; i++) {
                if(this -> board[3 * i + i] == 1) {
                    countX++;
                }
                if(this -> board[3 * i + i] == -1) {
                    countY++;
                }
            }    
            if (countX == 3) {
                return 1;
            }
            if (countY == 3) {
                return -1;
            }
            return 0;
        }

        int checkWinSecondDiagonal() const {
            int countX = 0, countY = 0;
            for(int i = 0, j = 2; i < 3; i++, j--) {
                if(this -> board[3 * i + j] == 1) {
                    countX++;
                }
                if(this -> board[3 * i + j] == -1) {
                    countY++;
                }
            }    
            if (countX == 3) {
                return 1;
            }
            if (countY == 3) {
                return -1;
            }
            return 0;
        }
        
        int checkWinColumns() const {
            int countX = 0, countY = 0;
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    if(this -> board[i + 3 * j] == 1) {
                        countX++;
                    }
                    if(this -> board[i + 3 * j] == -1) {
                        countY++;
                    }
                }
                if (countX == 3) {
                    return 1;
                }
                if (countY == 3) {
                    return -1;
                }
                countX = countY = 0;
            }    
            return 0;
        }

        int countPlaced() const {
            int counter = 0;
            for (int i = 0; i < 9; i++) {
                if (this -> board[i] != 0) {
                    counter++;
                }
            }
            return counter;
        }

    public:
        Board() {
            this -> board = new int[9];
            for (int i = 0; i < 9; i++) {
                this -> board[i] = 0;
            }
        }

        Board(int* board) {
            this -> board = new int[9];
            for (int i = 0; i < 9; i++) {
                this -> board[i] = board[i];
            }
        }

        Board(const Board& other) {
            this -> board = new int[9];
            for (int i = 0; i < 9; i++) {
                this -> board[i] = other.board[i];
            }
        }

        ~Board() {
            delete[] this -> board;
        }

        int turn() const {
            return (this -> countPlaced() % 2 == 0 ? 1 : -1); 
        }

        int hasMoreTurns() const {
            return (this -> countPlaced() < 9);
        }

        bool isPlaced(int pos) const {
            return this -> board[pos] != 0;
        } 

        Board move(int pos) const {
            int* new_board = new int[9];
            for (int i = 0; i < 9; i++) {
                new_board[i] = this -> board[i];
            }

            if (new_board[pos] == 0) {
                new_board[pos] = this -> turn();
            }
            Board b(new_board);

            delete[] new_board;
            return b;
        }

        std::vector<Board> allPossibleMoves() const {
            std::vector<Board> moves;
            for (int i = 0; i < 9; i++) {
                if(this -> board[i] == 0) {
                    moves.push_back(this -> move(i));
                }
            }

            return moves;
        }

        int calculateUtility () const {
            int row = checkWinRows();
            int col = checkWinColumns();
            int main_diag = checkWinMainDiagonal();
            int sec_diag = checkWinSecondDiagonal();

            if (row + col + main_diag + sec_diag > 0) {
                return 1;
            }
            else if (row + col + main_diag + sec_diag < 0) {
                return -1;
            }
            else return 0;
        }


        Board& operator= (const Board& other) {
            for (int i = 0; i < 9; i++) {
                this -> board[i] = other.board[i];
            }
            return *this;
        }

        friend std::ostream& operator<< (std::ostream& stream, const Board& board) {
            for (int i = 0; i < 9; i++) {
                if (i % 3 == 0) stream << "\n";
                stream << (board.board[i] == 0 ? " " : board.board[i] == 1 ? "X" : "O") << " ";
            }

            return stream;
        }
};

int minimax(Board current) {
    if (!current.hasMoreTurns() || current.calculateUtility() != 0) {
        return current.calculateUtility();
    }
    std::vector<Board> children = current.allPossibleMoves();
    int val = minimax(children[0]);
    for (int i = 1; i < children.size(); i++) {
        if (current.turn() == 1) {
            val = std::max(val, minimax(children[i]));
        }
        else {
            val = std::min(val, minimax(children[i]));
        }
    }

    return val;
}

int alphabeta(Board current, int alpha, int beta) { //start with alpha = -1, beta = 1
    if (!current.hasMoreTurns() || current.calculateUtility() != 0) {
        return current.calculateUtility();
    }
    int val = current.turn() == 1 ? -1 : 1;
    for (Board child : current.allPossibleMoves()) {
        if (current.turn() == 1) {
            alpha = alphabeta(child, alpha, beta);
            if (alpha < val) {
                break;
            }
            val = alpha;
        }
        else {
            beta = alphabeta(child, alpha, beta);
            if (beta > val) {
                break;
            }
            val = beta;
        }
    }

    return val;
}

Board alphabeta_nextmove(Board current) {
    int alpha = -1, beta = 1;
    int val = current.turn() == 1 ? -1 : 1;
    Board next;

    for (Board child : current.allPossibleMoves()) {
        if (current.turn() == 1) {
            alpha = alphabeta(child, alpha, beta);
            if (alpha < val) {
                break;
            }
            val = alpha;
            next = child;
        }
        else {
            beta = alphabeta(child, alpha, beta);
            if (beta > val) {
                break;
            }
            val = beta;
            next = child;
        }
    }

    return next;
}


void test() {
    int* board = new int[9];

    board[0] = 0;
    board[1] = 0;
    board[2] = 1;
    board[3] = 0;
    board[4] = 0;
    board[5] = 0;
    board[6] = 0;
    board[7] = 0;
    board[8] = 0;

    Board current(board);
    std::cout << current << std::endl;

    auto start = std::chrono::steady_clock::now();
    int util = alphabeta(current, -1, 1);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nAlpha beta time: " << elapsed.count() << "\n";
    std::cout << (util == 0 ? "Draw" : util < 0 ? "O Wins" : "X Wins") << std::endl;

    auto start1 = std::chrono::steady_clock::now();
    int util1 = minimax(current);
    auto end1 = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::cout << "\nMinimax time: " << elapsed1.count() << "\n";
    std::cout << (util1 == 0 ? "Draw" : util1 < 0 ? "O Wins" : "X Wins") << std::endl;

    delete[] board;
}

void game() {
    Board current;
    while (current.calculateUtility() == 0 && current.hasMoreTurns()) {
        int pos_x, pos_y;
        bool invalid_move = true;

        std::cout << (current.turn() == 1 ? "X" : "O") << " (1 - 3) (1 - 3): ";
        std::cin >> pos_x >> pos_y;
        if ((pos_x < 1 || pos_x > 3) || (pos_y < 1 || pos_y > 3) || current.isPlaced(3 * (pos_x - 1) + (pos_y - 1))) {
            continue;
        }
        current = current.move(3 * (pos_x - 1) + (pos_y - 1));
        
        if (current.hasMoreTurns()) {
            current = alphabeta_nextmove(current);
        }

        std::cout << current << std::endl;
    }

    int util = current.calculateUtility();
    std::cout << std::endl << (util == 0 ? "Draw" : util < 0 ? "O Wins" : "X Wins") << std::endl;

}

int main () {
    //test();
    game();
    return 0;
}
