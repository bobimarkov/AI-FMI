#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <vector>
#include <chrono>

void print_board(int* cols, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j != cols[i]) {
                std::cout << "- ";
            }
            else {
                std::cout << "X ";
            }
        }
        std::cout << std::endl; 
    }
}

int* init_ready(int N) {
    int* cols = new int[N];

    int first = floor(N/2);
    if (N%6 == 2 || N%6 == 3) {
        first = floor((N+2)/2);
    }
    int second = 0;
    for (int i = 0; i < N; i++) {
        if(i % 2 == 0) {
            cols[i] = first++;
        }
        else {
            cols[i] = second++;
        }
    }
    return cols;
}

int* init(int N) {
    bool* placed = new bool[N];
    for (int i = 0; i < N; i++) {
        placed[i] = false;
    }

    int* board = new int[N];

    for (int i = 0; i < N; i++) {
        int num = rand() % N;
        if (placed[num]) {
            i--;
            continue;
        }
        board[i] = num;
        placed[num] = true;
    }

    delete[] placed;
    return board;
}

int* qPR(int* cols, int N) {
    int* conflicts = new int[N];
    for(int i = 0; i < N; i++) {
        conflicts[i] = 0;
    }
    for(int i = 0; i < N; i++) {
        conflicts[cols[i]]++;
    }
    return conflicts;
}

int* qPD1(int* cols, int N) {
    int* conflicts = new int[N*2-1];
    for(int i = 0; i < N*2-1; i++) {
        conflicts[i] = 0;
    }
    for(int i = 0; i < N; i++) {
        conflicts[i-cols[i]+N-1]++;
    }
    return conflicts;
}

int* qPD2(int* cols, int N) {
    int* conflicts = new int[N*2-1];
    for(int i = 0; i < N*2-1; i++) {
        conflicts[i] = 0;
    }
    for(int i = 0; i < N; i++) {
        conflicts[i+cols[i]]++;
    }
    return conflicts;
}

int* min_conflicts(int N) {
    int* cols = init(N);
    bool hasNotConflicts = false;

    for (int i = 0; i <= 100*N; i++) {
        int* queensPerRow = qPR(cols, N);
        int* queensPerD1 = qPD1(cols, N);
        int* queensPerD2 = qPD2(cols, N);

        int totalConflicts = 0;
        for(int j = 0; j < N; j++) {
            int currentConflicts = queensPerRow[cols[j]] + queensPerD1[j - cols[j] + N - 1] + queensPerD2[j + cols[j]] - 3;
            totalConflicts += currentConflicts;
        }
        hasNotConflicts = totalConflicts == 0;

        if (hasNotConflicts) {
            delete[] queensPerRow;
            delete[] queensPerD1;
            delete[] queensPerD2;
            break;
        }

        int maxConflict = 0, minConflict = N;
        std::vector<int> maxConflictColIndexes, minConflictRowIndexes;

        for(int j = 0; j < N; j++) {
            int currentConflicts = queensPerRow[cols[j]] + queensPerD1[j - cols[j] + N - 1] + queensPerD2[j + cols[j]] - 3;
            if (currentConflicts > maxConflict) {
                maxConflict = currentConflicts;
                maxConflictColIndexes.clear();
            }
            if (currentConflicts == maxConflict) {
                maxConflictColIndexes.push_back(j);
            }
        }

        int maxConfColIndex = maxConflictColIndexes[rand() % maxConflictColIndexes.size()];

        for(int j = 0; j < N; j++) {
            int currentRowConflicts = queensPerRow[j] + queensPerD1[maxConfColIndex - j + N - 1] + queensPerD2[maxConfColIndex + j] - 3;
            if (minConflict > currentRowConflicts) {
                minConflict = currentRowConflicts;
                minConflictRowIndexes.clear();
            }
            if (minConflict == currentRowConflicts) {
                minConflictRowIndexes.push_back(j);
            }
        }

        cols[maxConfColIndex] = minConflictRowIndexes[rand() % minConflictRowIndexes.size()];

        delete[] queensPerRow;
        delete[] queensPerD1;
        delete[] queensPerD2;

        if (hasNotConflicts) {
            break;
        }
    }

    if(hasNotConflicts) {
        return cols;
    }
    delete[] cols;
    return min_conflicts(N);
}

int* max_conflicts(int N) {
    int* cols = init(N);
    bool hasMaxConflicts = false;

    for (int i = 0; i <= 10000*N; i++) {
        int* queensPerRow = qPR(cols, N);
        int* queensPerD1 = qPD1(cols, N);
        int* queensPerD2 = qPD2(cols, N);

        int totalConflicts = 0;
        for(int j = 0; j < N; j++) {
            int currentConflicts = queensPerRow[cols[j]] + queensPerD1[j - cols[j] + N - 1] + queensPerD2[j + cols[j]] - 3;
            totalConflicts += currentConflicts;
        }

        hasMaxConflicts = totalConflicts == (N - 1)*N;

        if (hasMaxConflicts) {
            delete[] queensPerRow;
            delete[] queensPerD1;
            delete[] queensPerD2;
            break;
        }


        int minConflict = N, maxConflict = 0;
        std::vector<int> minConflictColIndexes, maxConflictRowIndexes;

        for(int j = 0; j < N; j++) {
            int currentConflicts = queensPerRow[cols[j]] + queensPerD1[j - cols[j] + N - 1] + queensPerD2[j + cols[j]] - 3;
            if (currentConflicts < minConflict) {
                minConflict = currentConflicts;
                minConflictColIndexes.clear();
            }
            if (currentConflicts == minConflict) {
                minConflictColIndexes.push_back(j);
            }
        }

        int minConfColIndex = minConflictColIndexes[rand() % minConflictColIndexes.size()];

        for(int j = 0; j < N; j++) {
            int currentRowConflicts = queensPerRow[j] + queensPerD1[minConfColIndex - j + N - 1] + queensPerD2[minConfColIndex + j] - 3;
            if (maxConflict < currentRowConflicts) {
                maxConflict = currentRowConflicts;
                maxConflictRowIndexes.clear();
            }
            if (maxConflict == currentRowConflicts) {
                maxConflictRowIndexes.push_back(j);
            }
        }

        cols[minConfColIndex] = maxConflictRowIndexes[rand() % maxConflictRowIndexes.size()];

        delete[] queensPerRow;
        delete[] queensPerD1;
        delete[] queensPerD2;

        if (hasMaxConflicts) {
            break;
        }
    }

    if(hasMaxConflicts) {
        return cols;
    }
    delete[] cols;
    return max_conflicts(N);
}

//------------------------------------------

int main () {
    srand(time(0));

    for (int i = 10000; i <= 10000; i++) {
        auto start = std::chrono::steady_clock::now();
        int* solved = min_conflicts(i);
        auto end = std::chrono::steady_clock::now();

        // print_board(solved, i);

        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "elapsed time (" << i << "): " << elapsed_seconds.count() << "s\n";
        std::cout << "\n\n";
        delete[] solved;
    }
    return 0;
}