#include <iostream>
#include <vector>
#include <chrono>
#include <stack>
#include <unordered_map>
#include <utility>
#include <thread>
#include <stdlib.h>
#include "board.hpp"

using namespace std::chrono_literals;

bool DFS_rec(Board current, std::vector<int> visited, std::vector<int> &path, int direction, int threshold, Board target) 
{
    visited.push_back(direction);
    if (current == target)
    {
        path = visited;
        return true;
    }
    if (current.getMoves() + current.calculateHeretic(target) > threshold)
    {
        return false;
    }
    bool result = false;
    for (int i = 0; i <= 3; i++)
    {
        if (current.canMove(i))
        {
            result = result || DFS_rec(current.move(i), visited, path, i, threshold, target);
        }
    }
    return result;
}

std::vector<int> idaStar(Board initial, Board target)
{
    int i = initial.calculateHeretic(target);
    std::vector<int> path;
    while (true)
    {
        if (DFS_rec(initial, {}, path, -1, i, target)) return path;
        i++;
    }

    return {};
}

int main()
{
    int size = 3, emptyTile = 0;
    // std::cin >> size >> emptyTile;

    if (emptyTile == -1)
        emptyTile = size * size - 1;

    int *board = new int[size * size];

    // for (int i = 0; i < size * size; i++)
    // {
    //      std::cin >> board[i];
    // }

    board[0] = 7;
    board[1] = 2;
    board[2] = 4;
    board[3] = 5;
    board[4] = 0;
    board[5] = 6;
    board[6] = 8;
    board[7] = 3;
    board[8] = 1;

    // board[0] = 2;
    // board[1] = 7;
    // board[2] = 10;
    // board[3] = 3;
    // board[4] = 1;
    // board[5] = 6;
    // board[6] = 5;
    // board[7] = 8;
    // board[8] = 9;
    // board[9] = 11;
    // board[10] = 4;
    // board[11] = 12;
    // board[12] = 13;
    // board[13] = 0;
    // board[14] = 14;
    // board[15] = 15;

    // board[0] = 1;
    // board[1] = 2;
    // board[2] = 3;
    // board[3] = 4;
    // board[4] = 5;
    // board[5] = 6;
    // board[6] = 0;
    // board[7] = 8;
    // board[8] = 9;
    // board[9] = 10;
    // board[10] = 7;
    // board[11] = 11;
    // board[12] = 13;
    // board[13] = 14;
    // board[14] = 15;
    // board[15] = 12;

    Board initial(board, size), target(size, emptyTile);
    //std::cout << initial.move(0) << "\n" << initial.move(1) << "\n" << initial.move(2) << "\n" << initial.move(3) << std::endl;
    if (!initial.isSolvable())
    {
        std::cout << "This puzzle is not solvable!\n";
        return 0;
    }


    auto start = std::chrono::steady_clock::now();

    std::vector<int> path = idaStar(initial, target);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    std::cout << initial << std::endl;

    std::cout << "Result size: " << path.size() - 1 << std::endl;

    for (int i : path)
    {
        switch (i)
        {
        case 0:
            std::cout << "right\n";
            break;

        case 1:
            std::cout << "up\n";
            break;

        case 2:
            std::cout << "left\n";
            break;

        case 3:
            std::cout << "down\n";
            break;
        default:
            break;
        }
    }

    return 0;
}