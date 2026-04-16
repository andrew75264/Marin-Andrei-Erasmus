#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using Grid = std::array<std::array<int, 9>, 9>;

enum class Difficulty { Easy, Medium, Hard };

struct Move {
    int row;
    int col;
    int prevValue;
    int newValue;
    bool isHint;
};

std::mt19937& rng() {
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

std::string difficultyName(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy: return "Easy";
        case Difficulty::Medium: return "Medium";
        case Difficulty::Hard: return "Hard";
    }
    return "Medium";
}

int targetRemovalsForDifficulty(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy: return 36;
        case Difficulty::Medium: return 45;
        case Difficulty::Hard: return 52;
    }
    return 45;
}

bool isSafe(const Grid& board, int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; ++r) {
        for (int c = startCol; c < startCol + 3; ++c) {
            if (board[r][c] == num) {
                return false;
            }
        }
    }
    return true;
}

bool solveSudoku(Grid& board, int& solutionCount, int limit = 2) {
    int row = -1;
    int col = -1;
    int bestOptions = 10;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == 0) {
                int options = 0;
                for (int n = 1; n <= 9; ++n) {
                    if (isSafe(board, r, c, n)) {
                        ++options;
                    }
                }
                if (options < bestOptions) {
                    bestOptions = options;
                    row = r;
                    col = c;
                }
            }
        }
    }
    if (row == -1) {
        solutionCount += 1;
        return solutionCount >= limit;
    }
    for (int num = 1; num <= 9 && solutionCount < limit; ++num) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            solveSudoku(board, solutionCount, limit);
            board[row][col] = 0;
        }
    }
    return solutionCount >= limit;
}

bool generateFullGrid(Grid& board) {
    std::vector<int> values(9);
    std::iota(values.begin(), values.end(), 1);
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                std::shuffle(values.begin(), values.end(), rng());
                bool placed = false;
                for (int num : values) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (generateFullGrid(board)) {
                            placed = true;
                            break;
                        }
                    }
                }
                if (!placed) {
                    board[row][col] = 0;
                    return false;
                }
            }
        }
    }
    return true;
}

Grid createCompleteBoard() {
    Grid board{};
    for (auto& row : board) {
        row.fill(0);
    }
    generateFullGrid(board);
    return board;
}

std::vector<int> getCandidates(const Grid& board, int row, int col) {
    std::vector<int> candidates;
    if (board[row][col] != 0) {
        return candidates;
    }
    for (int value = 1; value <= 9; ++value) {
        if (isSafe(board, row, col, value)) {
            candidates.push_back(value);
        }
    }
    return candidates;
}

bool createPuzzle(const Grid& solution,
                  Grid& puzzleGrid,
                  std::array<std::array<bool, 9>, 9>& fixedCells,
                  Difficulty difficulty) {
    puzzleGrid = solution;
    for (auto& row : fixedCells) {
        row.fill(true);
    }
    std::vector<std::pair<int, int>> cells;
    cells.reserve(81);
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            cells.emplace_back(r, c);
        }
    }
    std::shuffle(cells.begin(), cells.end(), rng());
    int removed = 0;
    int targetRemovals = targetRemovalsForDifficulty(difficulty) +
                         std::uniform_int_distribution<int>(-2, 2)(rng());

    for (auto [row, col] : cells) {
        int backup = puzzleGrid[row][col];
        puzzleGrid[row][col] = 0;
        fixedCells[row][col] = false;

        Grid temp = puzzleGrid;
        int solutions = 0;
        solveSudoku(temp, solutions, 2);

        if (solutions != 1) {
            puzzleGrid[row][col] = backup;
            fixedCells[row][col] = true;
        } else {
            removed += 1;
            if (removed >= targetRemovals) {
                break;
            }
        }
    }
    return true;
}

void printGrid(const Grid& grid) {
    std::cout << "   1 2 3   4 5 6   7 8 9\n";
    for (int r = 0; r < 9; ++r) {
        if (r % 3 == 0) {
            std::cout << "  +-------+-------+-------+\n";
        }
        std::cout << char('A' + r) << " |";
        for (int c = 0; c < 9; ++c) {
            if (grid[r][c] == 0) {
                std::cout << " .";
            } else {
                std::cout << ' ' << grid[r][c];
            }
            if ((c + 1) % 3 == 0) {
                std::cout << " |";
            }
        }
        std::cout << "\n";
    }
    std::cout << "  +-------+-------+-------+\n";
}

void printCellCandidates(const Grid& board, int row, int col) {
    auto candidates = getCandidates(board, row, col);
    std::cout << "Candidates for " << char('A' + row) << (col + 1) << ": ";
    if (candidates.empty()) {
        std::cout << "none\n";
        return;
    }
    for (int value : candidates) {
        std::cout << value << ' ';
    }
    std::cout << "\n";
}

void printCandidateGrid(const Grid& board) {
    std::cout << "Full candidate grid:\n";
    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int row = blockRow * 3; row < blockRow * 3 + 3; ++row) {
            for (int subRow = 0; subRow < 3; ++subRow) {
                for (int blockCol = 0; blockCol < 3; ++blockCol) {
                    for (int col = blockCol * 3; col < blockCol * 3 + 3; ++col) {
                        if (board[row][col] != 0) {
                            if (subRow == 1) {
                                std::cout << ' ' << board[row][col] << ' ';
                            } else {
                                std::cout << "   ";
                            }
                        } else {
                            auto candidates = getCandidates(board, row, col);
                            int base = subRow * 3 + 1;
                            for (int d = base; d < base + 3; ++d) {
                                std::cout << (std::find(candidates.begin(), candidates.end(), d) != candidates.end() ? char('0' + d) : '.');
                            }
                        }
                        std::cout << (col % 3 == 2 ? "  " : " ");
                    }
                    if (blockCol < 2) {
                        std::cout << "| ";
                    }
                }
                std::cout << "\n";
            }
            if (row % 3 == 2 && row != 8) {
                std::cout << "  -------+-------+-------\n";
            }
        }
    }
}

void printStatus(Difficulty difficulty, int score, int hintsUsed, int undoCount, int streak, int elapsedSeconds) {
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;
    std::cout << "Difficulty: " << difficultyName(difficulty)
              << "   Score: " << score
              << "   Streak: " << streak
              << "   Time: " << minutes << ':' << (seconds < 10 ? "0" : "") << seconds
              << "   Hints: " << hintsUsed
              << "   Undos: " << undoCount << "\n";
}

void printHelp() {
    std::cout << "Commands:\n"
              << "  A 1 5      - place a value in row A, column 1\n"
              << "  hint / h   - reveal a single correct cell\n"
              << "  undo / u   - undo the last move or hint\n"
              << "  cand A 1   - show candidates for a cell\n"
              << "  cands / full - show the full candidate grid\n"
              << "  score      - show current score\n"
              << "  s          - show the solution\n"
              << "  q          - quit\n";
}

void playVictoryMusic() {
    std::cout << "Playing 15 seconds of Drake's \"God's Plan\"...\n";
    const std::vector<std::string> candidates = {"gods_plan.mp3", "Drake - God s Plan.mp3"};
    std::string cmd;
    for (const auto& file : candidates) {
        cmd += "if [ -f '" + file + "' ]; then ";
        cmd += "ffplay -nodisp -autoexit -t 15 '" + file + "' >/dev/null 2>&1; ";
        cmd += "exit 0; fi; ";
    }
    cmd += "if command -v ffplay >/dev/null 2>&1; then ";
    cmd += "ffplay -nodisp -autoexit -t 15 \"https://archive.org/download/drake-gods-plan/Drake%20-%20God%27s%20Plan.mp3\" >/dev/null 2>&1; ";
    cmd += "else printf 'Unable to play music. Install ffmpeg or place gods_plan.mp3 or \"Drake - God s Plan.mp3\" in the game folder.\n'; fi";
    int status = std::system(cmd.c_str());
    (void)status;
}

bool isComplete(const Grid& board) {
    for (const auto& row : board) {
        for (int n : row) {
            if (n == 0) {
                return false;
            }
        }
    }
    return true;
}

bool validateMove(const Grid& board, int row, int col, int value) {
    if (value < 1 || value > 9) {
        return false;
    }
    return isSafe(board, row, col, value);
}

bool applyMove(Grid& puzzle,
               const Grid& solution,
               int row,
               int col,
               int value,
               std::vector<Move>& history,
               int& score) {
    Move move{row, col, puzzle[row][col], value, false};
    if (!validateMove(puzzle, row, col, value)) {
        return false;
    }
    if (solution[row][col] != value) {
        std::cout << "That move is legal but not the correct solution value.\n";
        return false;
    }
    puzzle[row][col] = value;
    history.push_back(move);
    score += 25;
    return true;
}

bool undoLastMove(Grid& puzzle, std::vector<Move>& history, int& score) {
    if (history.empty()) {
        return false;
    }
    Move last = history.back();
    history.pop_back();
    puzzle[last.row][last.col] = last.prevValue;
    score = std::max(0, score - (last.isHint ? 0 : 10));
    std::cout << "Undid the last move at " << char('A' + last.row) << (last.col + 1) << ".\n";
    return true;
}

bool giveHint(Grid& puzzle,
              const Grid& solution,
              std::vector<Move>& history,
              int& hintsUsed,
              int& score) {
    std::vector<std::pair<int, int>> emptyCells;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (puzzle[r][c] == 0) {
                emptyCells.emplace_back(r, c);
            }
        }
    }
    if (emptyCells.empty()) {
        return false;
    }
    std::shuffle(emptyCells.begin(), emptyCells.end(), rng());
    auto [row, col] = emptyCells.front();
    Move hintMove{row, col, puzzle[row][col], solution[row][col], true};
    puzzle[row][col] = solution[row][col];
    history.push_back(hintMove);
    hintsUsed += 1;
    score = std::max(0, score - 75);
    std::cout << "Hint filled " << char('A' + row) << (col + 1) << " with " << solution[row][col] << ".\n";
    return true;
}

Difficulty chooseDifficulty() {
    std::cout << "Choose difficulty slider:\n"
              << "  1 - Easy  (more givens, smaller search)\n"
              << "  2 - Medium (balanced)\n"
              << "  3 - Hard  (fewer givens, more challenge)\n"
              << "Enter 1, 2, or 3 (default 2): ";
    int choice = 2;
    if (!(std::cin >> choice) || choice < 1 || choice > 3) {
        choice = 2;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (choice) {
        case 1: return Difficulty::Easy;
        case 3: return Difficulty::Hard;
        default: return Difficulty::Medium;
    }
}

int startingScore(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy: return 1000;
        case Difficulty::Medium: return 1500;
        case Difficulty::Hard: return 2000;
    }
    return 1500;
}

int main() {
    std::seed_seq seed{uint32_t(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
    rng().seed(seed);

    Difficulty difficulty = chooseDifficulty();
    int score = startingScore(difficulty);
    int hintsUsed = 0;
    int undoCount = 0;
    int streakCount = 0;

    Grid solution = createCompleteBoard();
    Grid puzzle;
    std::array<std::array<bool, 9>, 9> fixedCells;
    createPuzzle(solution, puzzle, fixedCells, difficulty);

    std::vector<Move> history;
    auto gameStart = std::chrono::steady_clock::now();
    auto lastActionTime = gameStart;

    std::cout << "Sudoku (NYT-style medium-like) - every puzzle is unique\n";
    std::cout << "Commands: A 1 5, hint, undo, cand A 1, score, s, q.\n";
    printHelp();
    std::cout << "\n";

    while (true) {
        auto now = std::chrono::steady_clock::now();
        int elapsedSeconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - gameStart).count());
        printStatus(difficulty, score, hintsUsed, undoCount, streakCount, elapsedSeconds);
        printGrid(puzzle);
        if (isComplete(puzzle)) {
            std::cout << "Congratulations! You completed the puzzle.\n";
            std::cout << "Final score: " << score << "\n";
            playVictoryMusic();
            break;
        }

        std::cout << "Move> ";
        std::string token;
        if (!(std::cin >> token)) {
            break;
        }
        if (token == "q" || token == "Q") {
            std::cout << "Goodbye!\n";
            break;
        }
        if (token == "s" || token == "S") {
            std::cout << "Solution:\n";
            printGrid(solution);
            break;
        }
        if (token == "hint" || token == "h") {
            if (!giveHint(puzzle, solution, history, hintsUsed, score)) {
                std::cout << "No empty cells available for a hint.\n";
            }
            streakCount = 0;
            lastActionTime = std::chrono::steady_clock::now();
            continue;
        }
        if (token == "undo" || token == "u") {
            if (!undoLastMove(puzzle, history, score)) {
                std::cout << "Nothing to undo.\n";
            } else {
                undoCount += 1;
            }
            streakCount = 0;
            lastActionTime = std::chrono::steady_clock::now();
            continue;
        }
        if (token == "cands" || token == "full") {
            printCandidateGrid(puzzle);
            continue;
        }
        if (token == "cand" || token == "c") {
            std::string rowToken;
            int col;
            if (!(std::cin >> rowToken >> col)) {
                std::cout << "Usage: cand A 1\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            if (rowToken.size() != 1 || rowToken[0] < 'A' || rowToken[0] > 'I') {
                std::cout << "Invalid row. Use A-I.\n";
                continue;
            }
            if (col < 1 || col > 9) {
                std::cout << "Column must be 1-9.\n";
                continue;
            }
            printCellCandidates(puzzle, rowToken[0] - 'A', col - 1);
            continue;
        }
        if (token == "score") {
            std::cout << "Current score: " << score << "\n";
            continue;
        }
        if (token.size() != 1 || token[0] < 'A' || token[0] > 'I') {
            std::cout << "Invalid command or row. Use A-I for moves or a command.\n";
            continue;
        }

        int row = token[0] - 'A';
        int col;
        int value;
        if (!(std::cin >> col >> value)) {
            std::cout << "Invalid input. Use row col value.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (col < 1 || col > 9) {
            std::cout << "Column must be 1-9.\n";
            continue;
        }
        col -= 1;
        if (fixedCells[row][col]) {
            std::cout << "That cell is fixed and cannot be changed.\n";
            continue;
        }
        auto nowMove = std::chrono::steady_clock::now();
        int secsSinceLast = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(nowMove - lastActionTime).count());
        int newStreak = secsSinceLast <= 15 ? streakCount + 1 : 1;
        if (!applyMove(puzzle, solution, row, col, value, history, score)) {
            streakCount = 0;
            lastActionTime = nowMove;
            continue;
        }
        streakCount = newStreak;
        int streakBonus = (streakCount > 1 ? std::min(streakCount - 1, 4) * 5 : 0);
        if (streakBonus > 0) {
            score += streakBonus;
            std::cout << "Streak bonus +" << streakBonus << "! (" << streakCount << " fast moves)\n";
        }
        lastActionTime = nowMove;
    }

    return 0;
}
