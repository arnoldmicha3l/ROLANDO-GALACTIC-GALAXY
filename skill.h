#ifndef SKILL_H

#define SKILL_H



#include <string>



class Skill {

public:

    std::string name;          // Skill name

    std::string description;   // Skill description

    int manaCost;              // Mana cost to use the skill

    bool isOneHitDelete;       // True if skill is an instant KO / no mana required



    Skill() : name(""), description(""), manaCost(0), isOneHitDelete(false) {}

    Skill(const std::string& name,

          const std::string& description,

          int manaCost,

          bool isOneHitDelete)

        : name(name),

          description(description),

          manaCost(manaCost),

          isOneHitDelete(isOneHitDelete) {}

};



#endif