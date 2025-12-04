#include "Character.h"
#include <iostream>
#include <algorithm> // Required for std::min/std::max

// --- Constructors ---

// Default Constructor Definition
Character::Character() :
    name("Unnamed"),
    title("The Blank"),
    maxHP(100),
    hp(100),
    maxMana(50),
    mana(50),
    baseDamage(10),
    passiveDesc("None"),
    bio("A generic character."),
    grudge("No major rivals.")
{}

// Parameterized Constructor Definition
// Note: DO NOT include default values (= "") here, only in the header!
Character::Character(const std::string &name,
                     const std::string &title,
                     int maxHP,
                     int maxMana,
                     int baseDamage,
                     const std::string &passiveDesc,
                     const std::string &bio,       // No = "" here
                     const std::string &grudge)    // No = "" here
    : name(name),
      title(title),
      maxHP(maxHP),
      hp(maxHP), // Start HP equals Max HP
      maxMana(maxMana),
      mana(maxMana), // Start Mana equals Max Mana
      baseDamage(baseDamage),
      passiveDesc(passiveDesc),
      bio(bio),
      grudge(grudge)
{}

// --- Utility Method ---

void Character::resetForNewRound() {
    hp = maxHP;
    mana = maxMana;
    // Note: You may add other reset logic here if needed (e.g., clearing temporary buffs)
}

// --- Getters ---

const std::string &Character::getName() const {
    return name;
}

const std::string &Character::getTitle() const {
    return title;
}

int Character::getHP() const {
    return hp;
}

int Character::getMana() const {
    return mana;
}

int Character::getMaxHP() const {
    return maxHP;
}

int Character::getMaxMana() const {
    return maxMana;
}

int Character::getBaseDamage() const {
    return baseDamage;
}

const std::string &Character::getPassiveDesc() const {
    return passiveDesc;
}

const std::vector<Skill> &Character::getSkills() const {
    return skills;
}

const std::string &Character::getBio() const {
    return bio;
}

const std::string &Character::getGrudge() const {
    return grudge;
}

// --- Setters ---

void Character::setHP(int value) {
    // Ensure HP doesn't exceed Max HP
    hp = std::min(value, maxHP);
    // Ensure HP is not negative
    hp = std::max(hp, 0);
}

void Character::setMaxHP(int value) {
    maxHP = value;
    // Optional: You might want to update current HP if maxHP decreases
}

// --- Other Methods ---

void Character::addSkill(const Skill &skill) {
    skills.push_back(skill);
}

void Character::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) {
        hp = 0;
    }
}

void Character::heal(int amount) {
    hp += amount;
    if (hp > maxHP) {
        hp = maxHP;
    }
}

void Character::useMana(int amount) {
    mana -= amount;
    if (mana < 0) {
        mana = 0;
    }
}

bool Character::isAlive() const {
    return hp > 0;
}