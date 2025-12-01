#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include <vector>
#include <string>
#include <random>

enum class GameMode {
    PVP,
    PVC
};

struct MatchResult {
    std::string mode;
    std::string p1_char;
    std::string p2_char;
    std::string winner_name;
    std::string score;
    long long timestamp; // Time in milliseconds

    std::string getTimeString() const; // Implemented in Game.cpp
};

class Game {
private:
    std::vector<Character> roster;
    std::vector<MatchResult> history;
    std::mt19937 rng; // Random number generator engine

    // Utility Functions
    void clearScreen();
    void showIntro();
    bool confirmExit();
    void initRoster();
    void printCharacterCard(const Character &ch, int index);
    
    // Character Selection / Utility
    Character chooseCharacter(int playerNumber, bool showGrudges, int forbiddenIndex);
    int getRandomInt(int min, int max);
    
    // Combat Functions
    int chooseSkill(const Character &ch);
    int computeDamage(Character &attacker, Character &defender, const Skill &skill);
    bool handleLowHP(Character &ch, int playerNumber, GameMode mode, bool isHuman);
    
    // Menu Functions
    void viewAllCharacters();
    void displayCharacterDetails(int index);
    void showCredits();

    // History Functions
    void saveMatchHistory();
    void loadMatchHistory(); // Added for completeness

public:
    Game();
    void run();
    void mainMenu();
    void playPVP();
    void playPVC();
    void viewMatchHistory();
};

#endif // GAME_H