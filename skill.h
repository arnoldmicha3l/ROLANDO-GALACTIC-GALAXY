#ifndef SKILL_H
#define SKILL_H

#include <string>

class Skill {
public:
    std::string name;
    std::string description;
    int manaCost;
    bool isOneHitDelete;

    Skill()
        : name(""), description(""), manaCost(0), isOneHitDelete(false) {}

    Skill(const std::string& n,
          const std::string& d,
          int cost,
          bool oneHit)
        : name(n),
          description(d),
          manaCost(cost),
          isOneHitDelete(oneHit) {}
};

#endif