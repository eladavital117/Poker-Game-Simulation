#include "PokerGame.h"

PokerGame::PokerGame(const int hands[][2][2], const int game_count, const int thread_count, const int hand_amount) {
    for (int i = 0; i < hand_amount; i++) {
        std::cout << "hand " << i + 1 << ": \n";
        print(hands[i], 2);
    }
    remove_player_cards_from_deck(hands);
    create_threads(thread_count, game_count, hands, hand_amount);
    for (int i = 0; i < hand_amount; i++) {
        
        hand_wins[i] /= game_count;
        hand_splits[i] /= game_count;
        std::cout << "hand " << i + 1 << " wins: " << hand_wins[i]*100 << "% splits: " << hand_splits[i]*100 << "%\n";
    }
}

void PokerGame::run_hands(const int hands[][2][2], const int start, const int end, const int hand_amount) {
    
    int hand_wins_thread[8] = { 0 };
    int hand_splits_thread[8] = { 0 };
    int hands_with_space_for_community_cards[8][7][2] = {0};
    double hands_scores[8][3];
    
    for (int i = 0; i < 8; i++) {
        hands_scores[i][2] = i;
    }

    for (int i = 0; i < hand_amount; i++) {
        hands_with_space_for_community_cards[i][0][0] = hands[i][0][0];
        hands_with_space_for_community_cards[i][0][1] = hands[i][0][1];

        hands_with_space_for_community_cards[i][1][0] = hands[i][1][0];
        hands_with_space_for_community_cards[i][1][1] = hands[i][1][1];
    }
    for (int i = start; i < end; i++) {
        int comparison_result[8] = { 0 };
        generate(hands_with_space_for_community_cards);
        for (int j = 0; j < hand_amount; j++) {
            possible_hands(hands_with_space_for_community_cards[j], hands_scores[j]);
        }
        compare_score(hands_scores, comparison_result);

        for (int j = 0; j < hand_amount; j++) {
            if (comparison_result[j] == 1) {
                hand_wins_thread[j]++;
            }
            if (comparison_result[j] == 2) {
                hand_splits_thread[j]++;
            }
            
        } 
    }
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (int j = 0; j < hand_amount; j++) {
            hand_wins[j] += hand_wins_thread[j];
            hand_splits[j] += hand_splits_thread[j];
        }
    }
    
}


// splits the workload to different threads
void PokerGame::create_threads(int thread_count, int total_calls, const int hands[][2][2], const int hand_amount) {
    std::vector<std::thread> threads;
    int calls_per_thread = total_calls / thread_count;
    int remainder = total_calls % thread_count;
    const int start = 0;

    for (int i = 0; i < thread_count; i++) {
        const int end = start + calls_per_thread + (i < remainder ? 1 : 0);
        threads.emplace_back(&PokerGame::run_hands, this, hands, start, end, hand_amount);
    }

    for (auto& t : threads) {
        t.join();
    }
}

void PokerGame::sort_scores(double scores[][3], int n) {
    for (int i = 0; i < 8; i++) {
        scores[i][2] = i;
    }
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            if (scores[j + 1][0] > scores[j][0] ||
                (scores[j+1][0] == scores[j][0] && scores[j+1][1] > scores[j][1])) {
                double temp1 = scores[j][0];
                double temp2 = scores[j][1];
                double temp3 = scores[j][2];
                scores[j][0] = scores[j + 1][0];
                scores[j][1] = scores[j + 1][1];
                scores[j][2] = scores[j + 1][2];
                scores[j + 1][0] = temp1;
                scores[j + 1][1] = temp2;
                scores[j + 1][2] = temp3;
            }
        }
    }
}

void PokerGame::compare_score(double hand_scores[8][3], int comparison_result[8]) {
    sort_scores(hand_scores, 8);

    for (int i = 0; i < 8; i++) {
        if (hand_scores[i][0] == hand_scores[i + 1][0] &&
            hand_scores[i][1] == hand_scores[i + 1][1]) {
            comparison_result[(int)hand_scores[i][2]] = 2;
            comparison_result[(int)hand_scores[i + 1][2]] = 2;
        }
        else {
            comparison_result[(int)hand_scores[i][2]] = 1;
            break;
        }
    }
}
// remove the cards of the players from the deck

void PokerGame::remove_player_cards_from_deck(const int hands[][2][2]) {
    int cards_to_remove[16][2] = {};
    int hand_count = 0;
    int k = 0;
    for (int i = 0; i < 8; i++) {
        if (hands[i][0][0] == 0) {
            break;
        }
        hand_count++;
        cards_to_remove[k][0] = hands[i][0][0];
        cards_to_remove[k][1] = hands[i][0][1];
        k++;
        cards_to_remove[k][0] = hands[i][1][0];
        cards_to_remove[k][1] = hands[i][1][1];
        k++;
    }

    for (int i = 0; i < 52; i++) {
        for (int j = 0; j < hand_count*2; j++) {
            if (deck[i][0] == cards_to_remove[j][0] && deck[i][1] == cards_to_remove[j][1]) {
                deck[i][0] = -1;
                deck[i][1] = -1;
            }
        }
    }
}

// fix generate function!!!
// generates 5 random cards and adds them to each hand
void PokerGame::generate(int hands[][7][2]) {
    int indexes[5] = { -1,-1,-1,-1,-1 };
    int i = 2;
    int rnd;
    bool already_exists;

    while (i < 7) {
        rnd = rand() % 52;
        if (deck[rnd][0] != -1) {
            already_exists = false;
            for (int j = 0; j < 5; j++) {
                if (indexes[j] == rnd) {
                    already_exists = true;
                    break;
                }
            }
            if (!already_exists) {
                for (int h = 0; h < 8; h++) {
                    hands[h][i][0] = deck[rnd][0];
                    hands[h][i][1] = deck[rnd][1];
                }
                indexes[i - 2] = rnd;
                i++;
            }
        }
    }
}

// gets an array of cards (as numbers) and prints it as strings
void PokerGame::print(const int cards[][2], const int size) {
    std::string result;

    for (int i = 0; i < size; i++) {
        result += std::to_string(cards[i][0]);

        if (cards[i][1] == 0) {
            result += "♠";
        }
        else if (cards[i][1] == 1) {
            result += "♥";
        }
        else if (cards[i][1] == 2) {
            result += "♦";
        }
        else if (cards[i][1] == 3) {
            result += "♣";
        }

        result += " ";
    }
    result += "\n";
    std::cout << result;
}


//generate all possible 5 card hands from 7 cards, returns the score of the best hand
void PokerGame::possible_hands(const int cards[][2], double hand_score[]) {
    double max_score[] = { 2,0 };
    int combinations[21][5][2];

    // Generate all combinations of 5 cards out of 7
    int index = 0;
    for (int i = 0; i < 7; ++i) {
        for (int j = i + 1; j < 7; ++j) {
            for (int k = j + 1; k < 7; ++k) {
                for (int l = k + 1; l < 7; ++l) {
                    for (int m = l + 1; m < 7; ++m) {
                        combinations[index][0][0] = cards[i][0];
                        combinations[index][0][1] = cards[i][1];

                        combinations[index][1][0] = cards[j][0];
                        combinations[index][1][1] = cards[j][1];

                        combinations[index][2][0] = cards[k][0];
                        combinations[index][2][1] = cards[k][1];

                        combinations[index][3][0] = cards[l][0];
                        combinations[index][3][1] = cards[l][1];

                        combinations[index][4][0] = cards[m][0];
                        combinations[index][4][1] = cards[m][1];

                        index++;
                    }
                }
            }
        }
    }

    for (auto& hand : combinations) {
        double hand_score[2];
        eval_hand(hand, hand_score);
        if ((hand_score[0] > max_score[0]) || (hand_score[0] == max_score[0] && hand_score[1] > max_score[1])) {
            max_score[0] = hand_score[0];
            max_score[1] = hand_score[1];
        }
    }

    hand_score[0] = max_score[0];
    hand_score[1] = max_score[1];
}

// sorts ranks and suits
void PokerGame::sort(int ranks[], int suits[], int n) {
    for (int i = 1; i < n; i++) {
        int key = ranks[i];
        int key2 = suits[i];
        int j = i - 1;

        while (j >= 0 && ranks[j] < key) {
            ranks[j + 1] = ranks[j];
            suits[j + 1] = suits[j];
            j--;
        }
        ranks[j + 1] = key;
        suits[j + 1] = key2;
    }
}

// returns [rank,score] for the hand, which is enough to evaluate it
void PokerGame::eval_hand(const int hand[][2], double hand_score[]) {
    int ranks[5] = {};
    int suits[5] = {};

    for (int i = 0; i < 5; i++) {
        ranks[i] = hand[i][0];
        suits[i] = hand[i][1];
    }

    sort(ranks, suits, 5);

    double result[2] = { 0, 0 };

    // true if has flush
    bool flush = true;
    for (int i = 1; i < 5; i++) {
        if (suits[i] != suits[0]) {
            flush = false;
        }
    }

    // true if has straight
    bool straight = true;
    for (int i = 1; i < 5; i++) {
        if (ranks[i] != ranks[i - 1] - 1) {
            straight = false;
        }
    }

    // check for flush
    if (flush) {
        // check for straight flush
        if (straight) {
            // check for royal flush
            if (ranks[0] == 14) {
                result[0] = 10;
                result[1] = 0;
            }
            else {
                result[0] = 9;
                result[1] = ranks[0];
            }
        }
        else {
            result[0] = 6;
            result[1] = ranks[0] + (ranks[1] / 14.0) + (ranks[2] / (14.0 * 14.0)) + (ranks[3] / (14.0 * 14.0 * 14.0)) + (ranks[4] / (14.0 * 14.0 * 14.0 * 14.0));
        }
    }

    // check for straight
    if (straight) {
        if (5 > result[0]) {
            result[0] = 5;
            result[1] = ranks[0] + (ranks[1] / 14.0) + (ranks[2] / (14.0 * 14.0)) + (ranks[3] / (14.0 * 14.0 * 14.0)) + (ranks[4] / (14.0 * 14.0 * 14.0 * 14.0));
        }
    }

    // check for quad, full-house, set, two pairs, one pair

    // a map [key,value] where key = card number, value = the amount of times that card appears in the hand

    int rank_count_array[13] = { 0 };

    for (int i = 0; i < 5; i++) {
        rank_count_array[ranks[i] - 2]++;
    }

    bool full_house = false;
    bool set = false;
    bool two_pairs = false;
    bool one_pair = false;

    // check for quad
    for (int i = 0; i < 13; i++) {
        if (rank_count_array[i] == 4) {
            result[0] = 8;
            if (ranks[0] == ranks[1]) {
                result[1] = ranks[0] + (ranks[4] / 14.0);
            }
            else {
                result[1] = ranks[4] + (ranks[0] / 14.0);
            }
            hand_score[0] = result[0];
            hand_score[1] = result[1];
            return;
        }
    }

    // check for full-house or set
    for (int i = 0; i < 13; i++) {
        if (rank_count_array[i] == 3) {
            set = true;
            for (int j = 0; j < 13; j++) {
                if (rank_count_array[j] == 2) {
                    full_house = true;
                    result[0] = 7;
                    result[1] = ranks[0] + (ranks[3] / 14.0);
                }
                if (full_house) {
                    hand_score[0] = result[0];
                    hand_score[1] = result[1];
                    return;
                }

                if (set && !flush) {
                    result[0] = 4;
                    result[1] = (ranks[0] == ranks[2]) ? ranks[0] : (ranks[1] == ranks[3] ? ranks[1] : ranks[2]);
                    if (ranks[0] == ranks[1] && ranks[1] == ranks[2]) {
                        result[1] += (ranks[3] / 14.0) + (ranks[4] / (14.0 * 14.0));
                    }
                    else if (ranks[1] == ranks[2] && ranks[2] == ranks[3]) {
                        result[1] += (ranks[0] / 14.0) + (ranks[4] / (14.0 * 14.0));
                    }
                    else {
                        result[1] += (ranks[0] / 14.0) + (ranks[1] / (14.0 * 14.0));
                    }

                    hand_score[0] = result[0];
                    hand_score[1] = result[1];
                    return;
                }
            }
        }
    }

    if (flush) {
        hand_score[0] = result[0];
        hand_score[1] = result[1];
        return;
    }

    // check for two pairs & one pair
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            if (rank_count_array[i] == 2 && rank_count_array[j] == 2 && j != i) {
                two_pairs = true;
            }
            else if (rank_count_array[i] == 2 && rank_count_array[j] == 2) {
                one_pair = true;
            }
        }
    }

    if (two_pairs) {
        result[0] = 3;
        if (ranks[0] == ranks[1]) {
            if (ranks[2] == ranks[3]) {
                result[1] = ranks[0] + (ranks[2] / 14.0) + (ranks[4] / (14.0 * 14.0));
            }
            else {
                result[1] = ranks[0] + (ranks[3] / 14.0) + (ranks[2] / (14.0 * 14.0));
            }
        }
        else if (ranks[1] == ranks[2]) {
            result[1] = (ranks[0] / (14.0 * 14.0)) + ranks[1] + (ranks[3] / 14.0);
        }
        hand_score[0] = result[0];
        hand_score[1] = result[1];
        return;
    }
    if (one_pair) {
        result[0] = 2;
        if (ranks[0] == ranks[1]) {
            result[1] = ranks[0] + (ranks[2] / 14.0) + (ranks[3] / (14 * 14)) + (ranks[4] / (14.0 * 14.0 * 14.0));
        }
        else if (ranks[1] == ranks[2]) {
            result[1] = ranks[1] + (ranks[0] / 14.0) + (ranks[3] / (14 * 14)) + (ranks[4] / (14.0 * 14.0 * 14.0));
        }
        else if (ranks[2] == ranks[3]) {
            result[1] = ranks[2] + (ranks[0] / 14.0) + (ranks[1] / (14 * 14)) + (ranks[4] / (14.0 * 14.0 * 14.0));
        }
        else if (ranks[3] == ranks[4]) {
            result[1] = ranks[3] + (ranks[0] / 14.0) + (ranks[1] / (14 * 14)) + (ranks[2] / (14.0 * 14.0 * 14.0));
        }
        hand_score[0] = result[0];
        hand_score[1] = result[1];
        return;
    }
    result[0] = 1;
    result[1] = ranks[0];
    hand_score[0] = result[0];
    hand_score[1] = result[1];

}

