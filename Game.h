#ifndef GAME_H
#define GAME_H
#include "Character.h"
#include <vector>
#include <random>
using namespace std;


enum class GameMode { PVP, PVC };

class Game {
public:
Game();
void run();
void clearScreen();
bool confirmExit();

private:
vector<Character> roster;
std::mt19937 rng;

void initRoster();
void showIntro();
void mainMenu();
void playPVP();
void playPVC();
Character chooseCharacter(int playerNumber, bool showGrudges, int forbiddenIndex);
int chooseSkill(const Character &ch);
int getRandomInt(int min, int max);
int computeDamage(Character &attacker, Character &defender, const Skill &skill);
bool handleLowHP(Character &ch, int playerNumber, GameMode mode, bool isHuman);
void printCharacterCard(const Character &ch, int index);

};

#endif // GAME_H