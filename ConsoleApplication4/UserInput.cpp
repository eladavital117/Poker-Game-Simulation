#include "UserInput.h";

#define MAX_THREAD_COUNT 128

UserInput::UserInput(int hands[][2][2], int game_count, int thread_count, int hand_count) {
    std::cout << "In this simulation you can analyze poker hands, getting accurate win chances for each hand.\n";
    std::cout << "first, input the amount of hands you would like to have: (between 2 and 8)\n";
    std::cin >> hand_count;

    if (hand_count > 8 || hand_count < 2) {
        std::cout << "must be between 2 and 8\n";
        return ;
    }

    std::cout << "Enter the amount of games you would like to simulate:\n";
    std::cin >> game_count;
    if (game_count <= 0) {
        std::cout << "must be positive\n";
        return ;
    }

    std::cout << "Enter the amount of threads you would like to use:\n";
    std::cin >> thread_count;
    if (thread_count < 1 || thread_count > MAX_THREAD_COUNT) {
        std::cout << "must be between 1 and 128";
    }
    std::cout << "Now enter the cards for each hand, first rank then suit\n";
    std::cout << "- for entering card ranks use the rank number itself or 11,12,13,14 for J,Q,K,A respectively\n";
    std::cout << "- for entering card suits: \n0 = ♠\n1 = ♥\n2 = ♦\n3 = ♣\n";

    for (int i = 0; i < hand_count; i++) {
        std::cout << "enter hand " << i + 1 << " card 1 rank: \n";
        std::cin >> hands[i][0][0];
        std::cout << "enter hand " << i + 1 << " card 1 suit: \n";
        std::cin >> hands[i][0][1];

        for (int j = 0; j < i; j++) {
            if ((hands[j][0][0] == hands[i][0][0] && hands[j][0][1] == hands[i][0][1]) ||
                (hands[j][1][0] == hands[i][0][0] && hands[j][1][1] == hands[i][0][1])) {
                std::cout << "error: cards must be unique\n";
                return;
            }
        }

        std::cout << "enter hand " << i + 1 << " card 2 rank: \n";
        std::cin >> hands[i][1][0];
        std::cout << "enter hand " << i + 1 << " card 2 suit: \n";
        std::cin >> hands[i][1][1];

        for (int j = 0; j < i; j++) {
            if ((hands[j][0][0] == hands[i][1][0] && hands[j][0][1] == hands[i][1][1]) ||
                (hands[j][1][0] == hands[i][1][0] && hands[j][1][1] == hands[i][1][1])) {
                std::cout << "error: cards must be unique\n";
                return;
            }
        }

        if (hands[i][0][0] > 14 || hands[i][0][0] < 2 || hands[i][0][1] > 3 || hands[i][0][1] < 0 ||
            hands[i][1][0] > 14 || hands[i][1][0] < 2 || hands[i][1][1] > 3 || hands[i][1][1] < 0) {
            std::cout << "error: numbers are not within range\n";
            return;
        }

        if (hands[i][0][0] == hands[i][1][0] && hands[i][0][1] == hands[i][1][1]) {
            std::cout << "error: cards must be unique\n";
            return;
        }

        std::cout << "\n\n";
    }
    std::cout << "* the simulation will now begin for " << game_count << " games.\n";
    std::cout << "* it will be splitted across " << thread_count << " threads\n";
    std::cout << "* it will be for the following " << hand_count << " hands:\n";
}
