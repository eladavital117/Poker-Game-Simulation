#pragma once
#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex>

class PokerGame{
public:
    /**
    * starts a new simulation of games between 2 poker hands
    *
    * @param[in] hand1 the first poker hand
    * @param[in] hand2 the second poker hand
    * @param[in] game_count the amount of games simulate
    * @param[in] thread_count the amount of threads to use
    */
    PokerGame(const int hand1[][2], const int hand2[][2], const int game_count, const int thread_count);

    /**
    * prints an array of cards (ex: {{2,1},{2,2}} outputs 2♥, 2♦)
    *
    * @param[in] cards the array of cards
    * @param[in] size the length of that array
    */
    void print(const int cards[][2], const int size);

private:
    double hand1_wins = 0;
    double hand2_wins = 0;
    double draw = 0;
    std::mutex mtx;

    int deck[52][2] = {
    {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0},
    {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}, {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 1},
    {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}, {11, 2}, {12, 2}, {13, 2}, {14, 2},
    {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}, {10, 3}, {11, 3}, {12, 3}, {13, 3}, {14, 3}
    };

    void run_hands(const int h1[][2], const int h2[][2], const int start, const int end);
    void create_threads(int threadCount, int totalCalls, const int h1[][2], const int h2[][2]);
    int compare_score(double h1[], double h2[]);
    void remove_player_cards_from_deck(const int h1[][2], const int h2[][2]);

    // generates random 5 cards for the current game
    void generate(int hand1[][2], int hand2[][2]);

    void possible_hands(const int cards[][2], double hand_score[]);
    void sort(int ranks[], int suits[], int n);
    void eval_hand(const int hand[][2], double hand_score[]);
};
