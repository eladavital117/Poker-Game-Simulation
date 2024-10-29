# Poker Game Simulation


## Overview

This project is a C++ application designed to simulate and analyze poker hands. It performs Monte Carlo simulations to compare two poker hands across a specified number of games. The simulation uses multithreading to handle large numbers of games efficiently, providing statistical insights into the relative strengths of the poker hands.


## Project Files
The project consists of the following files:

1. **PokerGame.cpp**: The core logic for simulating games, creating and managing threads and evaluating poker hands.

2. **UserInput.cpp**: Handles user input such as the amount of hands to be simulated, the amount of threads to use, etc..

3. **main.cpp**: The entry point of the application. It sets up the poker hands, initializes the PokerGame class, and runs the simulation. It also measures and outputs the time taken for the simulation.


## Key Features
- **Simulation of Poker Games**: Simulates a large number of poker games between up to 8 poker hands.

- **Multithreading**: Utilizes multiple threads to parallelize the simulation process, significantly reducing computation time for large numbers of games.

- **Hand Evaluation**: Includes comprehensive logic for evaluating poker hands based on standard poker rules, including flushes, straights, and full houses.

- **Card Generation**: Randomly generates the remaining cards in the deck for each game, ensuring a fair and unbiased simulation.

- **Statistical Output**: Provides percentages of wins for each hand and draws, offering insights into the relative strength of the hands.


## Prerequisites
Ensure you have the Visual Studio with C++ development tools installed.


## License
This project is licensed under the MIT License. See the 'LICENSE.txt' file for details.


## Author
Elad Avital

feel free to reach out with any questions or feedback!
