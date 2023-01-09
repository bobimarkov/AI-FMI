#ifndef BOARD_HPP
#define BOARD_HPP

#include <ostream>
#include <functional>

class Board {
    private:
        int *board;
        unsigned int size;
        int moves;
        int emptyIndex;

        int countInversions() const; 
        void reset(); 

    public:
        Board();
        Board(int* board, unsigned int size, int moves); 
        Board(int* board, unsigned int size); 
        Board(unsigned int size, int emptyIndex);
        Board(const Board& other); 
        ~Board(); 

        unsigned int getSize() const; 
        int getMoves() const; 
        int* getBoard() const;

        bool isSolvable() const; 
        bool canMove(int) const; 
        Board move(int) const; // 0 - right, 1 - up, 2 - left, 3 - down 
        int calculateHeretic(Board&) const; 

        bool operator== (const Board&) const; 
        void operator= (const Board&); 

        friend std::ostream& operator<< (std::ostream&, const Board&);
};

#endif