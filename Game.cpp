#include <windows.h>
#ifdef _WIN32
#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <cctype>
#include <random>
#endif

using namespace std;

Game::Game() {
    std::random_device rd;
    rng.seed(rd());
    initRoster();
}

void Game::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}


void Game::run() {
    clearScreen();
    showIntro();
    mainMenu();
}

void Game::showIntro() {
    cout << "==================================================\n";
    cout << "          ROLANDO GALACTIC GRAVEYARD                  \n";
    cout << "==================================================\n\n";

    cout << "  Welcome to Rolando Galactic Graveyard, a floating school\n";
    cout << "  in deep space where students settle their rivalries\n";
    cout << "  in the legendary battle arena.\n\n";

    cout << "  Choose your fighter, unleash your skills,\n";
    cout << "  and prove who rules the stars.\n\n";

    cout << "  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    clearScreen();
}

bool Game::confirmExit() {
    while (true) {
        cout << "\nAre you sure to exit the game? Type yes/no: ";
        string answer;
        cin >> answer;

        for (char &c : answer) {
            c = static_cast<char>(tolower(c));
        }

        if (answer == "yes") {
            return true;
        } else if (answer == "no") {
            return false;
        } else {
            cout << "Please type yes or no.\n";
        }
    }
}

void Game::mainMenu() {
    while (true) {
        cout << "================ MAIN MENU ================\n";
        cout << "  1. Player vs Player\n";
        cout << "  2. Player vs Computer\n";
        cout << "  3. Exit Game\n";
        cout << "===========================================\n";
        cout << "Enter choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            continue;
        }

        clearScreen();

        if (choice == 1) {
            playPVP();
        } else if (choice == 2) {
            playPVC();
        } else if (choice == 3) {
            if (confirmExit()) {
                clearScreen();
                cout << "Exiting Galactica Campus Brawl...\n";
                cout << "See you next orbit, cadet.\n\n";
                break;
            } else {
                clearScreen();
            }
        } else {
            cout << "Invalid choice. Please try again.\n\n";
        }
    }
}

void Game::initRoster() {
    // Arnold
    Character arnold(
        "Arnold",
        "The Lover Boy",
        100,
        80,
        18,
        "Romantic Aura: Heals 5 HP whenever his special charm lands.",
        "Arnold used to write love letters to half the class, "
        "but one viral rejection turned him into a legend of heartbreak.",
        "Arnold holds grudges against Kyle for mocking his love notes, "
        "Timothy for roasting his failed confessions, "
        "and Rolando for stealing the spotlight at every school event."
    );
    arnold.addSkill({"Heart Shot", "A focused blast of pure charm.", 10, false});
    arnold.addSkill({"Romantic Shield", "Protects his heart and softens incoming blows.", 12, false});
    arnold.addSkill({"Starlit Serenade", "A cosmic love song that hits harder in the dark of space.", 18, false});
    // ONE-HIT DELETE: no mana required
    arnold.addSkill({"Love Delete", "A forbidden letter that deletes the enemy from his story.", 0, true});

    // Kyle
    Character kyle(
        "Kyle",
        "The Master Beater",
        120,
        60,
        20,
        "Combo Master: 20% chance to double his attack.",
        "Kyle dominates every training exam and combat class, "
        "famous for finishing practice fights before the timer starts.",
        "Kyle cannot stand Arnold's drama, Laurence's attitude, "
        "and Timothy's trash talk. He swore to beat them all in the arena."
    );
    kyle.addSkill({"Meteor Jab", "Fast galactic punches straight to the jaw.", 10, false});
    kyle.addSkill({"Asteroid Uppercut", "A heavy uppercut that launches rivals skyward.", 15, false});
    kyle.addSkill({"Orbit Breaker", "A ruthless combo that breaks enemy rhythm.", 20, false});
    // ONE-HIT DELETE: no mana required
    kyle.addSkill({"Galaxy Eraser", "A clean, brutal hit that erases the opponent.", 0, true});

    // Laurence
    Character laurence(
        "Laurence",
        "The Bitch Slayer",
        110,
        90,
        17,
        "Relentless: Deals +5 damage against low HP opponents.",
        "Laurence was once quiet, until everyone pushed too far. "
        "Now no one dares cross him twice.",
        "Laurence has history with everyone. He remembers every insult "
        "from Timothy, every flex from Kyle, and every dramatic scene from Arnold."
    );
    laurence.addSkill({"Nebula Slash", "A sharp strike that cuts through space dust.", 8, false});
    laurence.addSkill({"Supernova Spin", "A spinning slash powered by starfire.", 14, false});
    laurence.addSkill({"Void Pressure", "Gravity crushes the enemy from all sides.", 18, false});
    // ONE-HIT DELETE: no mana required
    laurence.addSkill({"Oblivion Cut", "A one-hit dimensional cut that sends foes into nothingness.", 0, true});

    // Timothy
    Character timothy(
        "Timothy",
        "The Trash Talker",
        100,
        100,
        15,
        "Mind Games: 30% chance to add extra psychic damage.",
        "Timothy never throws the first punch, just the first insult. "
        "Most battles start because of his mouth.",
        "Timothy roasts everyone: Arnold's heartbreak, Kyle's ego, "
        "Laurence's temper, and Rolando's dramatic entrances."
    );
    timothy.addSkill({"Verbal Meteor", "Insults that land like falling rocks.", 8, false});
    timothy.addSkill({"Psychic Echo", "Words that echo painfully in the mind.", 12, false});
    timothy.addSkill({"Galaxy Roast", "An interstellar insult that burns pride and HP.", 20, false});
    // ONE-HIT DELETE: no mana required
    timothy.addSkill({"Silence of Space", "One cursed sentence that deletes the target.", 0, true});

    // Rolando
    Character rolando(
        "Rolando",
        "Galactic Slayer",
        130,
        70,
        19,
        "Galactic Fury: 25% chance to deal +5 bonus damage.",
        "Rolando is a transfer student rumored to have cleared whole simulations alone.",
        "Rolando feels above petty drama, but deep down he is tired of Kyle's bragging, "
        "Timothy's comments, and being treated like a final boss by everyone."
    );
    rolando.addSkill({"Comet Strike", "A charged strike with comet-level impact.", 10, false});
    rolando.addSkill({"Black Hole Crash", "Pulls the foe in before landing a crushing blow.", 15, false});
    rolando.addSkill({"Starfall Barrage", "A barrage of starlight strikes from above.", 20, false});
    // ONE-HIT DELETE: no mana required
    rolando.addSkill({"Cosmic Delete", "Erases the enemy from the galactic record.", 0, true});

    roster.clear();
    roster.push_back(arnold);
    roster.push_back(kyle);
    roster.push_back(laurence);
    roster.push_back(timothy);
    roster.push_back(rolando);
}

void Game::printCharacterCard(const Character &ch, int index) {
    cout << "  " << index << ") "
         << ch.getName() << " " << ch.getTitle() << "\n";
    cout << "     HP: " << ch.getMaxHP()
         << "  Mana: " << ch.getMaxMana()
         << "  Base Damage: " << ch.getBaseDamage() << "\n";
    cout << "     Passive: " << ch.getPassiveDesc() << "\n\n";
}

Character Game::chooseCharacter(int playerNumber,
                                bool showGrudges,
                                int forbiddenIndex) {
    while (true) {
        cout << "=============== PLAYER " << playerNumber << " ===============\n";
        cout << "Choose your fighter from Galactica Academy:\n\n";

        for (size_t i = 0; i < roster.size(); ++i) {
            int idx = static_cast<int>(i);
            cout << "  " << idx + 1 << ") "
                 << roster[i].getName()
                 << " " << roster[i].getTitle();
            if (idx == forbiddenIndex) {
                cout << "  [TAKEN]";
            }
            cout << "\n";
        }

        cout << "\nEnter choice (1-" << roster.size() << "): ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            continue;
        }

        int index = choice - 1;
        if (index < 0 || index >= static_cast<int>(roster.size())) {
            clearScreen();
            cout << "Invalid choice. Please try again.\n\n";
            continue;
        }

        if (index == forbiddenIndex) {
            clearScreen();
            cout << "That character is already chosen by the other player.\n";
            cout << "Please select another fighter.\n\n";
            continue;
        }

        Character chosen = roster[index];
        chosen.resetForNewRound();

        clearScreen();
        cout << "============================================\n";
        cout << "Player " << playerNumber << " chose:\n\n";
        cout << "  " << chosen.getName() << " " << chosen.getTitle() << "\n\n";
        cout << "  Story:\n";
        cout << "  " << chosen.getBio() << "\n\n";

        if (showGrudges) {
            cout << "  Grudges:\n";
            cout << "  " << chosen.getGrudge() << "\n\n";
        }

        // Skill damage preview (shown once after selecting)
        cout << "  Skills & Damage Preview:\n\n";
        const vector<Skill> &skills = chosen.getSkills();
        int minDmg = chosen.getBaseDamage() - 3;
        int maxDmg = chosen.getBaseDamage() + 5;

        for (size_t i = 0; i < skills.size(); ++i) {
            cout << "   " << i + 1 << ") " << skills[i].name
                 << "  (Mana: " << skills[i].manaCost << ")\n";
            cout << "      " << skills[i].description << "\n";
            if (skills[i].isOneHitDelete) {
                cout << "      Damage: ONE-HIT DELETE (no mana needed).\n\n";
            } else {
                cout << "      Estimated Damage: around "
                     << minDmg << " to " << maxDmg
                     << " before passives.\n\n";
            }
        }

        cout << "Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        clearScreen();
        return chosen;
    }
}

int Game::getRandomInt(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

int Game::chooseSkill(const Character &ch) {
    const vector<Skill> &skills = ch.getSkills();

    cout << "Select a skill for " << ch.getName()
         << " " << ch.getTitle() << ":\n\n";

    for (size_t i = 0; i < skills.size(); ++i) {
        cout << "  " << i + 1 << ") " << skills[i].name
             << "  (Mana: " << skills[i].manaCost << ")\n";
        cout << "     " << skills[i].description;
        if (skills[i].isOneHitDelete) {
            cout << "  [ONE-HIT DELETE]";
        }
        cout << "\n\n";
    }

    int randomIndex = getRandomInt(0, static_cast<int>(skills.size()) - 1);
    cout << "  Randomizer suggests: Skill #" << randomIndex + 1 << "\n\n";

    int choice;
    while (true) {
        cout << "Enter skill number (1-" << skills.size() << "): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }
        if (choice >= 1 && choice <= static_cast<int>(skills.size())) {
            break;
        }
        cout << "Invalid skill. Please try again.\n";
    }

    system("cls");
    return choice - 1;
}

int Game::computeDamage(Character &attacker,
                        Character &defender,
                        const Skill &skill) {
    // Handle one-hit delete (NO MANA REQUIRED)
    if (skill.isOneHitDelete) {
        cout << "*** ONE-HIT DELETE ACTIVATED! ***\n";
        cout << "  " << attacker.getName()
             << " uses " << skill.name
             << " to erase the opponent in a single strike.\n\n";
        return defender.getHP(); // enough to bring HP to 0
    }

    int base = attacker.getBaseDamage();
    int randomBonus = getRandomInt(-3, 5);
    int damage = base + randomBonus;

    cout << "  Damage roll: base " << base
         << " + random " << randomBonus
         << " = " << damage << " before passives.\n";

    // Passives
    if (attacker.getName() == "Arnold") {
        if (getRandomInt(1, 100) <= 25) {
            cout << "  Passive triggered: Romantic Aura! Extra 5 damage.\n";
            damage += 5;
            attacker.heal(5);
            cout << "  " << attacker.getName()
                 << " heals 5 HP from his charm.\n";
        }
    } else if (attacker.getName() == "Kyle") {
        if (getRandomInt(1, 100) <= 20) {
            cout << "  Passive triggered: Combo Master! Extra hit.\n";
            damage *= 2;
        }
    } else if (attacker.getName() == "Laurence") {
        if (defender.getHP() < 40) {
            cout << "  Passive triggered: Relentless! +5 damage.\n";
            damage += 5;
        }
    } else if (attacker.getName() == "Timothy") {
        if (getRandomInt(1, 100) <= 30) {
            cout << "  Passive triggered: Mind Games! Extra 5 psychic damage.\n";
            damage += 5;
        }
    } else if (attacker.getName() == "Rolando") {
        if (getRandomInt(1, 100) <= 25) {
            cout << "  Passive triggered: Galactic Fury! +5 damage.\n";
            damage += 5;
        }
    }

    if (damage < 0) {
        damage = 0;
    }

    cout << "  Final damage after passives: " << damage << "\n\n";
    return damage;
}

bool Game::handleLowHP(Character &ch,
                       int playerNumber,
                       GameMode mode,
                       bool isHuman) {
    (void)mode; // not used now, but kept for future

    if (!isHuman) {
        return true; // Computer never surrenders
    }

    if (ch.getHP() <= 15 && ch.getHP() > 0) {
        cout << "\nWarning: Player " << playerNumber
             << " has only " << ch.getHP() << " HP left.\n";
        cout << "Do you want to continue the battle or surrender?\n";
        cout << "  1. Continue\n";
        cout << "  2. Surrender\n";
        cout << "Choose: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }

        if (choice == 2) {
            cout << "\nPlayer " << playerNumber << " has surrendered!\n\n";
            return false;
        }
    }
    return true;
}

void Game::playPVP() {
    clearScreen();

    cout << "=============== PLAYER VS PLAYER ===============\n\n";

    Character p1 = chooseCharacter(1, true, -1);

    // Find index of Player 1's character in roster so Player 2 can't pick it
    int forbiddenIndex = -1;
    for (size_t i = 0; i < roster.size(); ++i) {
        if (roster[i].getName() == p1.getName() &&
            roster[i].getTitle() == p1.getTitle()) {
            forbiddenIndex = static_cast<int>(i);
            break;
        }
    }

    Character p2 = chooseCharacter(2, true, forbiddenIndex);

    int p1Wins = 0;
    int p2Wins = 0;
    const int roundsToWin = 3;

    for (int round = 1;
         round <= 5 && p1Wins < roundsToWin && p2Wins < roundsToWin;
         ++round) {

        p1.resetForNewRound();
        p2.resetForNewRound();

        clearScreen();
        cout << "================= ROUND " << round << " =================\n\n";

        bool roundOver = false;

        while (p1.isAlive() && p2.isAlive() && !roundOver) {

            cout << "STATUS\n";
            cout << "  Player 1: " << p1.getName()
                 << "  HP: " << p1.getHP()
                 << "  Mana: " << p1.getMana() << "\n";
            cout << "  Player 2: " << p2.getName()
                 << "  HP: " << p2.getHP()
                 << "  Mana: " << p2.getMana() << "\n\n";

            // Player 1 turn
            if (!handleLowHP(p1, 1, GameMode::PVP, true)) {
                p2Wins++;
                roundOver = true;
                break;
            }

            cout << "------------- PLAYER 1 TURN -------------\n\n";
            int skillIndex1 = chooseSkill(p1);
            const Skill &s1 = p1.getSkills()[skillIndex1];

            if (p1.getMana() < s1.manaCost) {
                cout << "Not enough mana for " << s1.name
                     << ". Using basic attack instead.\n\n";
            } else {
                p1.useMana(s1.manaCost);
            }

            int dmg1 = computeDamage(p1, p2, s1);
            cout << "Player 1 uses " << s1.name
                 << " and deals " << dmg1 << " damage!\n\n";
            p2.takeDamage(dmg1);

            cout << "Remaining HP and Mana:\n";
            cout << "  Player 1 - HP: " << p1.getHP()
                 << " | Mana: " << p1.getMana() << "\n";
            cout << "  Player 2 - HP: " << p2.getHP()
                 << " | Mana: " << p2.getMana() << "\n\n";

            if (!p2.isAlive()) {
                break;
            }

            // Player 2 turn
            if (!handleLowHP(p2, 2, GameMode::PVP, true)) {
                p1Wins++;
                roundOver = true;
                break;
            }

            cout << "------------- PLAYER 2 TURN -------------\n\n";
            int skillIndex2 = chooseSkill(p2);
            const Skill &s2 = p2.getSkills()[skillIndex2];

            if (p2.getMana() < s2.manaCost) {
                cout << "Not enough mana for " << s2.name
                     << ". Using basic attack instead.\n\n";
            } else {
                p2.useMana(s2.manaCost);
            }

            int dmg2 = computeDamage(p2, p1, s2);
            cout << "Player 2 uses " << s2.name
                 << " and deals " << dmg2 << " damage!\n\n";
            p1.takeDamage(dmg2);

            cout << "Remaining HP and Mana:\n";
            cout << "  Player 1 - HP: " << p1.getHP()
                 << " | Mana: " << p1.getMana() << "\n";
            cout << "  Player 2 - HP: " << p2.getHP()
                 << " | Mana: " << p2.getMana() << "\n\n";
        }

        if (p1.isAlive() && !p2.isAlive()) {
            cout << "Player 1 wins Round " << round << "!\n\n";
            p1Wins++;
        } else if (!p1.isAlive() && p2.isAlive()) {
            cout << "Player 2 wins Round " << round << "!\n\n";
            p2Wins++;
        }

        cout << "Scoreboard:\n";
        cout << "  Player 1: " << p1Wins << " wins\n";
        cout << "  Player 2: " << p2Wins << " wins\n\n";

        if (round < 5 && p1Wins < roundsToWin && p2Wins < roundsToWin) {
            cout << "Press Enter to proceed to the next round...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }
    }

    cout << "================ MATCH RESULT ================\n\n";
    if (p1Wins > p2Wins) {
        cout << "Player 1 is the champion of Galactica Academy!\n\n";
    } else if (p2Wins > p1Wins) {
        cout << "Player 2 is the champion of Galactica Academy!\n\n";
    } else {
        cout << "It ends in a cosmic draw.\n\n";
    }

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

void Game::playPVC() {
    clearScreen();

    cout << "============= PLAYER VS COMPUTER =============\n\n";

    // Bios only, no grudges in PVC
    Character player = chooseCharacter(1, false, -1);

    int aiIndex = getRandomInt(0, static_cast<int>(roster.size()) - 1);
    Character computer = roster[aiIndex];

    int playerWins = 0;
    int computerWins = 0;
    const int roundsToWin = 3;

    cout << "The Computer has appeared as:\n\n";
    cout << "  " << computer.getName()
         << " " << computer.getTitle() << "\n\n";
    cout << "Press Enter to begin the trial...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    for (int round = 1;
         round <= 5 && playerWins < roundsToWin && computerWins < roundsToWin;
         ++round) {

        player.resetForNewRound();
        computer.resetForNewRound();

        // Boost Computer HP to 200 each round
        computer.setMaxHP(200);
        computer.setHP(200);

        clearScreen();
        cout << "================= ROUND " << round << " =================\n\n";

        while (player.isAlive() && computer.isAlive()) {

            cout << "STATUS\n";
            cout << "  You: " << player.getName()
                 << "  HP: " << player.getHP()
                 << "  Mana: " << player.getMana() << "\n";
            cout << "  Computer: " << computer.getName()
                 << "  HP: " << computer.getHP()
                 << "  Mana: " << computer.getMana() << "\n\n";

            // Player turn (can surrender)
            if (!handleLowHP(player, 1, GameMode::PVC, true)) {
                computerWins++;
                break;
            }

            cout << "------------- YOUR TURN -------------\n\n";
            int skillIndexPlayer = chooseSkill(player);
            const Skill &sp = player.getSkills()[skillIndexPlayer];

            if (player.getMana() < sp.manaCost) {
                cout << "Not enough mana for " << sp.name
                     << ". Using basic attack instead.\n\n";
            } else {
                player.useMana(sp.manaCost);
            }

            int dmgP = computeDamage(player, computer, sp);
            cout << "You use " << sp.name
                 << " and deal " << dmgP << " damage!\n\n";
            computer.takeDamage(dmgP);

            cout << "Remaining HP and Mana:\n";
            cout << "  You      - HP: " << player.getHP()
                 << " | Mana: " << player.getMana() << "\n";
            cout << "  Computer - HP: " << computer.getHP()
                 << " | Mana: " << computer.getMana() << "\n\n";

            if (!computer.isAlive()) {
                break;
            }

            // Computer turn (no surrender)
            cout << "------------- COMPUTER TURN -------------\n\n";
            int aiSkillIndex = getRandomInt(
                0,
                static_cast<int>(computer.getSkills().size()) - 1
            );
            const Skill &sa = computer.getSkills()[aiSkillIndex];

            if (computer.getMana() < sa.manaCost) {
                // basic attack, no mana usage
            } else {
                computer.useMana(sa.manaCost);
            }

            int dmgA = computeDamage(computer, player, sa);
            cout << "Computer uses " << sa.name
                 << " and deals " << dmgA << " damage!\n\n";
            player.takeDamage(dmgA);

            cout << "Remaining HP and Mana:\n";
            cout << "  You      - HP: " << player.getHP()
                 << " | Mana: " << player.getMana() << "\n";
            cout << "  Computer - HP: " << computer.getHP()
                 << " | Mana: " << computer.getMana() << "\n\n";
        }

        if (player.isAlive() && !computer.isAlive()) {
            cout << "You win Round " << round << "!\n\n";
            playerWins++;
        } else if (!player.isAlive() && computer.isAlive()) {
            cout << "Computer wins Round " << round << "!\n\n";
            computerWins++;
        }

        cout << "Scoreboard:\n";
        cout << "  You: " << playerWins << " wins\n";
        cout << "  Computer: " << computerWins << " wins\n\n";

        if (round < 5 && playerWins < roundsToWin && computerWins < roundsToWin) {
            cout << "Press Enter to proceed to the next round...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }
    }

    cout << "================ MATCH RESULT ================\n\n";
    if (playerWins > computerWins) {
        cout << "You have passed the Galactica trial against the Computer!\n\n";
    } else if (computerWins > playerWins) {
        cout << "The Computer has outplayed you this time.\n\n";
    } else {
        cout << "The stars show a balanced result. It's a draw.\n\n";
    }

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}
