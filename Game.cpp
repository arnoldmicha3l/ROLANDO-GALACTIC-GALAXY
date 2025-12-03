#include <windows.h>
#include <mmsystem.h>
#ifdef _WIN32
#include "Game.h"
#include "Character.h"
#include "Skill.h"
#include <iostream>
#include <thread> 
#include <chrono> 
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <cctype>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <conio.h>
#endif

using namespace std;
#pragma comment(lib, "winmm.lib") 

// Assuming BASIC_ATTACK is defined in a global scope or Character/Skill headers
// For completeness, define a basic attack fallback here if not defined elsewhere:

const char* HISTORY_FILE = "match_history.txt";

// NOTE: MAX_HISTORY_CAPACITY must be defined in Game.h
// The MatchResult structure must also be fully defined in Game.h

// ===========================================
// CONSTRUCTOR & INITIALIZATION
// ===========================================

Game::Game() {
    std::random_device rd;
    rng.seed(rd());
    initRoster();
    loadMatchHistory(); 
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

// ===========================================
// HISTORY & UTILITY IMPLEMENTATIONS (The Queue Logic)
// ===========================================

std::string MatchResult::getTimeString() const {
    std::time_t rawtime = static_cast<std::time_t>(timestamp / 1000); 
    struct tm * timeinfo;
    char buffer[80];

    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

void Game::saveMatchHistory() {
    std::ofstream outfile(HISTORY_FILE);
    if (outfile.is_open()) {
        for (const auto& result : history) {
            outfile << result.mode << "|"
                    << result.p1_char << "|"
                    << result.p2_char << "|"
                    << result.winner_name << "|"
                    << result.score << "|"
                    << result.timestamp << "\n";
        }
        outfile.close();
    }
}

void Game::loadMatchHistory() {
    std::ifstream infile(HISTORY_FILE);
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> parts;
            while (std::getline(ss, token, '|')) {
                parts.push_back(token);
            }

            if (parts.size() == 6) {
                history.push_back({
                    parts[0], 
                    parts[1], 
                    parts[2], 
                    parts[3], 
                    parts[4], 
                    std::stoll(parts[5]) 
                });
            }
        }
        infile.close();
        
        // Queue Logic: Apply limit when loading history: if file had too many, remove oldest (FIFO).
        // Uses MAX_HISTORY_CAPACITY defined in Game.h
        while (history.size() > MAX_HISTORY_CAPACITY) {
            history.erase(history.begin());
        }
    }
}

void Game::viewMatchHistory() {
    clearScreen();
    cout << "============== MATCH HISTORY (Last " << MAX_HISTORY_CAPACITY << ") ==============\n";
    
    // **FIXED: Removed the non-C++ tag that caused the "identifier 'Image' is undefined" error.**
    // The visualization explains the FIFO (First-In, First-Out) principle.
    if (history.empty()) {
        cout << " No past cosmic brawls found in the archives.\n";
        cout << "===========================================\n\n";
    } else {
        // Print the header for the table
        cout << std::left 
             << std::setw(6) << "Mode"
             << std::setw(15) << "P1 Char"
             << std::setw(15) << "P2 Char"
             << std::setw(15) << "Winner"
             << std::setw(10) << "Score"
             << "Time\n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (const auto& result : history) {
            // Populate the table rows
            cout << std::left 
                 << std::setw(6) << result.mode
                 << std::setw(15) << result.p1_char
                 << std::setw(15) << result.p2_char
                 << std::setw(15) << result.winner_name
                 << std::setw(10) << result.score
                 << result.getTimeString()
                 << "\n";
        }
        cout << "==========================================================================================\n\n";
    }

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

void Game::typeText(const char* text, int delayMs) {
    bool skipThisString = false;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        cout << c << flush;

        if (skipThisString || delayMs <= 0)
            continue;

        int step = max(1, delayMs / 5);
        int elapsed = 0;

        while (elapsed < delayMs) {
            if (_kbhit()) {
                int key = _getch();
                if (key == ' ') {
                    skipThisString = true;
                    break;
                }
            }
            this_thread::sleep_for(chrono::milliseconds(step));
            elapsed += step;
        }
    }
}

// ===========================================
// MAIN MENU & INTRO
// ===========================================

void Game::showIntro() {
     PlaySound(TEXT("intro.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    cout << "==================================================\n";
    cout << "          ROLANDO GALACTIC GRAVEYARD              \n";
    cout << "==================================================\n\n";

    typeText("Welcome to Rolando Galactic Graveyard, a floating school.\n", 30);
    typeText("in deep space where students settle their rivalries.\n", 30);
    typeText("in the legendary battle arena.\n\n", 30);
    typeText("Choose your fighter, unleash your skills,\n", 30);

    typeText("Tonight, a new hunter enters the field.\n", 30);
    typeText("Choose your warrior and carve your legend among the ruins.\n\n", 30);

    typeText("Press Enter to continue...", 30);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

     PlaySound(NULL, 0, 0);
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
        PlaySound(TEXT("picking.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        cout << "================ MAIN MENU ================\n";
        cout << " 1. Player vs Player\n";
        cout << " 2. Player vs Computer\n";
        cout << " 3. View All Characters\n";
        cout << " 4. Credits\n";
        cout << " 5. View Match Results\n";
        cout << " 6. Exit Game\n";
        cout << "===========================================\n";
        cout << "Enter choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            continue;
        }
        PlaySound(NULL, 0, 0);
        clearScreen();


        if (choice == 1) {
            PlaySound(TEXT("pvp.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            playPVP();
        } else if (choice == 2) {
             PlaySound(TEXT("pvp.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            playPVC();
        } else if (choice == 3) {              
            viewAllCharacters();
        } else if (choice == 4) {              
            showCredits();
        }else if(choice == 5){
            viewMatchHistory();
        } else if (choice == 6) {
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

// ===========================================
// CHARACTER ROSTER & DETAIL 
// ===========================================
// ... (initRoster, viewAllCharacters, displayCharacterDetails - content unchanged)
void Game::initRoster() {
    // Arnold
    Character arnold(
        "Arnold",
        "The Lover Boy",
        100,
        80,
        18,
        "Romantic Aura: Heals 5 HP whenever his special charm lands.",
        "Arnold used to write love letters to half the class, but one viral rejection turned him into a legend of heartbreak.",
        "Arnold holds grudges against Kyle for mocking his love notes, Timothy for roasting his failed confessions, and Rolando for stealing the spotlight at every school event."
    );
    arnold.addSkill({"Heart Shot", "A focused blast of pure charm.", 10, false});
    arnold.addSkill({"Romantic Shield", "Protects his heart and softens incoming blows.", 12, false});
    arnold.addSkill({"Starlit Serenade", "A cosmic love song that hits harder in the dark of space.", 18, false});
    arnold.addSkill({"Love Delete", "A forbidden letter that deletes the enemy from his story.", 0, true});

    // Kyle
    Character kyle(
        "Kyle",
        "The Master Beater",
        120,
        60,
        20,
        "Combo Master: 20% chance to double his attack.",
        "Kyle dominates every training exam and combat class, famous for finishing practice fights before the timer starts.",
        "Kyle cannot stand Arnold's drama, Laurence's attitude, and Timothy's trash talk. He swore to beat them all in the arena."
    );
    kyle.addSkill({"Meteor Jab", "Fast galactic punches straight to the jaw.", 10, false});
    kyle.addSkill({"Asteroid Uppercut", "A heavy uppercut that launches rivals skyward.", 15, false});
    kyle.addSkill({"Orbit Breaker", "A ruthless combo that breaks enemy rhythm.", 20, false});
    kyle.addSkill({"Galaxy Eraser", "A clean, brutal hit that erases the opponent.", 0, true});

    // Laurence
    Character laurence(
        "Laurence",
        "The Bitch Slayer",
        110,
        90,
        17,
        "Relentless: Deals +5 damage against low HP opponents.",
        "Laurence was once quiet, until everyone pushed too far. Now no one dares cross him twice.",
        "Laurence has history with everyone. He remembers every insult from Timothy, every flex from Kyle, and every dramatic scene from Arnold."
    );
    laurence.addSkill({"Nebula Slash", "A sharp strike that cuts through space dust.", 8, false});
    laurence.addSkill({"Supernova Spin", "A spinning slash powered by starfire.", 14, false});
    laurence.addSkill({"Void Pressure", "Gravity crushes the enemy from all sides.", 18, false});
    laurence.addSkill({"Oblivion Cut", "A one-hit dimensional cut that sends foes into nothingness.", 0, true});

    // Timothy
    Character timothy(
        "Timothy",
        "The Trash Talker",
        100,
        100,
        15,
        "Mind Games: 30% chance to add extra psychic damage.",
        "Timothy never throws the first punch, just the first insult. Most battles start because of his mouth.",
        "Timothy roasts everyone: Arnold's heartbreak, Kyle's ego, Laurence's temper, and Rolando's dramatic entrances."
    );
    timothy.addSkill({"Verbal Meteor", "Insults that land like falling rocks.", 8, false});
    timothy.addSkill({"Psychic Echo", "Words that echo painfully in the mind.", 12, false});
    timothy.addSkill({"Galaxy Roast", "An interstellar insult that burns pride and HP.", 20, false});
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
        "Rolando feels above petty drama, but deep down he is tired of Kyle's bragging, Timothy's comments, and being treated like a final boss by everyone."
    );
    rolando.addSkill({"Comet Strike", "A charged strike with comet-level impact.", 10, false});
    rolando.addSkill({"Black Hole Crash", "Pulls the foe in before landing a crushing blow.", 15, false});
    rolando.addSkill({"Starfall Barrage", "A barrage of starlight strikes from above.", 20, false});
    rolando.addSkill({"Cosmic Delete", "Erases the enemy from the galactic record.", 0, true});

    roster.clear();
    roster.push_back(arnold);
    roster.push_back(kyle);
    roster.push_back(laurence);
    roster.push_back(timothy);
    roster.push_back(rolando);
}


void Game::viewAllCharacters() {
    while (true) {
        clearScreen();
        cout << "===== VIEW ALL CHARACTERS =====\n\n";

        for (size_t i = 0; i < roster.size(); i++) {
            cout << (i + 1) << ". " << roster[i].getName() << "\n";
        }

        cout << (roster.size() + 1) << ". Back\n\n";

        cout << "Choose a character to view: ";
        int choice;
        
        if (!(cin >> choice)) {
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
             continue;
        }

        if (choice == roster.size() + 1) {
            clearScreen(); 
            return;
        }

        if (choice >= 1 && choice <= (int)roster.size()) {
            displayCharacterDetails(choice - 1);
        }
    }
}


void Game::displayCharacterDetails(int index) {
    clearScreen();

    Character &c = roster[index]; 
    
    cout << "=========================================\n"; 
    cout << "            CHARACTER PROFILE            \n"; 
    cout << "=========================================\n\n";
    cout << "Name : " << c.getName() << "\n";
    cout << "Title: " << c.getTitle() << "\n\n";

    cout << "--- BACKSTORY ---\n";
    cout << c.getBio() << "\n\n"; 
    cout << "--- GRUDGES ---\n";
    cout << c.getGrudge() << "\n\n"; 

    cout << "=========================================\n\n";
    cout << left; 
    cout << setw(12) << "HP"          << ": " << c.getHP() << " / " << c.getMaxHP() << "\n";
    cout << setw(12) << "Mana"        << ": " << c.getMana() << " / " << c.getMaxMana() << "\n";
    cout << setw(12) << "Base Damage" << ": " << c.getBaseDamage() << "\n\n";
    cout << "=========================================\n\n";
    
    // Skill Preview 
    cout << "Skills & Damage Preview:\n\n";
    const vector<Skill> &skills = c.getSkills();
    int minDmg = c.getBaseDamage() - 3;
    int maxDmg = c.getBaseDamage() + 5;

    for (size_t i = 0; i < skills.size(); ++i) {
        cout << "  " << i + 1 << ") " << skills[i].name
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
    cout << "=========================================\n\n";

    cout << "Press Enter to go back...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


// ===========================================
// CHARACTER SELECTION & COMBAT LOGIC 
// ===========================================
// ... (chooseCharacter, getRandomInt, chooseSkill, computeDamage, handleLowHP - content unchanged)
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

        // Skill damage preview 
        cout << "  Skills & Damage Preview:\n\n";
        const vector<Skill> &skills = chosen.getSkills();
        int minDmg = chosen.getBaseDamage() - 3;
        int maxDmg = chosen.getBaseDamage() + 5;

        for (size_t i = 0; i < skills.size(); ++i) {
            cout << "    " << i + 1 << ") " << skills[i].name
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
    // Should not happen, but return an empty character to satisfy compiler
    return Character("", "", 0, 0, 0, "", "", ""); 
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
              << "  (Mana: " << skills[i].manaCost << ")";
        if (skills[i].isOneHitDelete) {
            cout << "  [ONE-HIT DELETE]";
        } else if (ch.getMana() < skills[i].manaCost) {
            cout << "  [MANA TOO LOW]";
        }
        cout << "\n      " << skills[i].description;
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
        return defender.getHP(); 
    }
    
    // Check if the skill is the Basic Attack fallback
    bool isBasicAttack = (skill.name == BASIC_ATTACK.name);

    int base = attacker.getBaseDamage();
    int randomBonus = getRandomInt(-3, 5);
    int damage = base + randomBonus;

    cout << "  Damage roll: base " << base
          << " + random " << randomBonus
          << " = " << damage << " before passives.\n";
          
    // Apply skill damage modifier ONLY if it's a specific skill (not Basic Attack)
    if (!isBasicAttack) {
        damage += skill.manaCost; // Reusing manaCost as a base damage bonus
    }

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

bool Game::handleLowHP(Character &ch, int playerNumber, GameMode mode, bool isHuman) {
    (void)mode; 

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


// ===========================================
// GAME MODES
// ===========================================

void Game::playPVP() {
    clearScreen();

    cout << "=============== PLAYER VS PLAYER ===============\n\n";

    Character p1 = chooseCharacter(1, true, -1);

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
            
            const Skill* skillToUse1 = &s1; 

            if (!s1.isOneHitDelete && p1.getMana() < s1.manaCost) {
                cout << "Not enough mana for " << s1.name
                      << ". Using basic attack instead.\n\n";
                skillToUse1 = &BASIC_ATTACK;
            } else {
                p1.useMana(s1.manaCost);
            }

            int dmg1 = computeDamage(p1, p2, *skillToUse1);
            cout << "Player 1 uses " << skillToUse1->name
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
            
            const Skill* skillToUse2 = &s2; 

            if (!s2.isOneHitDelete && p2.getMana() < s2.manaCost) {
                cout << "Not enough mana for " << s2.name
                      << ". Using basic attack instead.\n\n";
                skillToUse2 = &BASIC_ATTACK;
            } else {
                p2.useMana(s2.manaCost);
            }

            int dmg2 = computeDamage(p2, p1, *skillToUse2);
            cout << "Player 2 uses " << skillToUse2->name
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
    std::string winnerName;
    std::string score = std::to_string(p1Wins) + "-" + std::to_string(p2Wins);
    
    if (p1Wins > p2Wins) {
        cout << "Player 1 is the champion of Galactica Academy!\n\n";
        winnerName = p1.getName();
    } else if (p2Wins > p1Wins) {
        cout << "Player 2 is the champion of Galactica Academy!\n\n";
        winnerName = p2.getName();
    } else {
        cout << "It ends in a cosmic draw.\n\n";
        winnerName = "Draw";
    }

    // Record the match result
    MatchResult result;
    result.mode = "PVP";
    result.p1_char = p1.getName();
    result.p2_char = p2.getName();
    result.winner_name = winnerName;
    result.score = score;
    result.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    history.push_back(result);
    
    // **QUEUE LOGIC:** Check capacity and delete the oldest entry (FIFO) if exceeded
    if (history.size() > MAX_HISTORY_CAPACITY) {
        history.erase(history.begin());
    }
    // End QUEUE LOGIC

    saveMatchHistory();

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

void Game::playPVC() {
    clearScreen();

    cout << "============= PLAYER VS COMPUTER =============\n\n";

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
            
            const Skill* skillToUseP = &sp; 

            if (!sp.isOneHitDelete && player.getMana() < sp.manaCost) {
                cout << "Not enough mana for " << sp.name
                      << ". Using basic attack instead.\n\n";
                skillToUseP = &BASIC_ATTACK;
            } else {
                player.useMana(sp.manaCost);
            }

            int dmgP = computeDamage(player, computer, *skillToUseP);
            cout << "You use " << skillToUseP->name
                  << " and deals " << dmgP << " damage!\n\n";
            computer.takeDamage(dmgP);

            cout << "Remaining HP and Mana:\n";
            cout << "  You      - HP: " << player.getHP()
                  << " | Mana: " << player.getMana() << "\n";
            cout << "  Computer - HP: " << computer.getHP()
                  << " | Mana: " << computer.getMana() << "\n\n";

            if (!computer.isAlive()) {
                break;
            }

            // Computer turn (AI logic is simple random choice)
            cout << "------------- COMPUTER TURN -------------\n\n";
            int aiSkillIndex = getRandomInt(
                0,
                static_cast<int>(computer.getSkills().size()) - 1
            );
            const Skill &sa = computer.getSkills()[aiSkillIndex];

            const Skill* skillToUseA = &sa; 

            if (!sa.isOneHitDelete && computer.getMana() < sa.manaCost) {
                // AI uses basic attack
                skillToUseA = &BASIC_ATTACK;
            } else {
                computer.useMana(sa.manaCost);
            }

            int dmgA = computeDamage(computer, player, *skillToUseA);
            cout << "Computer uses " << skillToUseA->name
                  << " and deals " << dmgA << " damage!\n\n";
            player.takeDamage(dmgA);

            cout << "Remaining HP and Mana:\n";
            cout << "  You      - HP: " << player.getHP()
                  << " | Mana: " << player.getMana() << "\n";
            cout << "  Computer - HP: " << computer.getHP()
                  << " | Mana: " << computer.getMana() << "\n\n";
        }
        
        // Announce round winner
        if (player.isAlive() && !computer.isAlive()) {
            cout << "Player 1 wins Round " << round << "!\n\n";
            playerWins++;
        } else if (!player.isAlive() && computer.isAlive()) {
            cout << "Computer wins Round " << round << "!\n\n";
            computerWins++;
        }

        cout << "Scoreboard:\n";
        cout << "  Player 1: " << playerWins << " wins\n";
        cout << "  Computer: " << computerWins << " wins\n\n";

        if (round < 5 && playerWins < roundsToWin && computerWins < roundsToWin) {
            cout << "Press Enter to proceed to the next round...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }
    }

    // **FIXED: Completed the function with the match result and history logging.**
    cout << "================ MATCH RESULT ================\n\n";
    std::string winnerName;
    std::string score = std::to_string(playerWins) + "-" + std::to_string(computerWins);
    
    if (playerWins > computerWins) {
        cout << "Player 1 is the champion of Galactica Academy!\n\n";
        winnerName = player.getName();
    } else if (computerWins > playerWins) {
        cout << "The Computer is the champion of Galactica Academy!\n\n";
        winnerName = computer.getName();
    } else {
        cout << "It ends in a cosmic draw.\n\n";
        winnerName = "Draw";
    }

    // Record the match result
    MatchResult result;
    result.mode = "PVC";
    result.p1_char = player.getName();
    result.p2_char = computer.getName();
    result.winner_name = winnerName;
    result.score = score;
    result.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    history.push_back(result);
    
    // **QUEUE LOGIC:** Check capacity and delete the oldest entry (FIFO) if exceeded
    if (history.size() > MAX_HISTORY_CAPACITY) {
        history.erase(history.begin());
    }
    // End QUEUE LOGIC

    saveMatchHistory();

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

void Game::showCredits() {
    clearScreen();

    string credits[] = {
        "================ CREDITS ================",
        "Game Design     : Dave Laurence R. Repe",
        "Programming     : Arnold Michael P. Tabada, Nirhevn kyle Dialimas",
        "Artwork         : John Timothy Cabuguas",
        "Story & Lore    : John Timothy Cabuguas ",
        "Special Thanks  :  For: Rolando Supremo / Mentor: Rolando Supremo  / Inspiration: Rolando Supremo ",
        "========================================",
        "",
        "Thank you for playing Galactica Campus Brawl!",
        "Hope you enjoyed your adventure!",
        "",
        "Press Enter to go back..."
    };

    int totalLines = sizeof(credits) / sizeof(credits[0]);

    // Scroll effect
    for (int i = 0; i < totalLines + 10; i++) { 
        clearScreen();
        int start = max(0, i - 10); 
        for (int j = start; j <= i && j < totalLines; j++) {
            cout << credits[j] << "\n";
        }
        this_thread::sleep_for(chrono::milliseconds(500)); 
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen(); 
}