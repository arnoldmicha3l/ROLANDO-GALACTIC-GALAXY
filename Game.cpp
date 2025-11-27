#include "Game.h"
#include <iostream>
#include <limits>
using namespace std;

Game::Game() {
    random_device rd;
    rng.seed(rd());
    initRoster();
}

void Game::run() {
    showIntro();
    mainMenu();
}

void Game::showIntro() {
    cout << "\n===== GALACTICA CAMPUS BRAWL =====\n";
    cout << "Welcome to the floating academy of Galactica, where legends are born.\n";
    cout << "Choose your student fighter and dominate the arena across the stars!\n\n";
}

void Game::mainMenu() {
    while (true) {
        cout << "Main Menu:\n";
        cout << "1. Start Player vs Player\n";
        cout << "2. Start Player vs Computer (Teacher AI)\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            playPVP();
        } else if (choice == 2) {
            playPVC();
        } else if (choice == 3) {
            cout << "Exiting Galactica... see you next orbit!\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
}

void Game::initRoster() {
    Character arnold("Arnold", "The Lover Boy", 100, 80, 18,
                     "Romantic Aura: Heals 5 HP every time he lands a special hit.");
    arnold.addSkill({"Heart Shot", "A focused blast of pure charm.", 10, false});
    arnold.addSkill({"Romantic Shield", "Reduces damage next turn.", 12, false});
    arnold.addSkill({"Starlit Serenade", "Big damage powered by the stars.", 18, false});
    arnold.addSkill({"Love Delete", "A forbidden love letter that erases the foe in one hit.", 40, true});

    Character kyle("Kyle", "The Master Beater", 120, 60, 20,
                   "Combo Master: 20% chance to strike twice.");
    kyle.addSkill({"Meteor Jab", "Fast galactic punches.", 10, false});
    kyle.addSkill({"Asteroid Uppercut", "Heavy single blow.", 15, false});
    kyle.addSkill({"Orbit Breaker", "Crushing combo that shakes the arena.", 20, false});
    kyle.addSkill({"Galaxy Eraser", "One clean hit that deletes the opponent.", 35, true});

    Character laurence("Laurence", "The Bitch Slayer", 110, 90, 17,
                       "Relentless: Deals +5 damage if the enemy HP is below 40.");
    laurence.addSkill({"Nebula Slash", "Cuts through space dust.", 8, false});
    laurence.addSkill({"Supernova Spin", "Spinning slash of light.", 14, false});
    laurence.addSkill({"Void Pressure", "Crushes the enemy with gravity.", 18, false});
    laurence.addSkill({"Oblivion Cut", "One-hit dimensional cut.", 45, true});

    Character timothy("Timothy", "The Trash Talker", 100, 100, 15,
                      "Mind Games: Has a 30% chance to add extra psychic damage.");
    timothy.addSkill({"Verbal Meteor", "Insults so strong they hurt.", 8, false});
    timothy.addSkill({"Psychic Echo", "Echoes in the enemy's mind.", 12, false});
    timothy.addSkill({"Galaxy Roast", "Legendary roast from across the cosmos.", 20, false});
    timothy.addSkill({"Silence of Space", "Deletes the target with a word.", 40, true});

    Character rolando("Rolando", "Galactic Slayer", 130, 70, 19,
                      "Galactic Fury: 25% chance to deal +5 bonus damage.");
    rolando.addSkill({"Comet Strike", "A fast, heavy strike.", 10, false});
    rolando.addSkill({"Black Hole Crash", "Pulls the foe into a crushing blow.", 15, false});
    rolando.addSkill({"Starfall Barrage", "Multiple hits of starlight.", 20, false});
    rolando.addSkill({"Cosmic Delete", "A legendary one-hit cosmic execution.", 40, true});

    roster.push_back(arnold);
    roster.push_back(kyle);
    roster.push_back(laurence);
    roster.push_back(timothy);
    roster.push_back(rolando);
}

void Game::printCharacterCard(const Character &ch, int index) {
    cout << index << ") " << ch.getName() << " " << ch.getTitle() << "\n";
    cout << "   HP: " << ch.getMaxHP()
         << "  Mana: " << ch.getMaxMana()
         << "  Base Damage: " << ch.getBaseDamage() << "\n";
    cout << "   Passive: " << ch.getPassiveDesc() << "\n";
}

Character Game::chooseCharacter(int playerNumber) {
    cout << "\nPlayer " << playerNumber
         << ", choose your fighter from Galactica Academy:\n";
    for (size_t i = 0; i < roster.size(); ++i) {
        printCharacterCard(roster[i], static_cast<int>(i) + 1);
    }

    int choice = 0;
    while (true) {
        cout << "Enter choice (1-" << roster.size() << "): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (choice >= 1 && static_cast<size_t>(choice) <= roster.size()) {
            Character chosen = roster[choice - 1];
            chosen.resetForNewRound();
            return chosen;
        }
        cout << "Invalid choice. Try again.\n";
    }
}

int Game::chooseSkill(const Character &ch) {
    const auto &skills = ch.getSkills();
    cout << "Choose a skill for " << ch.getName()
         << " " << ch.getTitle() << ":\n";
    for (size_t i = 0; i < skills.size(); ++i) {
        cout << i + 1 << ") " << skills[i].name
             << " (Mana: " << skills[i].manaCost << ") - "
             << skills[i].description;
        if (skills[i].isOneHitDelete) {
            cout << " [ONE-HIT DELETE]";
        }
        cout << "\n";
    }

    int randomSkillIndex = getRandomInt(0, static_cast<int>(skills.size()) - 1);
    cout << "Randomizer suggests skill #" << randomSkillIndex + 1 << " this turn.\n";

    int choice = 0;
    while (true) {
        cout << "Enter skill number (1-" << skills.size() << "): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (choice >= 1 && static_cast<size_t>(choice) <= skills.size()) {
            return choice - 1;
        }
        cout << "Invalid skill. Try again.\n";
    }
}

int Game::getRandomInt(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

int Game::computeDamage(Character &attacker, Character &defender,
                        const Skill &skill) {
    // One-hit delete logic: if this skill is used, it just deletes the enemy
    if (skill.isOneHitDelete) {
        cout << "*** ONE-HIT DELETE ACTIVATED! ***\n";
        return defender.getHP(); // enough to drop to 0
    }

    int damage = attacker.getBaseDamage();
    damage += getRandomInt(-3, 5); // small random variation

    // Passives
    if (attacker.getName() == "Arnold") {
        if (getRandomInt(1, 100) <= 25) {
            cout << "Passive triggered: Romantic Aura! Extra 5 damage.\n";
            damage += 5;
            attacker.heal(5);
            cout << attacker.getName() << " heals 5 HP from charm.\n";
        }
    } else if (attacker.getName() == "Kyle") {
        if (getRandomInt(1, 100) <= 20) {
            cout << "Passive triggered: Combo Master! Extra hit.\n";
            damage *= 2;
        }
    } else if (attacker.getName() == "Laurence") {
        if (defender.getHP() < 40) {
            cout << "Passive triggered: Relentless! +5 damage.\n";
            damage += 5;
        }
    } else if (attacker.getName() == "Timothy") {
        if (getRandomInt(1, 100) <= 30) {
            cout << "Passive triggered: Mind Games! Extra 5 psychic damage.\n";
            damage += 5;
        }
    } else if (attacker.getName() == "Rolando") {
        if (getRandomInt(1, 100) <= 25) {
            cout << "Passive triggered: Galactic Fury! +5 damage.\n";
            damage += 5;
        }
    }

    if (damage < 0) damage = 0;
    return damage;
}

bool Game::handleLowHP(Character &ch, int playerNumber,
                       GameMode mode, bool isHuman) {
    (void)mode; // reserved for future use

    if (!isHuman) {
        return true; // AI never surrenders
    }

    if (ch.getHP() <= 15 && ch.getHP() > 0) {
        cout << "\nWarning: Player " << playerNumber
             << " has " << ch.getHP()
             << " HP left. Continue the battle or surrender?\n";
        cout << "1. Continue\n2. Surrender\nChoose: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }
        if (choice == 2) {
            cout << "Player " << playerNumber << " has surrendered!\n";
            return false; // surrendered
        }
    }
    return true;
}

void Game::playPVP() {
    cout << "\n=== PLAYER VS PLAYER: GALACTICA DUEL ===\n";
    Character p1 = chooseCharacter(1);
    Character p2 = chooseCharacter(2);

    int p1Wins = 0;
    int p2Wins = 0;
    const int roundsToWin = 3; // best of 5

    for (int round = 1;
         round <= 5 && p1Wins < roundsToWin && p2Wins < roundsToWin;
         ++round) {

        cout << "\n--- ROUND " << round << " ---\n";
        p1.resetForNewRound();
        p2.resetForNewRound();

        while (p1.isAlive() && p2.isAlive()) {
            cout << "\n[Status] P1 " << p1.getName()
                 << " HP: " << p1.getHP()
                 << " Mana: " << p1.getMana()
                 << " | P2 " << p2.getName()
                 << " HP: " << p2.getHP()
                 << " Mana: " << p2.getMana() << "\n";

            // Player 1 turn
            if (!handleLowHP(p1, 1, GameMode::PVP, true)) {
                p2Wins++;
                break;
            }
            int skillIndex1 = chooseSkill(p1);
            const Skill &s1 = p1.getSkills()[skillIndex1];
            if (p1.getMana() < s1.manaCost) {
                cout << "Not enough mana, using basic attack instead.\n";
            } else {
                p1.useMana(s1.manaCost);
            }
            int dmg1 = computeDamage(p1, p2, s1);
            cout << "Player 1 uses " << s1.name
                 << " and deals " << dmg1 << " damage!\n";
            p2.takeDamage(dmg1);
            if (!p2.isAlive()) break;

            // Player 2 turn
            if (!handleLowHP(p2, 2, GameMode::PVP, true)) {
                p1Wins++;
                break;
            }
            int skillIndex2 = chooseSkill(p2);
            const Skill &s2 = p2.getSkills()[skillIndex2];
            if (p2.getMana() < s2.manaCost) {
                cout << "Not enough mana, using basic attack instead.\n";
            } else {
                p2.useMana(s2.manaCost);
            }
            int dmg2 = computeDamage(p2, p1, s2);
            cout << "Player 2 uses " << s2.name
                 << " and deals " << dmg2 << " damage!\n";
            p1.takeDamage(dmg2);
        }

        if (p1.isAlive() && !p2.isAlive()) {
            cout << "\nPlayer 1 wins Round " << round << "!\n";
            p1Wins++;
        } else if (!p1.isAlive() && p2.isAlive()) {
            cout << "\nPlayer 2 wins Round " << round << "!\n";
            p2Wins++;
        }

        cout << "Score: Player 1 - " << p1Wins
             << " | Player 2 - " << p2Wins << "\n";
    }

    cout << "\n=== MATCH RESULT ===\n";
    if (p1Wins > p2Wins) {
        cout << "Player 1 is the champion of Galactica!\n";
    } else if (p2Wins > p1Wins) {
        cout << "Player 2 is the champion of Galactica!\n";
    } else {
        cout << "It's a draw across the stars.\n";
    }
}

void Game::playPVC() {
    cout << "\n=== PLAYER VS TEACHER AI: GALACTICA TRIAL ===\n";
    Character player = chooseCharacter(1);

    int aiIndex = getRandomInt(0, static_cast<int>(roster.size()) - 1);
    Character ai = roster[aiIndex];
    ai.resetForNewRound();
    cout << "The Teacher AI appears as: "
         << ai.getName() << " " << ai.getTitle() << "!\n";

    int playerWins = 0;
    int aiWins = 0;
    const int roundsToWin = 3;

    for (int round = 1;
         round <= 5 && playerWins < roundsToWin && aiWins < roundsToWin;
         ++round) {

        cout << "\n--- ROUND " << round << " ---\n";
        player.resetForNewRound();
        ai.resetForNewRound();

        while (player.isAlive() && ai.isAlive()) {
            cout << "\n[Status] YOU " << player.getName()
                 << " HP: " << player.getHP()
                 << " Mana: " << player.getMana()
                 << " | TEACHER AI " << ai.getName()
                 << " HP: " << ai.getHP()
                 << " Mana: " << ai.getMana() << "\n";

            // Player turn
            if (!handleLowHP(player, 1, GameMode::PVC, true)) {
                aiWins++;
                break;
            }
            int skillIndexPlayer = chooseSkill(player);
            const Skill &sp = player.getSkills()[skillIndexPlayer];
            if (player.getMana() < sp.manaCost) {
                cout << "Not enough mana, using basic attack instead.\n";
            } else {
                player.useMana(sp.manaCost);
            }
            int dmgP = computeDamage(player, ai, sp);
            cout << "You use " << sp.name
                 << " and deal " << dmgP << " damage!\n";
            ai.takeDamage(dmgP);
            if (!ai.isAlive()) break;

            // AI turn
            int aiSkillIndex = getRandomInt(
                0, static_cast<int>(ai.getSkills().size()) - 1);
            const Skill &sa = ai.getSkills()[aiSkillIndex];
            if (ai.getMana() >= sa.manaCost) {
                ai.useMana(sa.manaCost);
            }
            int dmgA = computeDamage(ai, player, sa);
            cout << "Teacher AI uses " << sa.name
                 << " and deals " << dmgA << " damage!\n";
            player.takeDamage(dmgA);
        }

        if (player.isAlive() && !ai.isAlive()) {
            cout << "\nYou win Round " << round << "!\n";
            playerWins++;
        } else if (!player.isAlive() && ai.isAlive()) {
            cout << "\nTeacher AI wins Round " << round << "!\n";
            aiWins++;
        }

        cout << "Score: YOU - " << playerWins
             << " | TEACHER AI - " << aiWins << "\n";
    }

    cout << "\n=== MATCH RESULT ===\n";
    if (playerWins > aiWins) {
        cout << "You have passed the Galactica Trial!\n";
    } else if (aiWins > playerWins) {
        cout << "The Teacher AI has dominated this exam...\n";
    } else {
        cout << "It's a cosmic draw.\n";
    }
}
