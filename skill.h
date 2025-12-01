#ifndef SKILL_H
#define SKILL_H

#include <string>
#include <utility> // For std::move if needed, but const reference works fine here

class Skill {
public:
    std::string name;             // Skill name
    std::string description;      // Skill description
    int manaCost;                 // Mana cost to use the skill
    bool isOneHitDelete;          // True if skill is an instant KO / no mana required

    // Default Constructor
    Skill() : name(""), description(""), manaCost(0), isOneHitDelete(false) {}

    // Parameterized Constructor
    Skill(const std::string& name,
          const std::string& description,
          int manaCost,
          bool isOneHitDelete)
        : name(name),
          description(description),
          manaCost(manaCost),
          isOneHitDelete(isOneHitDelete) {}
};

// Global constant definition for the Basic Attack fallback
// The Game.cpp logic relies on this definition being available.
const Skill BASIC_ATTACK = {"Basic Attack", "A simple physical strike.", 0, false};

#endif // SKILL_H