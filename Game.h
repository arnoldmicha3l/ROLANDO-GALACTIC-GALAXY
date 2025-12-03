#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include "Character.h"
#include "skill.h"

enum class GameMode { PVP, PVC };

class Game {
public:
    Game();
    void run();
    void clearScreen();
    bool confirmExit();

private:
    std::vector<Character> roster;
    std::mt19937 rng;

    void initRoster();
    void showIntro();
    void mainMenu();
    void playPVP();
    void playPVC();
    void viewAllCharacters();
    void displayCharacterDetails(int index);
    void showCredits();
    Character chooseCharacter(int playerNumber, bool showGrudges, int forbiddenIndex);
    int chooseSkill(const Character& ch);
    int getRandomInt(int min, int max);
    int computeDamage(Character& attacker, Character& defender, const Skill& skill);
    bool handleLowHP(Character& ch, int playerNumber, GameMode mode, bool isHuman);
    void printCharacterCard(const Character& ch, int index);
};

#endif