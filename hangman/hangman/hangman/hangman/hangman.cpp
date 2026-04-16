// Include-uri pentru bibliotecile standard C++
// <algorithm> - contine algoritmi standard pentru operatii pe containere, cum ar fi find pentru cautare, sort pentru sortare, etc.
// <cctype> - contine functii pentru verificare si conversie a caracterelor, cum ar fi isalpha pentru verificare daca e litera, tolower pentru conversie la litera mica
// <cstdlib> - contine functii pentru interactiune cu sistemul de operare, cum ar fi system() pentru executie comenzi in shell
// <iostream> - contine obiecte pentru input/output standard, cum ar fi cout pentru afisare pe ecran, cin pentru citire de la tastatura
// <random> - contine clase pentru generare numere pseudo-aleatorii, cum ar fi random_device pentru entropie hardware, mt19937 pentru motor de generare
// <set> - contine containerul set, care stocheaza elemente unice si sortate automat, folosit pentru litere ghicite
// <vector> - contine containerul vector, care este un array dinamic ce poate creste, folosit pentru lista de cuvinte
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

// Functie pentru alegerea unui cuvant aleatoriu din lista de cuvinte
std::string chooseWord(const std::vector<std::string>& words) {
    // std::random_device rd; - creeaza un obiect pentru generare entropie din hardware (numere aleatorii adevarate)
    std::random_device rd;
    // std::mt19937 gen(rd()); - motor Mersenne Twister pentru generare numere pseudo-aleatorii, initializat cu seed de la rd pentru variabilitate
    std::mt19937 gen(rd());
    // std::uniform_int_distribution<std::size_t> dist(0, words.size() - 1); - distributie uniforma pentru selectie index intre 0 si numarul de cuvinte minus 1
    std::uniform_int_distribution<std::size_t> dist(0, words.size() - 1);
    // return words[dist(gen)]; - genereaza un index aleatoriu si returneaza cuvantul corespunzator
    return words[dist(gen)];
}

// Functie pentru afisarea tablei cu literele ghicite si neghicite
void printBoard(const std::string& word, const std::set<char>& guessed) {
    // Parcurgem fiecare litera din cuvantul secret
    for (char ch : word) {
        if (guessed.count(ch)) {
            // Litera a fost ghicita, o afisam
            std::cout << ch << ' ';
        } else {
            // Litera nu a fost ghicita, afisam underscore ca placeholder
            std::cout << "_ ";
        }
    }
    std::cout << '\n';
}

// Functie pentru afisarea spanzuratorii in functie de incercarile ramase
void printHangman(int remainingAttempts, int maxAttempts) {
    // Calculam numarul de greseli facute pana acum
    int wrongGuesses = maxAttempts - remainingAttempts;
    if (wrongGuesses < 0) {
        wrongGuesses = 0;
    }
    // Determinam stadiul spanzuratorii (de la 0 la 6, unde 6 e complet)
    int stage = std::min(wrongGuesses, 6);

    // Array 2D cu stadiile spanzuratorii, fiecare sub-array reprezinta linii ASCII pentru desen
    const std::vector<std::vector<std::string>> states = {
        { // Stadiu 0: nici o parte desenata
            "  _______",
            " |/      |",
            " |",
            " |",
            " |",
            "_|___"
        },
        { // Stadiu 1: cap
            "  _______",
            " |/      |",
            " |      (_)",
            " |",
            " |",
            "_|___"
        },
        { // Stadiu 2: cap si trunchi
            "  _______",
            " |/      |",
            " |      (_)",
            " |       |",
            " |",
            "_|___"
        },
        { // Stadiu 3: cap, trunchi si un brat
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \\|",
            " |",
            "_|___"
        },
        { // Stadiu 4: cap, trunchi si ambele brate
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \\|/",
            " |",
            "_|___"
        },
        { // Stadiu 5: cap, trunchi, brate si o picior
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \\|/",
            " |       |",
            " |      /"
        },
        { // Stadiu 6: spanzuratoarea completa
            "  _______",
            " |/      |",
            " |      (_)",
            " |      \\|/",
            " |       |",
            " |      / \\"
        }
    };

    // Afisam fiecare linie a stadiului curent
    for (const std::string& line : states[stage]) {
        std::cout << line << '\n';
    }
}

// Functie pentru verificare daca cuvantul a fost ghicit complet
bool isWordGuessed(const std::string& word, const std::set<char>& guessed) {
    // Verificam fiecare litera din cuvant
    for (char ch : word) {
        if (!guessed.count(ch)) {
            // Daca vreo litera nu e in setul ghicit, returnam false
            return false;
        }
    }
    // Toate literele au fost ghicite
    return true;
}

// Functie pentru curatarea input-ului, pastrand doar literele si convertind la litere mici
std::string sanitize(const std::string& input) {
    std::string result;
    // Parcurgem fiecare caracter din input
    for (char ch : input) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            // Daca e litera, o convertim la litera mica si o adaugam
            result.push_back(std::tolower(static_cast<unsigned char>(ch)));
        }
        // Ignoram caracterele non-alfabetice
    }
    return result;
}

enum class Difficulty { Easy, Medium, Hard };

std::string difficultyName(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy: return "Easy";
        case Difficulty::Medium: return "Medium";
        case Difficulty::Hard: return "Hard";
    }
    return "Medium";
}

int getMaxAttempts(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy: return 8;
        case Difficulty::Medium: return 6;
        case Difficulty::Hard: return 5;
    }
    return 6;
}

std::vector<std::string> getWordList(Difficulty difficulty) {
    if (difficulty == Difficulty::Easy) {
        return {
            "goal", "pass", "shoot", "dunk", "court",
            "pit", "lap", "fuel", "guard", "block",
            "save", "race", "team", "score", "ball",
            "kick", "win", "play", "fast", "jump",
            "tire", "racecar", "coach", "match", "drill"
        };
    } else if (difficulty == Difficulty::Medium) {
        return {
            "pitstop", "overtake", "podium", "corner", "header",
            "foul", "assist", "crossover", "defender", "striker",
            "midfield", "rebound", "freekick", "fastbreak", "threepointer",
            "suspension", "tyre", "brake", "steer", "horsepower",
            "champion", "circuit", "referee", "goalkeeper", "tackle"
        };
    }
    return {
        "aerodynamics", "qualifying", "downforce", "chassis", "poleposition",
        "marshals", "pitlane", "alleyoop", "crossbar", "shotclock",
        "goalkeeper", "freethrow", "dribble", "offside", "safetycar",
        "gridwalk", "cornertaking", "speedtrap", "understeer", "oversteer",
        "sidepod", "braking", "throttle", "windtunnel", "gearbox"
    };
}

void printDifficultyBanner() {
    std::cout << "\n=======================================\n";
    std::cout << "|          CHOOSE DIFFICULTY?         |\n";
    std::cout << "|  1 - Easy   (more guesses, easier)  |\n";
    std::cout << "|  2 - Medium (classic hangman)       |\n";
    std::cout << "|  3 - Hard   (tough words, fewer tries)|\n";
    std::cout << "=======================================\n";
}

Difficulty chooseDifficulty() {
    printDifficultyBanner();
    std::cout << "Enter 1, 2 or 3: ";
    std::string input;
    std::getline(std::cin, input);
    if (input == "1" || sanitize(input) == "easy") {
        return Difficulty::Easy;
    }
    if (input == "3" || sanitize(input) == "hard") {
        return Difficulty::Hard;
    }
    return Difficulty::Medium;
}

// Functie pentru afisarea banner-ului de victorie
void printWinBanner() {
    // Afisare banner ASCII pentru victorie
    std::cout << "\n#############################\n";
    std::cout << "#         YOU WIN!         #\n";
    std::cout << "#    Play again? (y/n)     #\n";
    std::cout << "#############################\n\n";
}

// Functie pentru afisarea banner-ului de infrangere
void printLoseBanner(const std::string& word) {
    // Latimea interioara a banner-ului pentru aliniere
    const int innerWidth = 29;
    // Functie lambda pentru afisare linii in banner, captureaza variabile locale prin referinta [&]
    auto printLine = [&](const std::string& text) {
        std::string line = " " + text;
        if ((int)line.size() > innerWidth) {
            // Trunchiem textul daca depaseste latimea
            line = line.substr(0, innerWidth);
        }
        // Adaugam spatii pentru a umple latimea
        line += std::string(innerWidth - line.size(), ' ');
        std::cout << "#" << line << "#\n";
    };

    // Afisare banner ASCII pentru infrangere
    std::cout << "\n###################################\n";
    std::cout << "#          GAME OVER             #\n";
    std::cout << "###################################\n";
    // Afisare cuvantul secret dezvaluit
    printLine("The word was: " + word);
    // Intrebare pentru reluare joc
    printLine("Play again? (y/n)");
    std::cout << "###################################\n\n";
}

// Functia principala a programului
int main() {
    Difficulty difficulty = chooseDifficulty();
    std::vector<std::string> wordList = getWordList(difficulty);
    int maxAttempts = getMaxAttempts(difficulty);

    std::cout << "=== Hangman Game ===\n";
    std::cout << "Difficulty chosen: " << difficultyName(difficulty) << "\n";
    std::cout << "You have " << maxAttempts << " attempts.\n";
    bool keepPlaying = true;

    while (keepPlaying) {
        // Alegere cuvant secret aleatoriu din lista
        std::string secret = chooseWord(wordList);
        // Set pentru stocarea literelor ghicite (unice si sortate)
        std::set<char> guessedLetters;
        // Numar maxim de incercari gresite permise
        int remainingAttempts = maxAttempts;

        // Mesaj pentru inceputul rundei noi
        std::cout << "\nA new word has been chosen. Try to guess it!\n";

        // Bucla interna pentru fiecare incercare, continua pana cand castiga sau pierde
        while (remainingAttempts > 0 && !isWordGuessed(secret, guessedLetters)) {
            // Afisare stadiu spanzuratoare
            printHangman(remainingAttempts, maxAttempts);
            // Afisare tabla cu litere ghicite
            printBoard(secret, guessedLetters);
            // Afisare lista literelor ghicite deja
            std::cout << "Guessed letters: ";
            for (char ch : guessedLetters) {
                std::cout << ch << ' ';
            }
            std::cout << "\n";
            // Afisare numar incercari ramase
            std::cout << "Remaining attempts: " << remainingAttempts << "\n";
            // Cerere input de la utilizator
            std::cout << "Enter a letter or guess the whole word: ";

            std::string guess;
            std::getline(std::cin, guess);
            if (guess.empty()) {
                // Input gol, cerem din nou
                std::cout << "Please enter a letter or a word.\n";
                continue;
            }

            // Curatam input-ul de caractere non-alfabetice si convertim la litere mici
            guess = sanitize(guess);
            if (guess.empty()) {
                // Dupa curatare, nu a ramas nimic valid
                std::cout << "Please enter alphabetic characters only.\n";
                continue;
            }

            if (guess.size() == 1) {
                // Utilizatorul a ghicit o singura litera
                char letter = guess[0];
                if (guessedLetters.count(letter)) {
                    // Litera deja ghicita anterior
                    std::cout << "You already guessed '" << letter << "'. Try another.\n";
                    continue;
                }

                // Adaugam litera in setul ghicit
                guessedLetters.insert(letter);
                if (secret.find(letter) != std::string::npos) {
                    // Litera exista in cuvant, ghicire buna
                    std::cout << "Good guess!\n";
                } else {
                    // Litera nu exista, scadem incercari ramase
                    remainingAttempts--;
                    std::cout << "No '" << letter << "' in the word.\n";
                }
            } else {
                // Utilizatorul a ghicit cuvantul intreg
                if (guess == secret) {
                    // Cuvant corect, adaugam toate literele in set pentru a marca ca ghicit
                    for (char ch : secret) guessedLetters.insert(ch);
                    break; // Iesim din bucla incercarilor
                }
                // Cuvant gresit, scadem incercari
                remainingAttempts--;
                std::cout << "Wrong word guess.\n";
            }
        }

        // Dupa iesirea din bucla, verificam rezultatul
        if (isWordGuessed(secret, guessedLetters)) {
            // Victorie: toate literele ghicite
            printWinBanner();
            std::cout << "The word was: " << secret << "\n";
            // Redare muzica pentru victorie folosind comanda system (ffplay pentru audio)
            system("timeout 35 ffplay -nodisp -autoexit -ss 12 -t 30 \"/home/robotica/Documentos/Marin Andrei/hangman/hangman/hangman/hangman/Drake - God s Plan.mp3\" 2>/dev/null");
        } else {
            // Infrangere: incercari epuizate
            printLoseBanner(secret);
            // Redare sunet pentru infrangere
            system("ffplay -nodisp -autoexit \"/home/robotica/Documentos/Marin Andrei/hangman/hangman/hangman/hangman/GTA V Wasted_Busted - Sound Effect (HD) [K3kFQHKE0LA].mp3\" 2>/dev/null");
        }

        // Intrebare daca vrea sa joace din nou
        std::string answer;
        std::getline(std::cin, answer);
        answer = sanitize(answer);
        if (!answer.empty() && answer[0] == 'y') {
            std::cout << "Change difficulty? (y/n): ";
            std::string changeAnswer;
            std::getline(std::cin, changeAnswer);
            changeAnswer = sanitize(changeAnswer);
            if (!changeAnswer.empty() && changeAnswer[0] == 'y') {
                difficulty = chooseDifficulty();
                wordList = getWordList(difficulty);
                maxAttempts = getMaxAttempts(difficulty);
                std::cout << "New difficulty: " << difficultyName(difficulty) << "\n";
                std::cout << "You have " << maxAttempts << " attempts.\n";
            }
            keepPlaying = true;
        } else {
            keepPlaying = false;
        }
    }

    // Mesaj de sfarsit dupa ce utilizatorul alege sa nu mai joace
    std::cout << "Thanks for playing!\n";
    return 0;
}
