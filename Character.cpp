#include "Character.h"
#include <iostream>
#include <utility> 

// CONSTRUCTOR 1: Default Constructor
Character::Character() :
    name(""), title(""), maxHP(0), currentHP(0), maxMana(0), currentMana(0),
    baseDamage(0), passiveDesc(""), bio(""), grudge("") {}


// CONSTRUCTOR 2: Parameterized Constructor
Character::Character(const std::string& name, const std::string& title,
                     int maxHP, int maxMana, int baseDamage,
                     const std::string& passiveDesc, const std::string& bio,
                     const std::string& grudge)
    : name(name), title(title), maxHP(maxHP), currentHP(maxHP),
      maxMana(maxMana), currentMana(maxMana), baseDamage(baseDamage),
      passiveDesc(passiveDesc), bio(bio), grudge(grudge) {}


// GETTERS 
const std::string& Character::getName() const { return name; }
const std::string& Character::getTitle() const { return title; }
int Character::getMaxHP() const { return maxHP; }
int Character::getHP() const { return currentHP; }
int Character::getMaxMana() const { return maxMana; }
int Character::getMana() const { return currentMana; }
int Character::getBaseDamage() const { return baseDamage; }
const std::string& Character::getPassiveDesc() const { return passiveDesc; }
const std::string& Character::getBio() const { return bio; }
const std::string& Character::getGrudge() const { return grudge; }
const std::vector<Skill>& Character::getSkills() const { return skills; }
bool Character::isAlive() const { return currentHP > 0; }


// SETTERS & COMBAT METHODS
void Character::setMaxHP(int newMax) { maxHP = newMax; }

void Character::setHP(int newHP) { 
    currentHP = newHP;
    // Ensure HP doesn't go over max HP when setting externally
    if(currentHP > maxHP) {
        currentHP = maxHP;
    }
}

void Character::addSkill(const Skill& skill) { 
    skills.push_back(skill); 
}

void Character::takeDamage(int damage) {
    currentHP -= damage;
    if (currentHP < 0) {
        currentHP = 0;
    }
}

void Character::useMana(int manaUsed) {
    currentMana -= manaUsed;
    if (currentMana < 0) {
        currentMana = 0;
    }
}

void Character::heal(int amount) {
    currentHP += amount;
    if (currentHP > maxHP) {
        currentHP = maxHP;
    }
}

void Character::resetForNewRound() {
    currentHP = maxHP;
    currentMana = maxMana;
}