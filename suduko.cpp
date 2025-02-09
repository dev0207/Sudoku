#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#define LEN 9 // LEN represents the size of our sudoku

// Function to check if placing 'val' at board[row][col] is valid
bool is_possible(const std::vector<std::vector<int>>& board, int row, int col, int val) {
    // Check column
    for (int i = 0; i < LEN; ++i) {
        if (board[i][col] == val) {
            return false;
        }
    }
    // Check row
    for (int i = 0; i < LEN; ++i) {
        if (board[row][i] == val) {
            return false;
        }
    }
    // Check 3x3 square
    int startRow = 3 * (row / 3);
    int startCol = 3 * (col / 3);
    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            if (board[i][j] == val) {
                return false;
            }
        }
    }
    return true;
}

// Function to find the next empty cell using MRV (Most Constrained Variable)
bool find_next_cell(const std::vector<std::vector<int>>& board, int& row, int& col) {
    int minPossibilities = LEN + 1;  // More than the max possible values (9)
    bool found = false;

    // Loop through all cells and find the one with the fewest possibilities
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            if (board[i][j] == 0) {  // Only consider empty cells
                int possibilities = 0;
                for (int val = 1; val <= LEN; ++val) {
                    if (is_possible(board, i, j, val)) {
                        possibilities++;
                    }
                }
                if (possibilities < minPossibilities) {
                    row = i;
                    col = j;
                    minPossibilities = possibilities;
                    found = true;
                }
            }
        }
    }
    return found;
}

// Solve the sudoku using backtracking with MRV and forward checking
bool solve(std::vector<std::vector<int>>& board) {
    int row, col;

    // Use MRV heuristic to find the next empty cell
    if (!find_next_cell(board, row, col)) {
        return true;  // Puzzle solved
    }

    // Try each number for the current empty cell
    for (int num = 1; num <= LEN; ++num) {
        if (is_possible(board, row, col, num)) {
            board[row][col] = num;

            if (solve(board)) {
                return true;
            }

            // Backtrack
            board[row][col] = 0;
        }
    }
    return false;  // No solution found
}

// Print the sudoku board with 0 replaced by '-'
void print_board(const std::vector<std::vector<int>>& board) {
    for (int row = 0; row < LEN; ++row) {
        for (int col = 0; col < LEN; ++col) {
            if (board[row][col] == 0) {
                std::cout << "- "; // Replace 0 with dash
            } else {
                std::cout << board[row][col] << " ";
            }

            if ((col + 1) % 3 == 0 && col < LEN - 1) {
                std::cout << "| ";
            }
        }
        std::cout << std::endl;

        if ((row + 1) % 3 == 0 && row < LEN - 1) {
            std::cout << "------+-------+------" << std::endl;
        }
    }
}

// Function to generate a random sudoku puzzle
bool generate_sudoku(std::vector<std::vector<int>>& board) {
    // Initialize board with 0s
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            board[i][j] = 0;
        }
    }

    // Try to solve a random sudoku to generate a valid puzzle
    if (solve(board)) {
        // Remove some numbers to create a puzzle (difficulty)
        int num_remove = rand() % (LEN * LEN / 2); // Remove up to half the cells
        while (num_remove--) {
            int row = rand() % LEN;
            int col = rand() % LEN;
            board[row][col] = 0;
        }
        return true;
    }
    return false;
}

// Function to enter a custom Sudoku puzzle
void enter_custom_sudoku(std::vector<std::vector<int>>& board) {
    std::cout << "Enter the Sudoku puzzle row by row (use 0 for empty cells):\n";
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            int value;
            do {
                std::cout << "Enter value for cell [" << i + 1 << "][" << j + 1 << "]: ";
                std::cin >> value;
                if (value < 0 || value > 9) {
                    std::cout << "Invalid input! Please enter a number between 0 and 9.\n";
                }
            } while (value < 0 || value > 9);
            board[i][j] = value;
        }
    }
}

// Menu to interact with the user
void menu() {
    std::cout << "Sudoku Solver\n";
    std::cout << "1. Generate a random Sudoku puzzle\n";
    std::cout << "2. Enter a custom Sudoku puzzle\n";
    std::cout << "3. Solve the current Sudoku puzzle\n";
    std::cout << "4. Print the current Sudoku puzzle\n";
    std::cout << "5. Exit\n";
    std::cout << "Choose an option (1-5): ";
}

int main() {
    srand(time(0)); // Initialize random seed based on current time
    std::vector<std::vector<int>> sudokuBoard(LEN, std::vector<int>(LEN, 0));

    bool puzzleGenerated = false;
    bool puzzleSolved = false;

    int choice;

    while (true) {
        menu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Generate a random Sudoku puzzle
                if (generate_sudoku(sudokuBoard)) {
                    std::cout << "Random Sudoku puzzle generated:\n";
                    puzzleGenerated = true;
                    puzzleSolved = false;
                    print_board(sudokuBoard);
                } else {
                    std::cout << "Failed to generate Sudoku puzzle.\n";
                }
                break;

            case 2:
                // Enter a custom Sudoku puzzle
                enter_custom_sudoku(sudokuBoard);
                puzzleGenerated = true;
                puzzleSolved = false;
                break;

            case 3:
                // Solve the Sudoku puzzle
                if (!puzzleGenerated) {
                    std::cout << "Please generate or enter a Sudoku puzzle first.\n";
                } else {
                    if (solve(sudokuBoard)) {
                        std::cout << "Sudoku puzzle solved:\n";
                        puzzleSolved = true;
                    } else {
                        std::cout << "No solution exists.\n";
                        puzzleSolved = false;
                    }
                    print_board(sudokuBoard);
                }
                break;

            case 4:
                // Print the current Sudoku puzzle
                if (!puzzleGenerated) {
                    std::cout << "No puzzle to print. Please generate or enter a Sudoku puzzle first.\n";
                } else {
                    std::cout << "Current Sudoku puzzle:\n";
                    print_board(sudokuBoard);
                }
                break;

            case 5:
                // Exit
                std::cout << "Exiting the program. Goodbye!\n";
                return 0;

            default:
                std::cout << "Invalid option. Please choose a valid option (1-5).\n";
        }
    }

    return 0;
}
