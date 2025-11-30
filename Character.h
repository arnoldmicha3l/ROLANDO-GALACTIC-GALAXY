<<<<<<< HEAD
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>

struct Skill {
    std::string name;
    std::string description;
    int manaCost;
    bool isOneHitDelete;
};

class Character {
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
    int getMaxHP() const;
    int getMana() const;
    int getMaxMana() const;
    int getBaseDamage() const;
    const std::string &getPassiveDesc() const;
    const std::string &getBio() const;
    const std::string &getGrudge() const;
    const std::vector<Skill> &getSkills() const;

    void addSkill(const Skill &skill);
    void takeDamage(int amount);
    void heal(int amount);
    void useMana(int amount);
    bool isAlive() const;

    // setters for PVC buffed HP
    void setMaxHP(int newMaxHP);
    void setHP(int newHP);

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
};

#endif // CHARACTER_H
=======
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
>>>>>>> 7dafc3db74f679e99d72f5f7bbf926b64896dba3
