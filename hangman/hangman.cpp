#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <set>
#include <vector>

std::string chooseWord(const std::vector<std::string>& words) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, words.size() - 1);
    return words[dist(gen)];
}

void printBoard(const std::string& word, const std::set<char>& guessed) {
    for (char ch : word) {
        if (guessed.count(ch)) {
            std::cout << ch << ' ';
        } else {
            std::cout << "_ ";
        }
    }
    std::cout << '\n';
}

void printHangman(int remainingAttempts) {
    const int maxAttempts = 6;
    int wrongGuesses = maxAttempts - remainingAttempts;
    int stage = std::min(wrongGuesses, 6);

    const std::vector<std::vector<std::string>> states = {
        {
            "  _______",
            " |/      |",
            " |",
            " |",
            " |",
            "_|___"
        },
        {
            "  _______",
            " |/      |",
            " |      (_)",
            " |",
            " |",
            "_|___"
        },
        {
            "  _______",
            " |/      |",
            " |      (_)",
            " |       |",
            " |",
            "_|___"
        },
        {
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \|",
            " |",
            "_|___"
        },
        {
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \|/",
            " |",
            "_|___"
        },
        {
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \|/",
            " |       |",
            "_|___"
        },
        {
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \|/",
            " |       |",
            "_|___ /"
        }
    };

    for (const std::string& line : states[stage]) {
        std::cout << line << '\n';
    }
}

bool isWordGuessed(const std::string& word, const std::set<char>& guessed) {
    for (char ch : word) {
        if (!guessed.count(ch)) {
            return false;
        }
    }
    return true;
}

std::string sanitize(const std::string& input) {
    std::string result;
    for (char ch : input) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            result.push_back(std::tolower(static_cast<unsigned char>(ch)));
        }
    }
    return result;
}

int main() {
    std::vector<std::string> wordList = {
        "apple", "banana", "computer", "hangman", "programming",
        "rocket", "library", "mountain", "forest", "puzzle",
        "dragon", "galaxy", "ocean", "planet", "adventure",
        "festival", "notebook", "sunshine", "jungle", "velocity",
        "whisper", "tornado", "butterfly", "sandwich", "champion",
        "horizon", "parachute", "symphony", "backpack", "treasure",
        "elephant", "moonlight", "starlight", "pineapple", "dolphin",
        "guitar", "rainbow", "keyboard", "passport", "concert",
        "volcano", "bicycle", "fireworks", "umbrella",
        "pitstop", "aerodynamics", "qualifying", "downforce", "chassis",
        "poleposition", "flyaway", "tyre", "suspension", "marshals",
        "threepointer", "dribble", "rebound", "freethrow", "court",
        "alleyoop", "crossover", "fastbreak", "dunk", "guard"
    };

    std::cout << "=== Hangman Game ===\n";
    bool keepPlaying = true;

    while (keepPlaying) {
        std::string secret = chooseWord(wordList);
        std::set<char> guessedLetters;
        constexpr int maxAttempts = 6;
        int remainingAttempts = maxAttempts;

        std::cout << "\nA new word has been chosen. Try to guess it!\n";

        while (remainingAttempts > 0 && !isWordGuessed(secret, guessedLetters)) {
            printHangman(remainingAttempts);
            printBoard(secret, guessedLetters);
            std::cout << "Guessed letters: ";
            for (char ch : guessedLetters) {
                std::cout << ch << ' ';
            }
            std::cout << "\n";
            std::cout << "Remaining attempts: " << remainingAttempts << "\n";
            std::cout << "Enter a letter or guess the whole word: ";

            std::string guess;
            std::getline(std::cin, guess);
            if (guess.empty()) {
                std::cout << "Please enter a letter or a word.\n";
                continue;
            }

            guess = sanitize(guess);
            if (guess.empty()) {
                std::cout << "Please enter alphabetic characters only.\n";
                continue;
            }

            if (guess.size() == 1) {
                char letter = guess[0];
                if (guessedLetters.count(letter)) {
                    std::cout << "You already guessed '" << letter << "'. Try another.\n";
                    continue;
                }

                guessedLetters.insert(letter);
                if (secret.find(letter) != std::string::npos) {
                    std::cout << "Good guess!\n";
                } else {
                    remainingAttempts--;
                    std::cout << "No '" << letter << "' in the word.\n";
                }
            } else {
                if (guess == secret) {
                    for (char ch : secret) guessedLetters.insert(ch);
                    break;
                }
                remainingAttempts--;
                std::cout << "Wrong word guess.\n";
            }
        }

        if (isWordGuessed(secret, guessedLetters)) {
            std::cout << "\nCongratulations! You guessed the word: " << secret << "\n";
        } else {
            std::cout << "\nGame over. The word was: " << secret << "\n";
        }

        std::cout << "Play again? (y/n): ";
        std::string answer;
        std::getline(std::cin, answer);
        answer = sanitize(answer);
        keepPlaying = !answer.empty() && answer[0] == 'y';
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}
