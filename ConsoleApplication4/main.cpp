#include <chrono>
#include <windows.h>
#include <iostream>

#include "PokerGame.h"

#define GAME_COUNT 1000000
#define THREAD_COUNT 8

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    SetConsoleOutputCP(CP_UTF8); // make it possible for the console to print UTF-8 characters
    const int hand1[][2] = { {14,2}, {13,2} };
    const int hand2[][2] = { {2,0}, {2,1} };
    PokerGame pg(hand1, hand2, GAME_COUNT, THREAD_COUNT);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time elapsed: " << elapsed.count() << " seconds" << std::endl;
}
