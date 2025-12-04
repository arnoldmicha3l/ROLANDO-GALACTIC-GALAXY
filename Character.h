#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include "Skill.h"

class Character {
private:
    std::string name;
    std::string title;
    int maxHP;
    int hp;
    int maxMana;
    int mana;
    int baseDamage;
    std::string passiveDesc;
    std::string bio;
    std::string grudge;
    std::vector<Skill> skills;

public:
    Character();
    Character(const std::string &name,
              const std::string &title,
              int maxHP,
              int maxMana,
              int baseDamage,
              const std::string &passiveDesc,
              const std::string &bio,
              const std::string &grudge);

    void resetForNewRound();

    const std::string &getName() const;
    const std::string &getTitle() const;
    int getHP() const;
    int getMana() const;
    int getMaxHP() const;
    int getMaxMana() const;
    int getBaseDamage() const;
    const std::string &getPassiveDesc() const;
    const std::vector<Skill> &getSkills() const;

    const std::string &getBio() const;
    const std::string &getGrudge() const;

    void setHP(int value);
    void setMaxHP(int value);

    void addSkill(const Skill &skill);
    void takeDamage(int amount);
    void heal(int amount);
    void useMana(int amount);
    bool isAlive() const;
};

#endif
