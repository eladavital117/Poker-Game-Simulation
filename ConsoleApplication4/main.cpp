#include <chrono>
#include <windows.h>
#include <iostream>
#include "PokerGame.h"
#include "UserInput.h"

#define MAX_THREAD_COUNT 128

int main()
{
    SetConsoleOutputCP(CP_UTF8); // make it possible for the console to print UTF-8 characters

    int game_count = 1000000, thread_count = 4, hand_count = 3;
    int hands[8][2][2] = {0};
    UserInput(hands, game_count, thread_count, hand_count);
    auto start = std::chrono::high_resolution_clock::now();
    PokerGame pg(hands, game_count, thread_count, hand_count);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time elapsed: " << elapsed.count() << " seconds" << std::endl;
    
    return 0;
}
