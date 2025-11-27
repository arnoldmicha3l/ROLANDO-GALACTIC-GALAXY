#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
using namespace std;

struct Skill {
    string name;
    string description;
    int manaCost;
    bool isOneHitDelete;
};

class Character {
public:
    Character();
    Character(const string &name,
              const string &title,
              int maxHP,
              int maxMana,
              int baseDamage,
              const string &passiveDesc);

    void resetForNewRound();

    const string &getName() const;
    const string &getTitle() const;
    int getHP() const;
    int getMana() const;
    int getMaxHP() const;
    int getMaxMana() const;
    int getBaseDamage() const;
    const string &getPassiveDesc() const;
    const vector<Skill> &getSkills() const;

    void addSkill(const Skill &skill);
    void takeDamage(int amount);
    void heal(int amount);
    void useMana(int amount);
    bool isAlive() const;

private:
    string name;
    string title;
    int maxHP;
    int hp;
    int maxMana;
    int mana;
    int baseDamage;
    string passiveDesc;
    vector<Skill> skills;
};

#endif
