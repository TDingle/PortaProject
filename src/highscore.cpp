#include "highscore.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#define HIGHSCORE_FILE "highscore.txt"
static int highScore = -1;

bool is_number(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

int GetHighScore() {
    return std::max(0, highScore);
}

// called when our score changes
void OnScoreChange(int newScore) {
    static std::fstream hsFile;
    if (highScore == -1) {
        // we dont know the highscore, read it in from the highscore file
        // this only happens once the first time this func is called which is why i open a
        // file handle inline here
        std::ifstream t;
        t.open(HIGHSCORE_FILE, std::ios::in);
        if (t.is_open()) {
            std::stringstream buffer;
            buffer << t.rdbuf();
            if (is_number(buffer.str())) {
                highScore = std::stoi(buffer.str());
            }
        }
        t.close();
    }
    if (newScore > highScore) {
        highScore = newScore;
        hsFile.open(HIGHSCORE_FILE, std::ios::out | std::ios::trunc);
        hsFile << std::to_string(newScore);
        hsFile.close();
    }
    std::cout << "Highscore: " << highScore << "\n";
}