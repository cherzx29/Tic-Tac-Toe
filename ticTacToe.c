#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   


/*
Authours; Cherith Boya and Hamza Khan
Date; Nov 10, 2024
Description:

This is a C program that implements a console-based Tic-Tac-Toe game with customizable grid sizes, 
two different game modes, and an AI opponent with basic strategy.

- Players can choose to play in two modes: 
  1. Player vs Player (PvP), where two human players take turns to play.
  2. Player vs AI (PvAI), where one player competes against the AI.

- The game board can be adjusted in size (from 3x3 to 10x10).
- The AI has a simple strategy where it tries to win if possible, or blocks the player from winning.
- The game continues until a player wins or the game results in a draw.
- After each game, the score is displayed, and players are asked if they want to play again.
*/


// Define a constant for the maximum grid size. This helps to set an upper limit on the board size.
#define MAX_SIZE 10  // Maximum grid size

// Function Prototypes (forward declarations). These let the compiler know about the functions used later in the code.
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size);  // Initializes the board
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size);       // Prints the current state of the board
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size);          // Checks if there’s a win condition
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size);         // Checks if the game has resulted in a draw
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player);  // Handles player moves
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size);           // Handles AI moves with a strategy
void updateScore(int *playerScore, int *aiScore, char winner);   // Updates and tracks the score

// Main function: Entry point for the game.
int main() {
    int size, mode;              // Variables to hold board size and game mode (PvP or PvAI)
    char board[MAX_SIZE][MAX_SIZE]; // 2D array to represent the tic-tac-toe board
    int playerScore = 0, aiScore = 0; // Track player and AI scores
    char currentPlayer = 'X';    // Initialize with player X as the starting player

    // Seed the random number generator to allow AI to make random moves. `time(NULL)` provides a time-based seed.
    srand(time(NULL));


    // Display title in yellow
    //USED FOR COLOUR CHANGING; https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
    printf("\033[1;33mWelcome to Tic-Tac-Toe by Cherith Boya and Hamza Khan\033[0m\n\n");


    // Prompt the user for the board size and validate it
    printf("Enter grid size (3-10): ");
    scanf("%d", &size);
    if (size < 3 || size > 10) {   // If the input is out of range, default to 3x3
        printf("Invalid size. Setting to default 3x3.\n");
        size = 3;
    }

    // Prompt the user for the game mode (PvP or PvAI) and store the selection
    do {
        printf("Select mode (1: Player vs Player, 2: Player vs AI): ");
        scanf("%d", &mode);
        if (mode != 1 && mode != 2) {
            printf("Invalid selection. Please enter 1 for Player vs Player or 2 for Player vs AI.\n");
        }
    } while (mode != 1 && mode != 2); // Keep asking until a valid input is received


    // Game loop: Allows the user to play multiple games in a row if desired
    char playAgain;
    do {
        initializeBoard(board, size);  // Set up an empty board for a new game
        currentPlayer = 'X';           // Reset starting player to X each new game
        
        while (1) {                    // Loop for individual game rounds
            printBoard(board, size);   // Display the current state of the board

            // Determine if it’s the player’s or AI’s turn based on game mode and player type
            if (mode == 1 || currentPlayer == 'X') {
                playerMove(board, size, currentPlayer);  // Player makes a move
            } else {
                aiMove(board, size);                     // AI makes a move
            }

            // Check if the move has resulted in a win
            if (checkWin(board, size)) {
                printBoard(board, size);  // Show final board state
                printf("Player %c wins!\n", currentPlayer);
                
                // Update the score based on who won
                if (currentPlayer == 'X') {
                    updateScore(&playerScore, &aiScore, 'X');
                } else {
                    updateScore(&playerScore, &aiScore, 'O');
                }
                break;
            } else if (checkDraw(board, size)) {  // Check for draw condition if no one has won
                printBoard(board, size);
                printf("It's a draw!\n");
                break;
            }

            // Switch turns: Toggle between player X and player O
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }

        // Display the current scores for both players
        printf("Score: Player 1 = %d, AI or Player 2 = %d\n", playerScore, aiScore);

        // Ask if the user wants to play another game
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &playAgain);

    } while (playAgain == 'y' || playAgain == 'Y'); // Continue if the user says 'y'

    return 0;  // Exit the program
}

// Function to initialize the board by filling it with empty spaces (' ')
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }
}


void printBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {          // Loop over each row
        for (int j = 0; j < size; j++) {      // Loop over each column in the row
            if (board[i][j] == 'X') {
                printf("\033[1;31m X \033[0m");  // Print X in red, padded for alignment
            } else if (board[i][j] == 'O') {
                printf("\033[1;34m O \033[0m");  // Print O in blue, padded for alignment
            } else {
                printf("   ");                  // Empty space, padded for alignment
            }

            if (j < size - 1) printf(" |");     // Print a vertical separator if it's not the last cell in the row
        }
        printf("\n");                          // Move to the next line after a row is printed
        
        if (i < size - 1) {                    // Print a horizontal separator between rows, except after the last row
            for (int j = 0; j < size; j++) {
                printf("----");                // Print dashes for each cell
                if (j < size - 1) printf("+");  // Print a plus sign between cells
            }
            printf("\n");                      // Move to the next line after a separator row
        }
    }
    printf("\n");                              // Final newline for spacing after the board
}



int checkWin(char board[MAX_SIZE][MAX_SIZE], int size) {
    // Check rows
    for (int i = 0; i < size; i++) {
        int rowWin = 1;  // Flag to check if all elements in the row are the same
        for (int j = 1; j < size; j++) {
            if (board[i][j] != board[i][0] || board[i][0] == ' ') {
                rowWin = 0;
                break;  // Exit the loop as soon as a non-matching or empty cell is found
            }
        }
        if (rowWin) {
            return 1;  // Found a win in this row
        }
    }

    // Check columns
    for (int j = 0; j < size; j++) {
        int colWin = 1;  // Flag to check if all elements in the column are the same
        for (int i = 1; i < size; i++) {
            if (board[i][j] != board[0][j] || board[0][j] == ' ') {
                colWin = 0;
                break;  // Exit the loop as soon as a non-matching or empty cell is found
            }
        }
        if (colWin) {
            return 1;  // Found a win in this column
        }
    }

    // Check diagonals
    // Top-left to bottom-right diagonal
    int diag1Win = 1;
    for (int i = 1; i < size; i++) {
        if (board[i][i] != board[0][0] || board[0][0] == ' ') {
            diag1Win = 0;
            break;
        }
    }
    if (diag1Win) {
        return 1;  // Found a win in this diagonal
    }

    // Top-right to bottom-left diagonal
    int diag2Win = 1;
    for (int i = 1; i < size; i++) {
        if (board[i][size - 1 - i] != board[0][size - 1] || board[0][size - 1] == ' ') {
            diag2Win = 0;
            break;
        }
    }
    if (diag2Win) {
        return 1;  // Found a win in this diagonal
    }

    return 0;  // No win condition found
}


// Function to check for a draw (all cells filled with no winner)
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {  // Empty cell found, no draw
                return 0;
            }
        }
    }
    return 1;  // All cells are filled, so it’s a draw
}

// Function to handle player moves
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int row, col;
    do {
        printf("Player %c, enter your move (ROW,COLUMN): ", player);
        scanf("%d,%d", &row, &col);

        // Check if the row/column is out of bounds
        if (row < 1 || row > size || col < 1 || col > size) {
            printf("Invalid entry. Row and column must be between 1 and %d.\n", size);
        }
        // Check if the spot is already filled
        else if (board[row - 1][col - 1] != ' ') {
            printf("The spot at (%d, %d) is already filled. Please choose another spot.\n", row, col);
        }

    } while (row < 1 || row > size || col < 1 || col > size || board[row - 1][col - 1] != ' ');

    board[row - 1][col - 1] = player;  // Place the player's mark on the board
}


// AI move with strategic checking for winning or blocking opportunities
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size) {
    int row, col;
    
    // Win Check: Try each empty cell to see if placing an 'O' leads to a win
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'O';  // Tentatively place 'O'
                if (checkWin(board, size)) {
                    return;  // Keep this move if it leads to a win
                }
                board[row][col] = ' ';  // Undo the move if it doesn’t result in a win
            }
        }
    }

    // Block Check: Check if player can win and block by placing 'O' in the player's potential winning position
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'X';  // Tentatively place 'X'
                if (checkWin(board, size)) {
                    board[row][col] = 'O';  // Place 'O' to block the player’s winning move
                    return;
                }
                board[row][col] = ' ';  // Undo the move if it doesn't lead to player win
            }
        }
    }

    // Random Move: If no winning/blocking moves are available, choose a random empty cell
    do {
        row = rand() % size;
        col = rand() % size;
    } while (board[row][col] != ' ');

    board[row][col] = 'O';  // Place 'O' in the random empty position
}

// Function to update the score depending on the winner
void updateScore(int *playerScore, int *aiScore, char winner) {
    if (winner == 'X') {
        (*playerScore)++;  // Increment player’s score if X wins
    } else if (winner == 'O') {
        (*aiScore)++;      // Increment AI’s score if O wins
    }
}