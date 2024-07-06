#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50
#define NUM_QUESTIONS 5
#define MAX_LEADERBOARD_ENTRIES 10
#define MAX_CHANCES 5

// Structure for leaderboard entry
typedef struct {
    char name[MAX_NAME_LENGTH];
    int totalScore;
} LeaderboardEntry;

// Global variables
char playerName[MAX_NAME_LENGTH];    // To store player's name
int score = 0;                       // Global variable to store player's total score
int numEntries = 0;                  // Number of entries in the leaderboard
LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES]; // Array to store leaderboard entries
int i,j,q;
// Function prototypes
void showWelcome();
void clearScreen();
void showMenu();
void startGame();
void food();
void sports();
void animals();
void movies();
void exitGame();
void displayLeaderboard();
void updateScore(int points);
void playHangman(char *word, char *hint, int questionNumber);
void drawHangman(int mistakes);
void updateLeaderboard();
void readLeaderboardFromFile();
void writeLeaderboardToFile();
void resetLeaderboard();
void waitForEnter();

// Main function
int main() {
    char choice;

    // Read leaderboard from file on program start
    readLeaderboardFromFile();

    showWelcome();

    do {
        showMenu();
        printf("\nEnter your choice (1-4): ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                startGame();
                break;
            case '2':
                displayLeaderboard();
                break;
            case '3':
                resetLeaderboard();
                break;
            case '4':
                exitGame();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != '4');

    return 0;
}

// Function definitions

void showWelcome() {
    clearScreen();
    printf("\n******************************************* Welcome to the Hangman Game *******************************************\n\n");
    printf("***********************************************************************************************************************\n\n");
    printf("How to play:\n");
    printf("- Select a category and try to guess the word with %d chances.\n", MAX_CHANCES);
    printf("- Score 5 points for each correct letter guess.\n");
    printf("- Enter 'Y' for hint after making one mistake.\n");
    printf("- Enjoy the game!\n\n");
    printf("***********************************************************************************************************************\n\n");

    // Ask for player's name
    printf("Enter your name: ");
    fgets(playerName, sizeof(playerName), stdin);
    strtok(playerName, "\n");  // Remove newline character from fgets

    waitForEnter();
}

void clearScreen() {

    system("cls"); 

}

void showMenu() {
    clearScreen();
    printf("\nMenu:\n");
    printf("  1. Start\n");
    printf("  2. Leaderboard\n");
    printf("  3. Reset Leaderboard\n");
    printf("  4. Exit\n");
}

void startGame() {
    clearScreen();
    printf("\nStarting the game...\n");

    int category;
    printf("\nSelect a category:\n");
    printf("  1. Food\n");
    printf("  2. Sports\n");
    printf("  3. Animals\n");
    printf("  4. Movies\n");
    printf("Enter category (1-4): ");
    scanf("%d", &category);

    switch (category) {
        case 1:
            food();
            break;
        case 2:
            sports();
            break;
        case 3:
            animals();
            break;
        case 4:
            movies();
            break;
        default:
            printf("Invalid category. Please try again.\n");
            break;
    }

    // Display leaderboard after game ends
    displayLeaderboard();
}

// Function to play hangman game for food category
void food() {
    char *words[] = {"apple", "pasta", "pizza", "burger", "chowmein"};
    char *hints[] = {
        "A round, red or green fruit.",
        "Italian food served with sauce.",
        "Round, flat bread base Italian food.",
        "Food consisting patty, sliced vegetables, meat between buns.",
        "Sticky dish made with vegetables, meat."
    };

    for ( q = 0; q < NUM_QUESTIONS; q++) {
        clearScreen();
        printf("\nCategory: Food - Question No. %d\n", q + 1);
        playHangman(words[q], hints[q], q + 1);
    }

    updateLeaderboard();
}

// Function to play hangman game for sports category
void sports() {
    char *words[] = {"football", "basketball", "tennis", "cricket", "hockey"};
    char *hints[] = {
        "Popular spherical ball sport.",
        "Game having 4 quarters.",
        "Racket sport.",
        "Bat-and-ball sport.",
        "Game played with sticks."
    };

    for ( q = 0; q < NUM_QUESTIONS; q++) {
        clearScreen();
        printf("\nCategory: Sports - Question No. %d\n", q + 1);
        playHangman(words[q], hints[q], q + 1);
    }

    updateLeaderboard();
}

// Function to play hangman game for animals category
void animals() {
    char *words[] = {"elephant", "tiger", "kangaroo", "penguin", "crocodile"};
    char *hints[] = {
        "Large mammal.",
        "Large, striped cat.",
        "Large animal from Australia.",
        "Flightless bird.",
        "Large reptile."
    };

    for ( q = 0; q < NUM_QUESTIONS; q++) {
        clearScreen();
        printf("\nCategory: Animals - Question No. %d\n", q + 1);
        playHangman(words[q], hints[q], q + 1);
    }

    updateLeaderboard();
}

// Function to play hangman game for movies category
void movies() {
    char *words[] = {"captain", "pathan", "sholay", "joker", "titanic"};
    char *hints[] = {
        "Anmol KC.",
        "Shahrukh Khan.",
        "Yeh dosti....",
        "Gotham City.",
        "Romantic."
    };

    for ( q = 0; q < NUM_QUESTIONS; q++) {
        clearScreen();
        printf("\nCategory: Movies - Question No. %d\n", q + 1);
        playHangman(words[q], hints[q], q + 1);
    }

    updateLeaderboard();
}

void playHangman(char *word, char *hint, int questionNumber) {
    int len = strlen(word);
    char guess[30];  // Double the length to make dashes wider (+1 for null terminator)
    int mistakes = 0;     // Counting the number of mistakes
    int questionScore = 0;  // Score for current question
    int hintGiven = 0;  // Flag to track if hint has been given

    // Initialize guess array with wider dashes
    for ( j = 0; j < len * 2 - 1; j += 2) {
        guess[j] = '-';
        guess[j + 1] = ' ';  // Add a space between dashes for better readability
    }
    guess[len * 2 - 1] = '\0';

    while (mistakes < MAX_CHANCES) {
        clearScreen();
        printf("Category: Question No. %d\n", questionNumber);
        drawHangman(mistakes);

        // Display the word to guess
        printf("\nWord to guess: %s\n", guess);
        printf("Chances left: %d\n", MAX_CHANCES - mistakes);
        printf("Enter a letter or the whole word: ");

        char input[50];
        scanf("%s", input);

        // Convert the input to lowercase
        for (i = 0; input[i]; i++) {
            input[i] = tolower(input[i]);
        }

        // Check if the input matches the word
        if (strcmp(input, word) == 0) {
            printf("\nCongratulations! You guessed the word correctly: %s\n", word);
            questionScore = 5;  // Calculate score for this question (5 points per correct letter)
            updateScore(questionScore);  // Update total score
            break;  // Exit the loop upon correct guess
        }

        // Check if the input is a single character
        if ((strlen(input) == 1)) {
            char letter = input[0];
            int found = 0;

            // Check if the letter exists in the word
            for (j = 0; j < len; j++) {
                if (tolower(word[j]) == letter) {
                    guess[j * 2] = word[j];  // Update corresponding dash
                    found = 1;
                }
            }

            if (!found) {
                printf("Letter '%c' is not in the word.\n", letter);
                mistakes++;

                // Offer hint if one mistake is made and hint hasn't been given yet
                if (mistakes == 1 && !hintGiven) {
                    char hintChoice;
                    printf("Would you like a hint? (Y/N): ");
                    scanf(" %c", &hintChoice);
                    if (tolower(hintChoice) == 'y') {
                        printf("\nHint: %s\n", hint);
                        hintGiven = 1;
                    }
                }
            } else {
                printf("Good guess!\n");

                // Check if the word is completely guessed
                int correct = 1;
                for (j = 0; j < len; j++) {
                    if (guess[j * 2] == '-') {
                        correct = 0;
                        break;
                    }
                }
                if (correct) {
                    printf("\nCongratulations! You guessed the word: %s\n", word);
                    questionScore = 5;  // Calculate score for this question (5 points per correct letter)
                    updateScore(questionScore);  // Update total score
                    break;  // Exit the loop upon correct guess
                }
            }
        } else {
            printf("Invalid input. Please enter a single letter or the whole word.\n");
        }

        waitForEnter();
    }

    if (mistakes == MAX_CHANCES) {
        printf("\nSorry, you ran out of chances. The word was: %s\n", word);
        printf("\n");
        printf("  _______\n");
        printf("  |     |\n");
        printf("  O     |\n");
        printf(" /|\\    |\n");
        printf(" / \\    |\n");
        printf("        |\n");
        printf("=========\n");
    }

    printf("\nTotal score for this question: %d\n", questionScore);
    printf("\n");
    
    
    waitForEnter();
}



// Function to draw hangman based on the number of mistakes
void drawHangman(int mistakes) {
     printf("\n");
    printf("  _______\n");
    printf("  |     |\n");

    if (mistakes >= 1) {
        printf("  O     |\n"); // Draw the head
    } else {
        printf("        |\n");
    }

    if (mistakes >= 3) {
        printf(" /|\\    |\n"); // Draw full body with arms
    } else if (mistakes == 2) {
        printf(" /|     |\n"); // Draw torso with one arm
    } else {
        printf("        |\n");
    }

    if (mistakes >= 5) {
        printf(" / \\    |\n"); // Draw both legs
    } else if (mistakes == 4) {
        printf(" /      |\n"); // Draw one leg
    } else {
        printf("        |\n"); // No legs
    }

    printf("        |\n");
    printf("=========\n");
}


void updateScore(int points) {
    score += points;
    printf("Updated score: %d\n", score);
}

// Function to exit the game
void exitGame() {
    printf("\nExiting the game. Thank you for playing!\n");
    // Write leaderboard to file on exit
    writeLeaderboardToFile();
    exit(0);
}

// Function to display the leaderboard
void displayLeaderboard() {
    clearScreen();
    printf("\n******************** Leaderboard ********************\n");
    printf("Rank\tName\t\t\tTotal\n");

    if (numEntries == 0) {
        printf("\nNo entries in the leaderboard.\n");
    } else {
        for (i = 0; i < numEntries; i++) {
            printf("%d\t%s\t\t\t%d\n", i + 1, leaderboard[i].name, leaderboard[i].totalScore);
        }
    }

    printf("*****************************************************\n");
    waitForEnter();
}

// Function to update the leaderboard
void updateLeaderboard() {
    // Check if player already exists in the leaderboard
    int found = 0;
    for (i = 0; i < numEntries; i++) {
        if (strcmp(leaderboard[i].name, playerName) == 0) {
            leaderboard[i].totalScore += score;
            found = 1;
            break;
        }
    }

    // If player is not found, add to leaderboard
    if (!found) {
        if (numEntries < MAX_LEADERBOARD_ENTRIES) {
            strcpy(leaderboard[numEntries].name, playerName);
            leaderboard[numEntries].totalScore = score;
            numEntries++;
        } else {
            printf("Leaderboard is full. Cannot add more entries.\n");
        }
    }
    // Sort the leaderboard in descending order of totalScore
    for ( i = 0; i < numEntries - 1; i++) {
        for (j = 0; j < numEntries - i - 1; j++) {
            if (leaderboard[j].totalScore < leaderboard[j + 1].totalScore) {
                LeaderboardEntry temp = leaderboard[j];
                leaderboard[j] = leaderboard[j + 1];
                leaderboard[j + 1] = temp;
            }
        }
    }
}

// Function to read leaderboard from file
void readLeaderboardFromFile() {
    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open leaderboard file.\n");
        return;
    }

    numEntries = 0;
    while (fscanf(file, "%s %d", leaderboard[numEntries].name, &leaderboard[numEntries].totalScore) == 2) {
        numEntries++;
        if (numEntries >= MAX_LEADERBOARD_ENTRIES) {
            break;
        }
    }

    fclose(file);
}

// Function to write leaderboard to file
void writeLeaderboardToFile() {
    FILE *file = fopen("leaderboard.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open leaderboard file.\n");
        return;
    }

    for (i = 0; i < numEntries; i++) {
        fprintf(file, "%s %d\n", leaderboard[i].name, leaderboard[i].totalScore);
    }

    fclose(file);
}

// Function to reset the leaderboard
void resetLeaderboard() {
    numEntries = 0;
    printf("Leaderboard has been reset.\n");
    waitForEnter();
}

// Function to wait for enter key
void waitForEnter() {
    printf("\nPress Enter to continue...\n");
    getchar();
    getchar();  // Wait for enter key
}
