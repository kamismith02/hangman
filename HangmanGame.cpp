#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory> // For smart pointers
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// Base class for a game
class Game {
public:
    virtual void play() = 0; // Virtual function to be overridden
    virtual ~Game() {}
};

// Derived class for Hangman
class Hangman : public Game {
private:
    string word;               // The word to guess
    string guessedWord;        // Word display with blanks
    vector<char> guessedLetters; // Guessed letters
    int maxAttempts;           // Max number of incorrect attempts
    int attempts;              // Number of incorrect attempts made

    void displayStatus() {
        cout << "\nWord: " << guessedWord << "\n";
        cout << "Guessed Letters: ";
        for (char c : guessedLetters) cout << c << " ";
        cout << "\nAttempts left: " << (maxAttempts - attempts) << "\n";
    }

    bool checkWin() {
        return guessedWord == word;
    }

    bool isLetterGuessed(char guess) {
        return find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end();
    }

public:
    Hangman(const string& chosenWord, int maxAttempts = 6) 
        : word(chosenWord), maxAttempts(maxAttempts), attempts(0) {
        guessedWord = string(word.size(), '_'); // Initialize blanks
    }

    void play() override {
        cout << "Welcome to Hangman!\n";
        while (attempts < maxAttempts && !checkWin()) {
            displayStatus();
            cout << "Enter your guess: ";
            char guess;
            cin >> guess;

            // Check if already guessed
            if (isLetterGuessed(guess)) {
                cout << "You already guessed that letter.\n";
                continue;
            }

            guessedLetters.push_back(guess);

            // Check if guess is correct
            if (word.find(guess) != string::npos) {
                cout << "Good guess!\n";
                for (size_t i = 0; i < word.size(); i++) {
                    if (word[i] == guess) guessedWord[i] = guess;
                }
            } else {
                cout << "Wrong guess!\n";
                attempts++;
            }
        }

        // Game result
        if (checkWin()) {
            cout << "Congratulations! You guessed the word: " << word << "\n";
        } else {
            cout << "Game Over! The word was: " << word << "\n";
        }
    }
};

// Helper function to read words from a file
vector<string> readWordsFromFile(const string& filename) {
    ifstream file(filename);
    vector<string> words;
    string word;

    if (!file) {
        cerr << "Error opening file: " << filename << "\n";
        return words;
    }

    while (file >> word) {
        words.push_back(word);
    }

    return words;
}

// Function to dynamically create a Hangman game object
unique_ptr<Game> createHangmanGame(const vector<string>& words) {
    if (words.empty()) {
        cerr << "No words available to play the game.\n";
        exit(1);
    }

    srand(static_cast<unsigned int>(time(0)));
    string randomWord = words[rand() % words.size()];
    return make_unique<Hangman>(randomWord); // Using smart pointer
}

// Main function
int main() {
    string filename = "words.txt";
    vector<string> words = readWordsFromFile(filename);

    if (words.empty()) {
        cout << "No words found in the file. Exiting...\n";
        return 1;
    }

    unique_ptr<Game> game = createHangmanGame(words);
    game->play();

    return 0;
}
