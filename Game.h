<<<<<<< HEAD
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
=======
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include "Character.h"
using namespace std;

enum class GameMode { PVP, PVC };

class Game {
public:
    Game();
    void run();

private:
    vector<Character> roster;
    mt19937 rng;

    void initRoster();
    void showIntro();
    void mainMenu();
    void playPVP();
    void playPVC();

    Character chooseCharacter(int playerNumber);
    int chooseSkill(const Character &ch);
    int getRandomInt(int min, int max);

    int computeDamage(Character &attacker, Character &defender, const Skill &skill);
    bool handleLowHP(Character &ch, int playerNumber, GameMode mode, bool isHuman);
    void printCharacterCard(const Character &ch, int index);
};

#endif // GAME_H
>>>>>>> 7dafc3db74f679e99d72f5f7bbf926b64896dba3
