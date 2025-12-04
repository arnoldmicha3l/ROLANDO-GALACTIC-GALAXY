#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include "Skill.h" // Includes the Skill class/struct

class Character {
private:
    std::string name;
    std::string title;
    int maxHP;
    int currentHP;
    int maxMana;
    int currentMana;
    int baseDamage;
    std::string passiveDesc;
    std::string bio;
    std::string grudge;
    std::vector<Skill> skills;

public:
    // Constructors (Declarations)
    Character(); 
    Character(const std::string& name, const std::string& title, int maxHP, int maxMana, 
              int baseDamage, const std::string& passiveDesc, 
              const std::string& bio, const std::string& grudge);

    // Getters (Declarations matching C.cpp return types)
    const std::string& getName() const;
    const std::string& getTitle() const;
    int getMaxHP() const;
    int getHP() const;
    int getMaxMana() const;
    int getMana() const;
    int getBaseDamage() const;
    const std::string& getPassiveDesc() const;
    const std::string& getBio() const;
    const std::string& getGrudge() const;
    const std::vector<Skill>& getSkills() const;
    bool isAlive() const;

    // Setters / Combat Methods (Declarations)
    void setMaxHP(int newMax);
    void setHP(int newHP);
    void addSkill(const Skill& skill);
    void takeDamage(int damage);
    void useMana(int manaUsed);
    void heal(int amount);
    void resetForNewRound();
};

#endif // CHARACTER_H