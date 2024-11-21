#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 100
#define MAX_WRONG_GUESSES 6
#define MAX_WORDS 100

// Function to load words from a file into an array
int loadWordsFromFile(const char *filename, char words[MAX_WORDS][MAX_WORD_LENGTH]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 0;
    }

    int count = 0;
    while (fgets(words[count], MAX_WORD_LENGTH, file)) {
        size_t len = strlen(words[count]);
        if (len > 0 && words[count][len - 1] == '\n') {
            words[count][len - 1] = '\0'; // Remove newline character
        }
        count++;
        if (count >= MAX_WORDS) {
            break;
        }
    }

    fclose(file);
    return count;
}

// Function to select a random word from the loaded words
const char *getRandomWord(char words[MAX_WORDS][MAX_WORD_LENGTH], int wordCount) {
    if (wordCount == 0) {
        fprintf(stderr, "Error: No words loaded from file\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL)); // Seed for random number generation
    int randomIndex = rand() % wordCount;
    return words[randomIndex];
}

// Function to display the current guessed word with underscores for unguessed letters
void displayWord(const char *word, const int guessed[]) {
    for (size_t i = 0; i < strlen(word); ++i) {
        if (guessed[i]) {
            printf("%c ", word[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

// Function to check if the word is fully guessed
int isWordGuessed(const int guessed[], size_t wordLength) {
    for (size_t i = 0; i < wordLength; ++i) {
        if (!guessed[i]) {
            return 0;
        }
    }
    return 1;
}

// Function to process a player's guess and update the guessed array
int processGuess(char guess, const char *word, int guessed[]) {
    int correctGuess = 0;
    for (size_t i = 0; i < strlen(word); ++i) {
        if (word[i] == guess) {
            guessed[i] = 1;
            correctGuess = 1;
        }
    }
    return correctGuess;
}

// Function to display the state of the game
void displayGameState(int attempts, int maxAttempts, const char *wrongGuesses, int wrongGuessCount) {
    printf("Intentos restantes: %d\n", maxAttempts - attempts);
    printf("Letras incorrectas: ");
    for (int i = 0; i < wrongGuessCount; ++i) {
        printf("%c ", wrongGuesses[i]);
    }
    printf("\n");
}

// Main game function
void playHangman(const char *word, int maxAttempts) {
    size_t wordLength = strlen(word);
    int guessed[wordLength];
    memset(guessed, 0, sizeof(guessed)); // Initialize all elements to 0

    char wrongGuesses[MAX_WRONG_GUESSES];
    int wrongGuessCount = 0;
    int attempts = 0;

    while (attempts < maxAttempts) {
        printf("\nPalabra: ");
        displayWord(word, guessed);

        displayGameState(attempts, maxAttempts, wrongGuesses, wrongGuessCount);

        char guess;
        printf("Adivina una letra: ");
        scanf(" %c", &guess);

        if (!processGuess(guess, word, guessed)) {
            if (wrongGuessCount < MAX_WRONG_GUESSES) {
                wrongGuesses[wrongGuessCount++] = guess;
            }
            attempts++;
        }

        if (isWordGuessed(guessed, wordLength)) {
            printf("\n¡Felicidades! Has adivinado la palabra: %s\n", word);
            return;
        }
    }

    printf("\nLo siento, has perdido. La palabra era: %s\n", word);
}

int main() {
    const char *filename = "palabras.txt"; // File containing words
    char words[MAX_WORDS][MAX_WORD_LENGTH]; // Array to store words

    printf("¡Bienvenido al juego de ahorcado!\n");

    // Load words from file
    int wordCount = loadWordsFromFile(filename, words);
    if (wordCount == 0) {
        fprintf(stderr, "Error: No se pudieron cargar palabras del archivo.\n");
        return EXIT_FAILURE;
    }

    // Select a random word
    const char *randomWord = getRandomWord(words, wordCount);

    // Start the game
    playHangman(randomWord, MAX_WRONG_GUESSES);

    return EXIT_SUCCESS;
}
