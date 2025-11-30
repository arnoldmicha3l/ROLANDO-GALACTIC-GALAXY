<<<<<<< HEAD
#include "Character.h"

using namespace std;

Character::Character()
    : name(""),
      title(""),
      maxHP(100),
      hp(100),
      maxMana(50),
      mana(50),
      baseDamage(10),
      passiveDesc(""),
      bio(""),
      grudge("") {}

Character::Character(const string &name,
                     const string &title,
                     int maxHP,
                     int maxMana,
                     int baseDamage,
                     const string &passiveDesc,
                     const string &bio,
                     const string &grudge)
    : name(name),
      title(title),
      maxHP(maxHP),
      hp(maxHP),
      maxMana(maxMana),
      mana(maxMana),
      baseDamage(baseDamage),
      passiveDesc(passiveDesc),
      bio(bio),
      grudge(grudge) {}

void Character::resetForNewRound() {
    hp = maxHP;
    mana = maxMana;
}

const string &Character::getName() const { return name; }
const string &Character::getTitle() const { return title; }
int Character::getHP() const { return hp; }
int Character::getMaxHP() const { return maxHP; }
int Character::getMana() const { return mana; }
int Character::getMaxMana() const { return maxMana; }
int Character::getBaseDamage() const { return baseDamage; }
const string &Character::getPassiveDesc() const { return passiveDesc; }
const string &Character::getBio() const { return bio; }
const string &Character::getGrudge() const { return grudge; }
const vector<Skill> &Character::getSkills() const { return skills; }

void Character::addSkill(const Skill &skill) {
    skills.push_back(skill);
}

void Character::takeDamage(int amount) {
    hp -= amount;
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

void Character::setMaxHP(int newMaxHP) {
    maxHP = newMaxHP;
    if (hp > maxHP) {
        hp = maxHP;
    }
}

void Character::setHP(int newHP) {
    hp = newHP;
    if (hp > maxHP) hp = maxHP;
}
=======
#include "Character.h"
using namespace std;

Character::Character()
    : name(""), title(""), maxHP(100), hp(100),
      maxMana(50), mana(50), baseDamage(10), passiveDesc("") {}

Character::Character(const string &name,
                     const string &title,
                     int maxHP,
                     int maxMana,
                     int baseDamage,
                     const string &passiveDesc)
    : name(name), title(title), maxHP(maxHP), hp(maxHP),
      maxMana(maxMana), mana(maxMana), baseDamage(baseDamage),
      passiveDesc(passiveDesc) {}

void Character::resetForNewRound() {
    hp = maxHP;
    mana = maxMana;
}

const string &Character::getName() const { return name; }
const string &Character::getTitle() const { return title; }
int Character::getHP() const { return hp; }
int Character::getMana() const { return mana; }
int Character::getMaxHP() const { return maxHP; }
int Character::getMaxMana() const { return maxMana; }
int Character::getBaseDamage() const { return baseDamage; }
const string &Character::getPassiveDesc() const { return passiveDesc; }
const vector<Skill> &Character::getSkills() const { return skills; }

void Character::addSkill(const Skill &skill) {
    skills.push_back(skill);
}

void Character::takeDamage(int amount) {
    hp -= amount;
}

void Character::heal(int amount) {
    hp += amount;
    if (hp > maxHP) hp = maxHP;
}

void Character::useMana(int amount) {
    mana -= amount;
    if (mana < 0) mana = 0;
}

bool Character::isAlive() const {
    return hp > 0;
}
>>>>>>> 7dafc3db74f679e99d72f5f7bbf926b64896dba3
