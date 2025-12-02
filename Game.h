// Game.h
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include <string>
#include "Character.h"

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
    std::mt19937 rng;

    // helpers
    void clearScreen();
    void typeText(const std::string &text, int delayMs = 10);

    void showIntro();
    bool confirmExit();
    void mainMenu();
    void initRoster();

    void printCharacterCard(const Character &ch, int index);
    Character chooseCharacter(int playerNumber,
                              bool showGrudges,
                              int forbiddenIndex = -1);

    int getRandomInt(int min, int max);
    int chooseSkill(const Character &ch);
    int computeDamage(Character &attacker,
                      Character &defender,
                      const Skill &skill);

    bool handleLowHP(Character &ch,
                     int playerNumber,
                     GameMode mode,
                     bool isHuman);

    void playPVP();
    void playPVC();
};

#endif // GAME_H
