#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include <vector>
#include <string>
#include <random>
#include <chrono>

// Forward declarations
class Character;
struct Skill;

// ===========================================
// HISTORY STRUCTURES
// ===========================================

// Match result data structure
struct MatchResult {
    std::string mode;
    std::string p1_char;
    std::string p2_char;
    std::string winner_name;
    std::string score; // e.g., "3-1"
    long long timestamp;

    std::string getTimeString() const;
};

// ===========================================
// GAME CLASS
// ===========================================

enum class GameMode {
    PVP,
    PVC
};

class Game {
public:
    Game();
    void run();

private:
    std::vector<Character> roster;
    // History vector acts as the Queue (FIFO)
    std::vector<MatchResult> history;
    std::mt19937 rng;

    // Constants
    const char* HISTORY_FILE = "match_history.txt";
    const int MAX_HISTORY_CAPACITY = 3;

    // Core Game Flow
    void showIntro();
    void mainMenu();
    void clearScreen();
    bool confirmExit();
    void showCredits();

    // Character Management
    void initRoster();
    void viewAllCharacters();
    void displayCharacterDetails(int index);
    void typeText(const char* text, int delayMs = 30);

    // Combat & Utility
    int getRandomInt(int min, int max);
    Character chooseCharacter(int playerNumber, bool showGrudges, int forbiddenIndex = -1);
    int chooseSkill(const Character &ch);
    int computeDamage(Character &attacker, Character &defender, const Skill &skill);
    bool handleLowHP(Character &ch, int playerNumber, GameMode mode, bool isHuman);

    // Game Modes
    void playPVP();
    void playPVC();

    // History Queue Management
    void saveMatchHistory();
    void loadMatchHistory();
    void viewMatchHistory();
};

#endif // GAME_H