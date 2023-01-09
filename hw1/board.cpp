#include "board.hpp"

#include <iostream>
#include <vector>
#include <cmath>

int Board::countInversions() const
{
    int inversionsCount = 0;

    for (int i = 0; i < this -> size * this -> size; i++)
    {
        for (int j = i + 1; j < this -> size * this -> size; j++)
        {
            if (this -> board[i] > this -> board[j])
            {
                inversionsCount++;
            }
        }
    }

    return inversionsCount;
}

void Board::reset() {
    this -> size = 0;
    this -> moves = 0;
    this -> emptyIndex = 0;

    if (board) {
        delete[] board;
    }
}

Board::Board()
{
    this->size = 3;
    this->moves = 0;

    int value = 1;

    this->board = new int[this->size * this->size];
    for (int i = 0; i < this->size * this->size; i++)
    {
        if (i == 8)
        {
            this->board[i] = 0;
            this->emptyIndex = i;
        }
        else
        {
            this->board[i] = value++;
        }
    }
}

Board::Board(int* board, unsigned int size, int moves)
{
    this->size = size;
    this->moves = moves;

    this->board = new int[this -> size * this -> size];
    for (int i = 0; i < this -> size * this -> size; i++)
    {
        this->board[i] = board[i];
        if (this->board[i] == 0)
        {
            this->emptyIndex = i;
        }
    }

    delete[] board;
}

Board::Board(int* board, unsigned int size)
{
    this->size = size;
    this->moves = 0;

    this->board = new int[this->size * this->size];
    for (int i = 0; i < this->size * this->size; i++)
    {
        this->board[i] = board[i];
        if (this->board[i] == 0)
        {
            this->emptyIndex = i;
        }
    }

    delete[] board;
}

Board::Board(unsigned int size, int emptyIndex)
{
    this->size = size;
    this->moves = 0;

    int value = 1;
    this -> board = new int[this -> size * this -> size];
    for (int i = 0; i < this -> size * this -> size; i++)
    {
        if (i == emptyIndex)
        {
            this->board[i] = 0;
            this->emptyIndex = i;
        }
        else
        {
            this->board[i] = value++;
        }
    }
}

Board::Board(const Board &other)
{
    this->size = other.size;
    this->moves = other.moves;
    this->emptyIndex = other.emptyIndex;

    this->board = new int[this -> size * this -> size];
    for (int i = 0; i < this -> size * this -> size; i++)
    {
        this->board[i] = other.board[i];
    }
}

Board::~Board()
{
    this -> reset();
}

unsigned int Board::getSize() const
{
    return this->size;
}

int Board::getMoves() const
{
    return this->moves;
}

int* Board::getBoard() const {
    return this -> board;
}

bool Board::isSolvable() const
{
    return (this->size % 2 == 0 && (this -> countInversions() + int(this -> emptyIndex / this -> size) + 1) % 2 != 0) || (this->size % 2 != 0 && this->countInversions() % 2 == 0);
}

bool Board::canMove(int direction) const
{
    switch (direction)
    {
    case 0:
        return this->emptyIndex % this -> size != 0;
    case 1:
        return floor(this->emptyIndex / this -> size) != this -> size - 1;
    case 2:
        return this->emptyIndex % this -> size != this -> size - 1;
    case 3:
        return floor(this->emptyIndex / this -> size) != 0;
    default:
        return false;
    }
}

Board Board::move(int direction) const
{
    int *board = new int[this -> size * this -> size];
    for (int i = 0; i < this -> size * this -> size; i++)
    {
        board[i] = this -> board[i];
    }

    switch (direction)
    {
    case 0:
        std::swap(board[this -> emptyIndex], board[this -> emptyIndex - 1]);
        break;
    case 1:
        std::swap(board[this -> emptyIndex], board[this -> emptyIndex + this -> size]);
        break;
    case 2:
        std::swap(board[this -> emptyIndex], board[this -> emptyIndex + 1]);
        break;
    case 3:
        std::swap(board[this -> emptyIndex], board[this -> emptyIndex - this -> size]);
        break;
    }

    return Board(board, this -> size, this -> moves + 1);
}

int Board::calculateHeretic(Board& solution) const
{
    int h = 0;

    int* currentX = new int[this -> size * this -> size];
    int* currentY = new int[this -> size * this -> size];
    int* solutionX = new int[this -> size * this -> size];
    int* solutionY = new int[this -> size * this -> size];

    for (int i = 0; i < this->size * this->size; i++)
    {
        currentX[this -> board[i]] = int(i / this -> size);
        currentY[this -> board[i]] = i % this -> size;
        solutionX[solution.board[i]] = int(i / this -> size);
        solutionY[solution.board[i]] = i % this -> size;
    }

    for (int i = 1; i < this->size * this->size; i++)
    {
        h += fabs(solutionX[i] - currentX[i]) + fabs(solutionY[i] - currentY[i]);
    }

    delete[] currentX;
    delete[] currentY;
    delete[] solutionX;
    delete[] solutionY;

    return h;
}

bool Board::operator==(const Board& right) const
{
    if (this->size == right.size)
    {
        for (int i = 0; i < this -> size * this -> size; i++)
        {
            if (this -> board[i] != right.board[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void Board::operator= (const Board& other) {
    this -> reset();

    this->size = other.size;
    this->moves = other.moves;
    this->emptyIndex = other.emptyIndex;

    this->board = new int[this -> size * this -> size];
    for (int i = 0; i < this -> size * this -> size; i++)
    {
        this->board[i] = other.board[i];
    }   
}

std::ostream& operator<<(std::ostream& stream, const Board& board)
{
    for (int i = 0; i < board.size * board.size; i++)
    {
        if (i % board.size == 0) {
            stream << "\n";
        }
        stream << board.board[i] << " ";
    }
    return stream;
}