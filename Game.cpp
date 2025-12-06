#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <cctype>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional> // Added in the first snippet
#include <unordered_map> // Added in the first snippet

#include "Game.h"
#include "Character.h"
#include "Skill.h"

using namespace std;

const char* HISTORY_FILE = "match_history.txt";

// ===========================================
// MATCH RESULT: TIME STRING
// ===========================================

string MatchResult::getTimeString() const {
    // timestamp is stored as milliseconds since Unix epoch
    using namespace std::chrono;

    time_t raw = static_cast<time_t>(timestamp / 1000);
    tm* ti = localtime(&raw);
    if (!ti) return "unknown";

    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ti);
    return string(buf);
}

// ===========================================
// CONSTRUCTOR / CORE
// ===========================================

Game::Game() {
    random_device rd;
    rng.seed(rd());
    initRoster();
    loadMatchHistory();
}

void Game::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}

void Game::run() {
    clearScreen();
    showIntro();
    mainMenu();
}

// ===========================================
// MATCH HISTORY SYSTEM
// ===========================================

void Game::saveMatchHistory() {
    ofstream out(HISTORY_FILE);
    if (!out.is_open()) return;

    for (auto& r : history) {
        out << r.mode << "|"
            << r.p1_char << "|"
            << r.p2_char << "|"
            << r.winner_name << "|"
            << r.score << "|"
            << r.timestamp << "\n";
    }
}

void Game::loadMatchHistory() {
    ifstream in(HISTORY_FILE);
    if (!in.is_open()) return;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string part;
        vector<string> p;

        while (getline(ss, part, '|'))
            p.push_back(part);

        if (p.size() == 6) {
            history.push_back({
                p[0], p[1], p[2], p[3], p[4], stoll(p[5])
            });
        }
    }

    // Enforce queue capacity (FIFO)
    while (history.size() > MAX_HISTORY_CAPACITY)
        history.erase(history.begin());
}

void Game::viewMatchHistory() {
    clearScreen();
    cout << "============== MATCH HISTORY (Last "
         << MAX_HISTORY_CAPACITY << ") ==============\n\n";

    if (history.empty()) {
        cout << "No past cosmic brawls found in the archives.\n\n";
    } else {
        cout << left
             << setw(6)  << "Mode"
             << setw(15) << "P1 Char"
             << setw(15) << "P2 Char"
             << setw(15) << "Winner"
             << setw(10) << "Score"
             << "Time\n";
        cout << "---------------------------------------------------------------\n";

        for (const auto& r : history) {
            cout << left
                 << setw(6)  << r.mode
                 << setw(15) << r.p1_char
                 << setw(15) << r.p2_char
                 << setw(15) << r.winner_name
                 << setw(10) << r.score
                 << r.getTimeString() << "\n";
        }
        cout << "\n";
    }

    cout << "Press Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

// ===========================================
// TYPE TEXT — Sleep() + SKIP WITH SPACEBAR
// ===========================================

void Game::typeText(const char* text, int delayMs) {
    bool skipLine = false;

    for (int i = 0; text[i] != '\0'; i++) {
        cout << text[i] << flush;

        if (skipLine || delayMs <= 0)
            continue;

        int step = max(1, delayMs / 5);
        int elapsed = 0;

        while (elapsed < delayMs) {
            if (_kbhit()) {
                if (_getch() == ' ') {
                    // Spacebar: skip the rest of this line at full speed
                    skipLine = true;
                    break;
                }
            }

            Sleep(step);
            elapsed += step;
        }
    }
}

// ===========================================
// INTRO SCREEN (WITH MUSIC)
// ===========================================

void Game::showIntro() {
    // Loop intro music
    PlaySound(TEXT("intro.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    cout << "==================================================\n";
    cout << "          ROLANDO GALACTIC GRAVEYARD              \n";
    cout << "==================================================\n\n";

    typeText("Welcome to Rolando Galactic Graveyard, a floating school.\n", 25);
    typeText("in deep space where students settle their rivalries\n", 25);
    typeText("in the legendary battle arena.\n\n", 25);
    typeText("Choose your fighter, unleash your skills,\n", 25);
    typeText("Tonight, a new hunter enters the field.\n", 25);
    typeText("Choose your warrior and carve your legend.\n\n", 25);

    typeText("Press Enter to continue...", 25);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    // Stop music
    PlaySound(NULL, 0, 0);
    clearScreen();
}

// ===========================================
// CONFIRM EXIT
// ===========================================

bool Game::confirmExit() {
    while (true) {
        cout << "\nAre you sure to exit? yes/no: " << flush;
        string a;
        cin >> a;

        for (char& c : a) c = static_cast<char>(tolower(c));

        if (a == "yes") return true;
        if (a == "no") {
            return false;
        }

        cout << "Please type yes or no.\n";
    }
}

// ===========================================
// MAIN MENU
// ===========================================

void Game::mainMenu() {

    unordered_map<int, function<void()>> menuActions = {
        {1, [this]{ PlaySound(TEXT("battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); playPVP(); }},
        {2, [this]{ PlaySound(TEXT("battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); playPVC(); }},
        {3, [this]{ viewAllCharacters(); }},
        {4, [this]{ PlaySound(TEXT("intro.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); showCredits(); }},
        {5, [this]{  PlaySound(TEXT("characterselection.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); viewMatchHistory(); }},
        {6, [this]{ if(confirmExit()){ clearScreen(); exit(0); } }}
    };

    while (true) {
        clearScreen();
        PlaySound(TEXT("characterselection.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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

        auto it = menuActions.find(choice);
        if (it != menuActions.end()) {
        it->second();

        } else {
            cout << "Invalid choice. Please try again.\n\n";
        }
    }
}

// ===========================================
// ROSTER SETUP
// ===========================================

void Game::initRoster() {
    // Arnold
    struct CharacterData {
        std::string name, title, bio, grudge;
        int hp, mana, baseDmg;
        std::vector<Skill> skills;
    };

    std::vector<CharacterData> data = {
        //Arnold
        {"Arnold", "The Lover Boy",
         "Arnold used to write love letters to half the class...",
         "Arnold holds grudges against Kyle, Timothy, and Rolando.",
         100, 80, 18,
         { {"Heart Shot", "A focused blast of pure charm.", 10, false},
           {"Romantic Shield", "Softens incoming blows.", 12, false},
           {"Starlit Serenade", "A cosmic love song.", 18, false},
           {"Love Delete", "Deletes the enemy.", 0, true} }},
        // kyle
        {"Kyle", "The Master Beater",
         "Kyle dominates every combat exam.",
         "Kyle cannot stand Arnold's drama, Laurence's attitude, and Timothy's trash talk.",
         120, 60, 20,
         { {"Meteor Jab", "Fast galactic punches.", 10, false},
           {"Asteroid Uppercut", "Launches rivals.", 15, false},
           {"Orbit Breaker", "Breaks enemy rhythm.", 20, false},
           {"Galaxy Eraser", "One-hit erase.", 0, true} }},
        //Dave
        {"Laurence", "The Bitch Slayer",
         "Laurence was once quiet, until everyone pushed too far.",
         "Laurence has history with everyone and never forgets a slight.",
         110, 90, 17,
         { {"Nebula Slash", "Sharp space strike.", 8, false},
           {"Supernova Spin", "Starfire spin.", 14, false},
           {"Void Pressure", "Gravity crush.", 18, false},
           {"Oblivion Cut", "Dimensional delete.", 0, true} }},
        //timothy
        {"Timothy", "The Trash Talker",
         "Timothy starts fights with words, not punches.",
         "He roasts Arnold's heartbreaks, Kyle's ego, and Laurence's temper.",
         100, 100, 15,
         { {"Verbal Meteor", "Insults drop like rocks.", 8, false},
           {"Psychic Echo", "Painful echoes in the mind.", 12, false},
           {"Galaxy Roast", "Burns pride and HP.", 20, false},
           {"Silence of Space", "Cursed delete.", 0, true} }},
        //Rolando
        {"Rolando", "Galactic Slayer",
         "Rumored to clear simulations alone.",
         "He hates Kyle's bragging and Timothy's comments.",
         130, 70, 19,
         { {"Comet Strike", "Comet impact.", 10, false},
           {"Black Hole Crash", "Pull + crush.", 15, false},
           {"Starfall Barrage", "Starlight barrage.", 20, false},
           {"Cosmic Delete", "Instant erase.", 0, true} }}
    };

    roster.clear();
    for (const auto& d : data) {
        Character c(d.name, d.title, d.hp, d.mana, d.baseDmg, "", d.bio, d.grudge);
        for (const auto& s : d.skills) c.addSkill(s);
        roster.push_back(c);
    }
}

// ===========================================
// CHARACTER VIEW, DETAILS, SELECTION
// ===========================================

void Game::viewAllCharacters() {
    while (true) {
        clearScreen();
        cout << "===== VIEW ALL CHARACTERS =====\n\n";

        for (size_t i = 0; i < roster.size(); i++)
            cout << i+1 << ". " << roster[i].getName() << "\n";

        cout << roster.size() + 1 << ". Back\n\n";

        cout << "Choose a character: ";
        int choice;

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == (int)roster.size() + 1) {
            clearScreen();
            return;
        }

        if (choice >= 1 && choice <= (int)roster.size())
            displayCharacterDetails(choice - 1);
    }
}

void Game::displayCharacterDetails(int i) {
    clearScreen();

    Character& c = roster[i];

    cout << "=========================================\n";
    cout << "            CHARACTER PROFILE            \n";
    cout << "=========================================\n\n";

    cout << "Name : " << c.getName() << "\n";
    cout << "Title: " << c.getTitle() << "\n\n";

    cout << "--- BACKSTORY ---\n" << c.getBio() << "\n\n";
    cout << "--- GRUDGES ---\n" << c.getGrudge() << "\n\n";

    cout << left;
    cout << setw(12) << "HP"          << ": " << c.getHP()        << " / " << c.getMaxHP()     << "\n";
    cout << setw(12) << "Mana"        << ": " << c.getMana()      << " / " << c.getMaxMana()   << "\n";
    cout << setw(12) << "Base Damage" << ": " << c.getBaseDamage() << "\n\n";

    cout << "=== SKILLS ===\n";
    const auto& s = c.getSkills();

    int min = c.getBaseDamage() - 3;
    int max = c.getBaseDamage() + 5;

    for (size_t z = 0; z < s.size(); z++) {
        cout << z+1 << ") " << s[z].name << " (Mana: " << s[z].manaCost << ")\n";
        cout << "    " << s[z].description << "\n";
        if (s[z].isOneHitDelete)
            cout << "    Damage: ONE-HIT DELETE.\n\n";
        else
            cout << "    Est. Damage: " << min << "–" << max << "\n\n";
    }

    cout << "Press Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ===========================================
// UTILS & SKILLS
// ===========================================

int Game::getRandomInt(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

int Game::chooseSkill(const Character& ch) {
    const auto& skills = ch.getSkills();

    cout << "Choose a skill for " << ch.getName() << ":\n\n";

    for (size_t i = 0; i < skills.size(); i++) {
        cout << i+1 << ") " << skills[i].name
             << " (Mana: " << skills[i].manaCost << ")";
        if (skills[i].isOneHitDelete)
            cout << " [DELETE]";
        else if (ch.getMana() < skills[i].manaCost)
            cout << " [LOW MANA]";
        cout << "\n    " << skills[i].description << "\n\n";
    }

    int suggested = getRandomInt(1, (int)skills.size());
    cout << "Random suggests: " << suggested << "\n\n";

    int c;
    while (true) {
        cout << "Enter skill number: ";
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (c >= 1 && c <= (int)skills.size()) break;
        cout << "Invalid.\n";
    }

    system("cls");
    return c-1;
}

int Game::computeDamage(Character& atk, Character& def, const Skill& s) {
    // ONE HIT DELETE
    if (s.isOneHitDelete) {
        cout << "*** ONE HIT DELETE ACTIVATED! ***\n";
        return def.getHP();
    }

    bool basic = (s.name == BASIC_ATTACK.name);

    int base = atk.getBaseDamage();
    int rnd = getRandomInt(-3, 5);
    int dmg = base + rnd;

    if (!basic)
        dmg += s.manaCost;

    // PASSIVES
    if (atk.getName() == "Arnold") {
        if (getRandomInt(1,100) <= 25) {
            cout << "Romantic Aura! +5 dmg + heal 5.\n";
            dmg += 5;
            atk.heal(5);
        }
    }
    else if (atk.getName() == "Kyle") {
        if (getRandomInt(1,100) <= 20) {
            cout << "Combo Master! DOUBLE DAMAGE!\n";
            dmg *= 2;
        }
    }
    else if (atk.getName() == "Laurence") {
        if (def.getHP() < 40) {
            cout << "Relentless! +5 dmg.\n";
            dmg += 5;
        }
    }
    else if (atk.getName() == "Timothy") {
        if (getRandomInt(1,100) <= 30) {
            cout << "Mind Games! +5 dmg.\n";
            dmg += 5;
        }
    }
    else if (atk.getName() == "Rolando") {
        if (getRandomInt(1,100) <= 25) {
            cout << "Galactic Fury! +5 dmg.\n";
            dmg += 5;
        }
    }

    if (dmg < 0) dmg = 0;

    cout << "Final damage: " << dmg << "\n\n";
    return dmg;
}

// ===========================================
// CONTINUE OR SURRENDER
// ===========================================

bool Game::handleLowHP(Character &ch, int playerNumber, GameMode mode, bool isHuman) {
    using namespace std;

    (void)mode;
    if (!isHuman) return true;

    if (ch.getHP() <= 15 && ch.isAlive()) {
        int choice;

        cout << "\n";
        cout << "STATUS\n"; 
        cout << "Player " << playerNumber
             << " (" << ch.getName() << "): HP: " << ch.getHP() << " | Mana: 25\n"; 
        cout << "Warning: Player " << playerNumber
             << " (" << ch.getName() << ") has only " << ch.getHP() << " HP left.\n";
        cout << "Do you want to continue the battle or surrender?\n";
        
        cout << " 1. Continue\n";
        
        cout << " 2. Surrender\n";

        do {
            cout << "Choose option: ";

            if (cin >> choice) {
                if (choice == 1) {
                    clearScreen();
                    return true;
                } else if (choice == 2) {
                    clearScreen();
                    cout << "Player " << playerNumber << " (" << ch.getName() << ") has surrendered!\n";
                    return false;
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Invalid choice. Please select again.\n"; 
            
        } while (true);
    }

    return true;
}


// ===========================================
// CHARACTER SELECTION
// ===========================================

Character Game::chooseCharacter(int playerNumber, bool showGrudges, int forbiddenIndex) {
    while (true) {
        clearScreen();
        cout << "===== PLAYER " << playerNumber << " -- CHOOSE YOUR CHARACTER =====\n\n";

        for (size_t i = 0; i < roster.size(); i++) {
            if ((int)i == forbiddenIndex)
                cout << i + 1 << ". " << roster[i].getName() << " (UNAVAILABLE)\n";
            else
                cout << i + 1 << ". " << roster[i].getName() << "\n";
        }

        cout << roster.size() + 1 << ". Back\n\n";

        int choice;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Back option
        if (choice == (int)roster.size() + 1) {
            Character back;
            back.setHP(-1);          // special flag
            back.setMaxHP(-1);
            return back;              // signal to cancel
        }

        if (choice >= 1 && choice <= (int)roster.size()) {
            int index = choice - 1;

            if (index == forbiddenIndex) {
                cout << "That character is unavailable. Choose another.\n";
                Sleep(1000);
                continue;
            }

            clearScreen();
            displayCharacterDetails(index);

            if (showGrudges) {
                cout << "\nThis character holds grudges against: "
                     << roster[index].getGrudge() << "\n\n";
            }

        cout << "Confirm select? (yes/no): ";
        string confirm;
        while (true) {
            cin >> confirm;
            for (char &c : confirm) c = static_cast<char>(tolower(c));

            if (confirm == "yes") {
                return roster[index];  
            }
            else if (confirm == "no") {
                break;  
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        }
        else {
            cout << "Invalid choice.\n";
            Sleep(800);
        }
    }
}


// ===========================================
// PVP MODE
// ===========================================

void Game::playPVP() {
    clearScreen();
    cout << "=============== PLAYER VS PLAYER ===============\n\n";

    Character p1 = chooseCharacter(1, true, -1);
    if (p1.getHP() == -1) return;  // player chose Back -> exit to main menu

    int forbidden = -1;
    for (size_t i = 0; i < roster.size(); i++) {
        if (roster[i].getName() == p1.getName())
        forbidden = (int)i;
    }

    Character p2 = chooseCharacter(2, true, forbidden);
    if (p2.getHP() == -1) return;  // player chose Back -> exit to main menu

    // MUSIC: begin battle music AFTER both players are chosen
    PlaySound(TEXT("battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    int w1 = 0, w2 = 0;
    const int winGoal = 3;

    for (int round = 1; round <= 5 && w1 < winGoal && w2 < winGoal; round++) {

        p1.resetForNewRound();
        p2.resetForNewRound();

        clearScreen();
        cout << "================= ROUND " << round << " =================\n\n";

        while (p1.isAlive() && p2.isAlive()) {
            cout << "STATUS:\n";
            cout << "P1: " << p1.getName() << " HP: " << p1.getHP() << " Mana: " << p1.getMana() << "\n";
            cout << "P2: " << p2.getName() << " HP: " << p2.getHP() << " Mana: " << p2.getMana() << "\n\n";

            if (!handleLowHP(p1, 1, GameMode::PVP, true)) {
                w2++;
                break;
            }

            cout << "--- PLAYER 1 TURN ---\n";
            int i1 = chooseSkill(p1);
            const Skill& s1 = p1.getSkills()[i1];

            const Skill* use1 = &s1;
            if (!s1.isOneHitDelete && p1.getMana() < s1.manaCost) {
                cout << "Not enough mana! Using basic attack.\n";
                use1 = &BASIC_ATTACK;
            } else p1.useMana(s1.manaCost);

            int dmg1 = computeDamage(p1, p2, *use1);
            p2.takeDamage(dmg1);

            if (!p2.isAlive()) break;

            if (!handleLowHP(p2, 2, GameMode::PVP, true)) {
                w1++;
                break;
            }

            cout << "--- PLAYER 2 TURN ---\n";
            int i2 = chooseSkill(p2);
            const Skill& s2 = p2.getSkills()[i2];

            const Skill* use2 = &s2;
            if (!s2.isOneHitDelete && p2.getMana() < s2.manaCost) {
                cout << "Not enough mana! Using basic attack.\n";
                use2 = &BASIC_ATTACK;
            } else p2.useMana(s2.manaCost);

            int dmg2 = computeDamage(p2, p1, *use2);
            p1.takeDamage(dmg2);
        }

        if (p1.isAlive() && !p2.isAlive()) {
            cout << "Player 1 wins round " << round << "!\n\n";
            w1++;
        }
        else if (p2.isAlive() && !p1.isAlive()) {
            cout << "Player 2 wins round " << round << "!\n";
            w2++;
        }

        cout << "Score: " << p1.getName() << " = " << w1 << " | "
             << p2.getName() << " = " << w2 << "\n\n";
             
        if (round < 5 && w1 < winGoal && w2 < winGoal) {
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }
    }

    cout << "================ MATCH RESULT ================\n\n";
    string winner;
    if (w1 > w2) {
        cout << "Player 1 is the champion!\n";
        winner = p1.getName();
    }
    else if (w2 > w1) {
        cout << "Player 2 is the champion!\n";
        winner = p2.getName();
    }
    else {
        cout << "The battle ends in a draw.\n";
        winner = "Draw";
    }

    // Stop battle music and play winner sound (no loop) if there is a champion
    PlaySound(NULL, 0, 0);
    if (winner != "Draw") {
        PlaySound(TEXT("winner.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }

    using namespace std::chrono;
    MatchResult mr;
    mr.mode = "PVP";
    mr.p1_char = p1.getName();
    mr.p2_char = p2.getName();
    mr.winner_name = winner;
    mr.score = to_string(w1) + "-" + to_string(w2);
    mr.timestamp = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();

    history.push_back(mr);
    if (history.size() > MAX_HISTORY_CAPACITY)
        history.erase(history.begin());

    saveMatchHistory();

    cout << "\nPress Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

// ===========================================
// PVC MODE
// ===========================================

void Game::playPVC() {
    clearScreen();
    cout << "============= PLAYER VS COMPUTER =============\n\n";

    // Player chooses character
    Character p = chooseCharacter(1, false, -1);

    // Check if player pressed Back
    if (p.getHP() == -1) return;  // return to main menu

    // Find the player's character index to forbid bot from choosing it
    int forbiddenIndex = -1;
    for (size_t i = 0; i < roster.size(); i++) {
        if (roster[i].getName() == p.getName()) {
            forbiddenIndex = (int)i;
            break;
        }
    }

    // Pick bot character avoiding the player's character
    int aiIndex;
    do {
        aiIndex = getRandomInt(0, (int)roster.size() - 1);
    } while (aiIndex == forbiddenIndex);
    Character bot = roster[aiIndex];

    int pw = 0, cw = 0;
    const int winGoal = 3;

    cout << "Computer chosen: " << bot.getName() << " " << bot.getTitle() << "\n";
    cout << "Press Enter to start the battle...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    // MUSIC: battle music starts when the fight actually begins
    PlaySound(TEXT("battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    for (int r = 1; r <= 5 && pw < winGoal && cw < winGoal; r++) {

        p.resetForNewRound();
        bot.resetForNewRound();

        bot.setMaxHP(200);
        bot.setHP(200);

        clearScreen();
        cout << "================= ROUND " << r << " =================\n\n";

        while (p.isAlive() && bot.isAlive()) {

            cout << "STATUS:\n";
            cout << "You: " << p.getName() << " HP: " << p.getHP() << " Mana: " << p.getMana() << "\n";
            cout << "Bot: " << bot.getName() << " HP: " << bot.getHP() << " Mana: " << bot.getMana() << "\n\n";

            if (!handleLowHP(p, 1, GameMode::PVC, true)) {
                cw++;
                break;
            }

            cout << "--- YOUR TURN ---\n";
            int psi = chooseSkill(p);
            const Skill& ps = p.getSkills()[psi];

            const Skill* useP = &ps;
            if (!ps.isOneHitDelete && p.getMana() < ps.manaCost)
                useP = &BASIC_ATTACK;
            else
                p.useMana(ps.manaCost);

            int pdmg = computeDamage(p, bot, *useP);
            bot.takeDamage(pdmg);

            if (!bot.isAlive()) break;

            cout << "--- BOT TURN ---\n";

            // Bot chooses skill excluding 1-hit delete
            std::vector<int> availableSkills;
            for (size_t i = 0; i < bot.getSkills().size(); i++) {
                if (!bot.getSkills()[i].isOneHitDelete)
                    availableSkills.push_back((int)i);
            }

            int aiS = availableSkills[getRandomInt(0, (int)availableSkills.size() - 1)];
            const Skill& as = bot.getSkills()[aiS];

            const Skill* useA = &as;
            if (bot.getMana() < as.manaCost)
                useA = &BASIC_ATTACK;
            else
                bot.useMana(as.manaCost);

            int admg = computeDamage(bot, p, *useA);
            p.takeDamage(admg);
        }

        if (p.isAlive() && !bot.isAlive()) {
            cout << "You win round " << r << "!\n";
            pw++;
        } else if (bot.isAlive() && !p.isAlive()) {
            cout << "Bot wins round " << r << "!\n";
            cw++;
        }

        cout << "Score: " << p.getName() << " = " << pw << " | "
             << bot.getName() << " = " << cw << "\n\n";

        if (r < 5 && pw < winGoal && cw < winGoal) {
            cout << "Press Enter...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }
    }

    cout << "================ MATCH RESULT ================\n\n";
    string winner;
    if (pw > cw) {
        cout << "You are the champion!\n";
        winner = p.getName();
    } else if (cw > pw) {
        cout << "The Computer wins!\n";
        winner = bot.getName();
    } else {
        cout << "Match ends in a draw.\n";
        winner = "Draw";
    }

    // Stop battle music and play appropriate result sound
    PlaySound(NULL, 0, 0);
    if (pw > cw) {
        PlaySound(TEXT("winner.wav"), NULL, SND_FILENAME | SND_ASYNC);
    } else if (cw > pw) {
        PlaySound(TEXT("defeat.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }

    using namespace std::chrono;
    MatchResult mr;
    mr.mode = "PVC";
    mr.p1_char = p.getName();
    mr.p2_char = bot.getName();
    mr.winner_name = winner;
    mr.score = to_string(pw) + "-" + to_string(cw);
    mr.timestamp = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();

    history.push_back(mr);
    if (history.size() > MAX_HISTORY_CAPACITY)
        history.erase(history.begin());

    saveMatchHistory();

    cout << "\nPress Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}


// ===========================================
// CREDITS SCREEN
// ===========================================

void Game::showCredits() {
    clearScreen();

    string credits[] = {
        "================ CREDITS ================",
        "Game Design     : Dave Laurence R. Repe",
        "Programming     : Arnold Michael P. Tabada\n\t\t: Nirhevn Kyle Dialimas\n\t\t: John Timothy Cabuguas\n\t\t: Dave Laurence R. Repe\n",
        "Artwork         : John Timothy Cabuguas",
        "Story & Lore    : John Timothy Cabuguas",
        "Special Thanks  : Rolando Supremo",
        "========================================",
        "",
        "Thank you for playing Galactica Campus Brawl!",
        "Hope you enjoyed your adventure!",
        "",
        "Press Enter to go back..."
    };

    int total = sizeof(credits) / sizeof(credits[0]);
    int delay = 400;

    for (int i = 0; i < total + 10; i++) {
        clearScreen();
        int start = max(0, i - 10);

        for (int j = start; j <= i && j < total; j++) {
            cout << credits[j] << "\n";
        }

        Sleep(delay);
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    // STOP credits music when exiting credits screen
    PlaySound(NULL, 0, 0);

    clearScreen();
}