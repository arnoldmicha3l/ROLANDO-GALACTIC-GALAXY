#include "Game.h"
#include "Character.h"
#include "Skill.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <cctype>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional>
#include <unordered_map>

#include <windows.h>
#include <mmsystem.h>
#include <conio.h> 

#pragma comment(lib, "winmm.lib")

using namespace std;

namespace Color {
    
    const std::string RESET = "\033[0m";
    
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string WHITE = "\033[37m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string ORANGE = "\033[93m";
    const std::string BROWN = "\033[33m";
    const std::string VIOLET = "\033[95m";
    
    const std::string BOLD = "\033[1m";
    const std::string BOLD_RED = "\033[1;31m";
    const std::string BOLD_GREEN = "\033[1;32m";
    const std::string BOLD_YELLOW = "\033[1;33m";
    const std::string BOLD_BLUE = "\033[1;34m";
    const std::string BOLD_MAGENTA = "\033[1;35m";
    const std::string BOLD_CYAN = "\033[1;36m";
    const std::string BRIGHT_WHITE = "\033[97m";
    const std::string BOLD_WHITE = "\033[1;97m";
    const std::string BOLD_VIOLET = "\033[1;95m";
}

#define COLOR_ARNOLD Color::BOLD_YELLOW
#define COLOR_KYLE Color::BOLD_BLUE
#define COLOR_LAURENCE Color::BOLD_MAGENTA
#define COLOR_TIMOTHY Color::WHITE
#define COLOR_ROLANDO Color::BOLD_RED
#define COLOR_ORANGE Color::ORANGE
#define COLOR_BROWN Color::BROWN
#define COLOR_VIOLET Color::VIOLET
#define COLOR_WHITE Color::WHITE

const std::string& getCharacterColor(const std::string& name) {
    if (name == "Arnold") return COLOR_ARNOLD;
    if (name == "Kyle") return COLOR_KYLE;
    if (name == "Laurence") return COLOR_LAURENCE;
    if (name == "Timothy") return COLOR_TIMOTHY;
    if (name == "Rolando") return COLOR_ROLANDO;
    return COLOR_WHITE;
}

const char* HISTORY_FILE = "match_history.txt";

// ===========================================
// MATCH RESULT: TIME STRING
// ===========================================

string MatchResult::getTimeString() const {

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

    // Queue capacity (FIFO)
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
    PlaySound(TEXT("intro.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    cout << Color::BOLD_BLUE << "==================================================" << Color::RESET << "\n";
    cout << Color::BOLD_CYAN << "         ROLANDO GALACTIC GRAVEYARD             " << Color::RESET << "\n";
    cout << Color::BOLD_BLUE << "==================================================" << Color::RESET << "\n\n";

    typeText((Color::WHITE + "Welcome to  \"Rolando Galactic Graveyard\" , a floating school.\n").c_str(), 25);
    typeText((Color::WHITE + "in deep space where students settle their rivalries\n").c_str(), 25);
    typeText((Color::WHITE + "in the legendary battle arena.\n\n").c_str(), 25);
    typeText((Color::WHITE+ "Choose your fighter, unleash your skills,\n").c_str(), 25);
    typeText((Color::WHITE + "Tonight, a new hunter enters the field.\n").c_str(), 25);
    typeText((Color::WHITE + "Choose your warrior and carve your legend." + Color::RESET + "\n\n").c_str(), 25);

    typeText(Color::BRIGHT_WHITE.c_str(), 0);
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
        cout << Color::BOLD_RED << "\nAre you sure to exit? " << Color::RESET << "(yes/no): " << flush;
        string a;
        cin >> a;

        for (char& c : a) c = static_cast<char>(tolower(c));

        if (a == "yes") return true;
        if (a == "no") {
            return false;
        }

        cout << Color::RED << "Please type yes or no.\n" << Color::RESET;
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
        {5, [this]{ PlaySound(TEXT("characterselection.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); viewMatchHistory(); }},
        {6, [this]{ if(confirmExit()){ clearScreen(); exit(0); } }}
    };

    while (true) {
        clearScreen();
        PlaySound(TEXT("characterselection.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); 
        cout << Color::BOLD_GREEN << "================ MAIN MENU ================" << Color::RESET << "\n";
        cout << " " << Color::CYAN << "1. Player vs Player" << Color::RESET << "\n";
        cout << " " << Color::WHITE<< "2. Player vs Computer" << Color::RESET << "\n";
        cout << " " << Color::YELLOW << "3. View All Characters" << Color::RESET << "\n";
        cout << " " << Color::MAGENTA << "4. Credits" << Color::RESET << "\n";
        cout << " " << Color::BLUE << "5. View Match Results" << Color::RESET << "\n";
        cout << " " << Color::RED << "6. Exit Game" << Color::RESET << "\n";
        cout << Color::BOLD_GREEN << "===========================================" << Color::RESET << "\n";
        cout << Color::BOLD_WHITE << "Enter choice: " << Color::RESET;

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            continue;
        }
        PlaySound(NULL, 0, 0); // Stop selection music before action
        clearScreen();

        auto it = menuActions.find(choice);
        if (it != menuActions.end()) {
            it->second();
        } else {
            cout << Color::RED << "Invalid choice. Please try again.\n\n" << Color::RESET;
        }
    }
}

// ===========================================
// ROSTER SETUP
// ===========================================

void Game::initRoster() {
    struct CharacterData {
        std::string name, title, bio, grudge;
        int hp, mana, baseDmg;
        std::vector<Skill> skills;
    };

    std::vector<CharacterData> data = {
        
        {"Arnold", "The Lover Boy",
         "Arnold used to write love letters to half the class...",
         "Arnold holds grudges against Kyle, Timothy, and Rolando.",
         100, 80, 18,
         { {"Heart Shot", "A focused blast of pure charm.", 10, false},
           {"Romantic Shield", "Softens incoming blows.", 12, false},
           {"Starlit Serenade", "A cosmic love song.", 18, false},
           {"Love Delete", "Deletes the enemy.", 0, true} }},
        
        {"Kyle", "The Master Beater",
         "Kyle dominates every combat exam.",
         "Kyle cannot stand Arnold's drama, Laurence's attitude, and Timothy's trash talk.",
         120, 60, 20,
         { {"Meteor Jab", "Fast galactic punches.", 10, false},
           {"Asteroid Uppercut", "Launches rivals.", 15, false},
           {"Orbit Breaker", "Breaks enemy rhythm.", 20, false},
           {"Galaxy Eraser", "One-hit erase.", 0, true} }},
        
        {"Laurence", "The Bitch Slayer",
         "Laurence was once quiet, until everyone pushed too far.",
         "Laurence has history with everyone and never forgets a slight.",
         110, 90, 17,
         { {"Nebula Slash", "Sharp space strike.", 8, false},
           {"Supernova Spin", "Starfire spin.", 14, false},
           {"Void Pressure", "Gravity crush.", 18, false},
           {"Oblivion Cut", "Dimensional delete.", 0, true} }},
        
        {"Timothy", "The Trash Talker",
         "Timothy starts fights with words, not punches.",
         "He roasts Arnold's heartbreaks, Kyle's ego, and Laurence's temper.",
         100, 100, 15,
         { {"Verbal Meteor", "Insults drop like rocks.", 8, false},
           {"Psychic Echo", "Painful echoes in the mind.", 12, false},
           {"Galaxy Roast", "Burns pride and HP.", 20, false},
           {"Silence of Space", "Cursed delete.", 0, true} }},
        
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
        //Character constructor takes (Name, Title, HP, Mana, BaseDmg, Color, Bio, Grudge)
        Character c(d.name, d.title, d.hp, d.mana, d.baseDmg, getCharacterColor(d.name), d.bio, d.grudge); 
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
        cout << Color::BOLD_MAGENTA << "===== VIEW ALL CHARACTERS =====" << Color::RESET << "\n\n";

        for (size_t i = 0; i < roster.size(); i++)
            cout << Color::WHITE << i+1 << ". " << getCharacterColor(roster[i].getName()) << roster[i].getName() << Color::RESET << "\n";

        cout << Color::WHITE << roster.size() + 1 << ". Back" << Color::RESET << "\n\n";

        cout << Color::BOLD_WHITE << "Choose a character: " << Color::RESET;
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
        else {
            cout << Color::RED << "Invalid character choice." << Color::RESET << "\n";
            Sleep(800);
        }
    }
}

void Game::displayCharacterDetails(int i) {
    clearScreen();

    Character& c = roster[i];

    cout << Color::BOLD_CYAN << "=========================================" << Color::RESET << "\n";
    cout << Color::BOLD_WHITE << "           CHARACTER PROFILE             " << Color::RESET << "\n";
    cout << Color::BOLD_CYAN << "=========================================" << Color::RESET << "\n\n";

    cout << Color::BOLD_WHITE << "Name : " << getCharacterColor(c.getName()) << c.getName() << Color::RESET << "\n";
    cout << Color::BOLD_WHITE << "Title: " << Color::YELLOW << c.getTitle() << Color::RESET << "\n\n";

    cout << Color::BOLD_VIOLET << "--- BACKSTORY ---" << Color::RESET << "\n" << c.getBio() << "\n\n";
    cout << Color::BOLD_VIOLET << "--- GRUDGES ---" << Color::RESET << "\n" << Color::WHITE << c.getGrudge() << Color::RESET << "\n\n";

    cout << left;
    cout << Color::BOLD_WHITE << setw(12) << "HP" << Color::RESET << Color::GREEN << ": " << c.getHP() << " / " << c.getMaxHP() << Color::RESET << "\n";
    cout << Color::BOLD_WHITE << setw(12) << "Mana" << Color::RESET << Color::BLUE << ": " << c.getMana() << " / " << c.getMaxMana() << Color::RESET << "\n";
    cout << Color::BOLD_WHITE << setw(12) << "Base Damage" << Color::RESET << Color::ORANGE << ": " << c.getBaseDamage() << Color::RESET << "\n\n";

    cout << Color::BOLD_WHITE << "=== SKILLS ===" << Color::RESET << "\n";
    const auto& s = c.getSkills();

    int min = c.getBaseDamage() - 3;
    int max = c.getBaseDamage() + 5;

    for (size_t z = 0; z < s.size(); z++) {
        cout << Color::CYAN << z+1 << ") " << s[z].name << Color::RESET << " (Mana: " << Color::BLUE << s[z].manaCost << Color::RESET << ")\n";
        cout << Color::WHITE << "    " << s[z].description << Color::RESET << "\n";
        if (s[z].isOneHitDelete)
            cout << Color::RED << "    Damage: ONE-HIT DELETE." << Color::RESET << "\n\n";
        else
            cout << Color::ORANGE << "    Est. Damage: " << min << "-" << max << Color::RESET << "\n\n";
    }

    cout << Color::BRIGHT_WHITE << "Press Enter..." << Color::RESET;
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
    const std::string& chColor = getCharacterColor(ch.getName());

    cout << Color::BOLD_WHITE << "Choose a skill for " << chColor << ch.getName() << Color::RESET << ":\n\n";

    for (size_t i = 0; i < skills.size(); i++) {
        const Skill& skill = skills[i];
        
        cout << Color::CYAN << i+1 << ") " << skill.name << Color::RESET 
             << " (Mana: " << Color::BLUE << skill.manaCost << Color::RESET << ")";

        if (skill.isOneHitDelete)
            cout << Color::BOLD_RED << " [DELETE]";
        else if (ch.getMana() < skill.manaCost)
            cout << Color::RED << " [LOW MANA]";
            
        cout << "\n" << Color::WHITE << "    " << skill.description << Color::RESET << "\n\n";
    }

    int suggested = getRandomInt(1, (int)skills.size());
    cout << Color::MAGENTA << "Random suggests: " << suggested << Color::RESET << "\n\n";

    int c;
    while (true) {
        cout << Color::BOLD_WHITE << "Enter skill number: " << Color::RESET;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (c >= 1 && c <= (int)skills.size()) break;
        cout << Color::RED << "Invalid.\n" << Color::RESET;
    }

    system("cls");
    return c-1;
}

int Game::computeDamage(Character& atk, Character& def, const Skill& s) {
    // ONE HIT DELETE
    if (s.isOneHitDelete) {
        cout << Color::BOLD_RED << "* ONE HIT DELETE ACTIVATED! *" << Color::RESET << "\n";
        return def.getHP();
    }

    bool basic = (s.name == BASIC_ATTACK.name);

    int base = atk.getBaseDamage();
    int rnd = getRandomInt(-3, 5);
    int dmg = base + rnd;

    if (!basic)
        dmg += s.manaCost;

    if (atk.getName() == "Arnold") {
        if (getRandomInt(1,100) <= 25) {
            cout << COLOR_ARNOLD << "Romantic Aura! +5 dmg + heal 5." << Color::RESET << "\n";
            dmg += 5;
            atk.heal(5);
        }
    }
    else if (atk.getName() == "Kyle") {
        if (getRandomInt(1,100) <= 20) {
            cout << COLOR_KYLE << "Combo Master! DOUBLE DAMAGE!" << Color::RESET << "\n";
            dmg *= 2;
        }
    }
    else if (atk.getName() == "Laurence") {
        if (def.getHP() < 40) {
            cout << COLOR_LAURENCE << "Relentless! +5 dmg." << Color::RESET << "\n";
            dmg += 5;
        }
    }
    else if (atk.getName() == "Timothy") {
        if (getRandomInt(1,100) <= 30) {
            cout << COLOR_TIMOTHY << "Mind Games! +5 dmg." << Color::RESET << "\n";
            dmg += 5;
        }
    }
    else if (atk.getName() == "Rolando") {
        if (getRandomInt(1,100) <= 25) {
            cout << COLOR_ROLANDO << "Galactic Fury! +5 dmg." << Color::RESET << "\n";
            dmg += 5;
        }
    }

    if (dmg < 0) dmg = 0;

    cout << Color::WHITE << "Final damage: " << dmg << Color::RESET << "\n\n";
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
        const std::string& chColor = getCharacterColor(ch.getName());

        cout << "\n";
        cout << Color::BOLD_WHITE << "STATUS WARNING" << Color::RESET << "\n"; 
        cout << "Player " << playerNumber
             << " (" << chColor << ch.getName() << Color::RESET << "): "
             << Color::RED << "HP: " << ch.getHP() << Color::RESET << " | Mana: " << ch.getMana() << "\n"; 
        cout << Color::BOLD_WHITE << "Warning: Player " << playerNumber
             << " (" << ch.getName() << ") has only " << ch.getHP() << " HP left." << Color::RESET << "\n";
        cout << Color::BOLD_WHITE << "Do you want to continue the battle or surrender?" << Color::RESET << "\n";
        
        cout << Color::GREEN << " 1. Continue" << Color::RESET << "\n";
        cout << Color::RED << " 2. Surrender" << Color::RESET << "\n";

        do {
            cout << Color::BOLD_WHITE << "Choose an option:(1/2): " << Color::RESET;

            if (cin >> choice) {
                if (choice == 1) {
                    clearScreen();
                    return true;
                } else if (choice == 2) {
                    clearScreen();
                    cout << Color::BOLD_RED << "Player " << playerNumber << " (" << ch.getName() << ") has surrendered!" << Color::RESET << "\n";
                    return false;
                }
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << Color::RED << "Invalid choice. Please select again.\n" << Color::RESET; 
            
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
        cout << Color::BOLD_GREEN << "===== PLAYER " << playerNumber << " -- CHOOSE YOUR CHARACTER =====" << Color::RESET << "\n\n";

        for (size_t i = 0; i < roster.size(); i++) {
            const std::string& chName = roster[i].getName();
            const std::string& chColor = getCharacterColor(chName);
            
            cout << Color::WHITE << i + 1 << ". " << chColor << chName << Color::RESET;
            
            if ((int)i == forbiddenIndex)
                cout << Color::RED << " (UNAVAILABLE)" << Color::RESET;
            
            cout << "\n";
        }

        cout << Color::WHITE << roster.size() + 1 << ". Back" << Color::RESET << "\n\n";

        int choice;
        cout << Color::BOLD_WHITE << "Enter choice: " << Color::RESET;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Back option
        if (choice == (int)roster.size() + 1) {
            Character back;
            back.setHP(-1);           // special flag
            back.setMaxHP(-1);
            return back;              // signal to cancel
        }

        if (choice >= 1 && choice <= (int)roster.size()) {
            int index = choice - 1;

            if (index == forbiddenIndex) {
                cout << Color::RED << "That character is unavailable. Choose another." << Color::RESET << "\n";
                Sleep(1000);
                continue;
            }

            clearScreen();
            displayCharacterDetails(index);

            if (showGrudges) {
                cout << Color::BOLD_WHITE << "\nThis character holds grudges against: "
                     << Color::WHITE << roster[index].getGrudge() << Color::RESET << "\n\n";
            }

        cout << Color::BOLD_WHITE << "Confirm select? " << Color::RESET << "(yes/no): ";
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
            cout << Color::RED << "Invalid choice." << Color::RESET << "\n";
            Sleep(800);
        }
    }
}


// ===========================================
// PVP MODE
// ===========================================

void Game::playPVP() {
    clearScreen();
    cout << Color::BOLD_YELLOW << "=============== PLAYER VS PLAYER ===============\n\n" << Color::RESET;

    Character p1 = chooseCharacter(1, true, -1);
    if (p1.getHP() == -1) return; 

    int forbidden = -1;
    for (size_t i = 0; i < roster.size(); i++) {
        if (roster[i].getName() == p1.getName())
        forbidden = (int)i;
    }

    Character p2 = chooseCharacter(2, true, forbidden);
    if (p2.getHP() == -1) return; 

    // MUSIC: begin battle music AFTER both players are chosen
    PlaySound(TEXT("battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    int w1 = 0, w2 = 0;
    const int winGoal = 3;

    for (int round = 1; round <= 5 && w1 < winGoal && w2 < winGoal; round++) {

        p1.resetForNewRound();
        p2.resetForNewRound();

        clearScreen();
        cout << Color::BOLD_WHITE << "================= ROUND " << round << " =================\n\n" << Color::RESET;

        while (p1.isAlive() && p2.isAlive()) {
            cout << Color::BOLD_WHITE << "STATUS:" << Color::RESET << "\n";
            cout << "P1: " << getCharacterColor(p1.getName()) << p1.getName() << Color::RESET 
                 << Color::GREEN << " HP: " << p1.getHP() << Color::RESET
                 << Color::BLUE << " Mana: " << p1.getMana() << Color::RESET << "\n";
            cout << "P2: " << getCharacterColor(p2.getName()) << p2.getName() << Color::RESET 
                 << Color::GREEN << " HP: " << p2.getHP() << Color::RESET 
                 << Color::BLUE << " Mana: " << p2.getMana() << Color::RESET << "\n\n";

            if (!handleLowHP(p1, 1, GameMode::PVP, true)) {
                w2++;
                break;
            }

            cout << Color::BOLD_CYAN << "--- PLAYER 1 TURN (" << p1.getName() << ") ---\n" << Color::RESET;
            int i1 = chooseSkill(p1);
            const Skill& s1 = p1.getSkills()[i1];

            const Skill* use1 = &s1;
            if (!s1.isOneHitDelete && p1.getMana() < s1.manaCost) {
                cout << Color::RED << "Not enough mana! Using basic attack.\n" << Color::RESET;
                use1 = &BASIC_ATTACK;
            } else p1.useMana(s1.manaCost);

            int dmg1 = computeDamage(p1, p2, *use1);
            p2.takeDamage(dmg1);

            if (!p2.isAlive()) break;

            if (!handleLowHP(p2, 2, GameMode::PVP, true)) {
                w1++;
                break;
            }

            cout << Color::BOLD_MAGENTA << "--- PLAYER 2 TURN (" << p2.getName() << ") ---\n" << Color::RESET;
            int i2 = chooseSkill(p2);
            const Skill& s2 = p2.getSkills()[i2];

            const Skill* use2 = &s2;
            if (!s2.isOneHitDelete && p2.getMana() < s2.manaCost) {
                cout << Color::RED << "Not enough mana! Using basic attack.\n" << Color::RESET;
                use2 = &BASIC_ATTACK;
            } else p2.useMana(s2.manaCost);

            int dmg2 = computeDamage(p2, p1, *use2);
            p1.takeDamage(dmg2);
        }

        if (p1.isAlive() && !p2.isAlive()) {
            cout << Color::BOLD_GREEN << "Player 1 (" << p1.getName() << ") wins round " << round << "!\n\n" << Color::RESET;
            w1++;
        }
        else if (p2.isAlive() && !p1.isAlive()) {
            cout << Color::BOLD_GREEN << "Player 2 (" << p2.getName() << ") wins round " << round << "!\n\n" << Color::RESET;
            w2++;
        }
        else if (!p1.isAlive() && !p2.isAlive()) {
             cout << Color::YELLOW << "Double K.O. in round " << round << "!" << Color::RESET << "\n\n";
        }


        cout << Color::BOLD_WHITE << "Current Score: " 
             << getCharacterColor(p1.getName()) << p1.getName() << Color::RESET << " = " << w1 << " | "
             << getCharacterColor(p2.getName()) << p2.getName() << Color::RESET << " = " << w2 << "\n\n";
                 
        if (round < 5 && w1 < winGoal && w2 < winGoal) {
            cout << Color::BRIGHT_WHITE << "Press Enter to continue..." << Color::RESET;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }
    }

    cout << Color::BOLD_YELLOW << "================ MATCH RESULT ================" << Color::RESET << "\n\n";
    string winner;
    if (w1 > w2) {
        cout << Color::BOLD_GREEN << "Player 1 (" << p1.getName() << ") is the champion!" << Color::RESET << "\n";
        winner = p1.getName();
    }
    else if (w2 > w1) {
        cout << Color::BOLD_GREEN << "Player 2 (" << p2.getName() << ") is the champion!" << Color::RESET << "\n";
        winner = p2.getName();
    }
    else {
        cout << Color::BOLD_WHITE << "The battle ends in a draw." << Color::RESET << "\n";
        winner = "Draw";
    }

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

    cout << Color::BRIGHT_WHITE << "\nPress Enter..." << Color::RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

// ===========================================
// PVC MODE
// ===========================================

void Game::playPVC() {
    clearScreen();
    cout << Color::BOLD_YELLOW << "============= PLAYER VS COMPUTER =============\n\n" << Color::RESET;

    
    Character p = chooseCharacter(1, false, -1);

    if (p.getHP() == -1) return;  

    int forbiddenIndex = -1;
    for (size_t i = 0; i < roster.size(); i++) {
        if (roster[i].getName() == p.getName()) {
            forbiddenIndex = (int)i;
            break;
        }
    }

    int aiIndex;
    do {
        aiIndex = getRandomInt(0, (int)roster.size() - 1);
    } while (aiIndex == forbiddenIndex);
    Character bot = roster[aiIndex];

    int pw = 0, cw = 0;
    const int winGoal = 3;

    cout << Color::BOLD_WHITE << "Computer chosen: " << getCharacterColor(bot.getName()) << bot.getName() << Color::YELLOW << " " << bot.getTitle() << Color::RESET << "\n";
    cout << Color::BRIGHT_WHITE << "Press Enter to start the battle..." << Color::RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    PlaySound(TEXT("battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    for (int r = 1; r <= 5 && pw < winGoal && cw < winGoal; r++) {

        p.resetForNewRound();
        bot.resetForNewRound();

        
        bot.setMaxHP(p.getMaxHP() + 50); 
        bot.setHP(p.getMaxHP() + 50);

        clearScreen();
        cout << Color::BOLD_WHITE << "================= ROUND " << r << " =================\n\n" << Color::RESET;

        while (p.isAlive() && bot.isAlive()) {

            cout << Color::BOLD_WHITE << "STATUS:" << Color::RESET << "\n";
            cout << "You: " << getCharacterColor(p.getName()) << p.getName() << Color::RESET 
                 << Color::GREEN << " HP: " << p.getHP() << Color::RESET 
                 << Color::BLUE << " Mana: " << p.getMana() << Color::RESET << "\n";
            cout << "Bot: " << getCharacterColor(bot.getName()) << bot.getName() << Color::RESET 
                 << Color::GREEN << " HP: " << bot.getHP() << Color::RESET 
                 << Color::BLUE << " Mana: " << bot.getMana() << Color::RESET << "\n\n";

            if (!handleLowHP(p, 1, GameMode::PVC, true)) {
                cw++;
                break;
            }

            cout << Color::BOLD_CYAN << "--- YOUR TURN (" << p.getName() << ") ---\n" << Color::RESET;
            int psi = chooseSkill(p);
            const Skill& ps = p.getSkills()[psi];

            const Skill* useP = &ps;
            if (!ps.isOneHitDelete && p.getMana() < ps.manaCost) {
                cout << Color::RED << "Not enough mana! Using basic attack.\n" << Color::RESET;
                useP = &BASIC_ATTACK;
            }
            else
                p.useMana(ps.manaCost);

            int pdmg = computeDamage(p, bot, *useP);
            bot.takeDamage(pdmg);

            if (!bot.isAlive()) break;

            cout << Color::BOLD_MAGENTA << "--- BOT TURN (" << bot.getName() << ") ---\n" << Color::RESET;

            std::vector<int> availableSkills;
            for (size_t i = 0; i < bot.getSkills().size(); i++) {
                if (!bot.getSkills()[i].isOneHitDelete)
                    availableSkills.push_back((int)i);
            }

            int aiS = availableSkills[getRandomInt(0, (int)availableSkills.size() - 1)];
            const Skill& as = bot.getSkills()[aiS];

            const Skill* useA = &as;
            if (bot.getMana() < as.manaCost) {
                cout << Color::WHITE << bot.getName() << " uses Basic Attack!\n" << Color::RESET;
                useA = &BASIC_ATTACK;
            }
            else {
                cout << getCharacterColor(bot.getName()) << bot.getName() << " uses " << as.name << "!\n" << Color::RESET;
                bot.useMana(as.manaCost);
            }

            int admg = computeDamage(bot, p, *useA);
            p.takeDamage(admg);
        }

        if (p.isAlive() && !bot.isAlive()) {
            cout << Color::BOLD_GREEN << "You win round " << r << "!\n" << Color::RESET;
            pw++;
        } else if (bot.isAlive() && !p.isAlive()) {
            cout << Color::BOLD_RED << "Bot wins round " << r << "!\n" << Color::RESET;
            cw++;
        } else if (!p.isAlive() && !bot.isAlive()) {
             cout << Color::YELLOW << "Double K.O. in round " << r << "!" << Color::RESET << "\n\n";
        }


        cout << Color::BOLD_WHITE << "Current Score: You (" 
             << getCharacterColor(p.getName()) << p.getName() << Color::RESET << ") = " << pw << " | Bot ("
             << getCharacterColor(bot.getName()) << bot.getName() << Color::RESET << ") = " << cw << "\n\n";

        if (r < 5 && pw < winGoal && cw < winGoal) {
            cout << Color::BRIGHT_WHITE << "Press Enter..." << Color::RESET;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }
    }

    cout << Color::BOLD_YELLOW << "================ MATCH RESULT ================" << Color::RESET << "\n\n";
    string winner;
    if (pw > cw) {
        cout << Color::BOLD_GREEN << "You are the champion!" << Color::RESET << "\n";
        winner = p.getName();
    } else if (cw > pw) {
        cout << Color::BOLD_RED << "The Computer wins!" << Color::RESET << "\n";
        winner = bot.getName();
    } else {
        cout << Color::BOLD_WHITE << "Match ends in a draw." << Color::RESET << "\n";
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

    cout << Color::BRIGHT_WHITE << "\nPress Enter..." << Color::RESET;
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
        Color::BOLD_YELLOW + "================ CREDITS ================" + Color::RESET,
        Color::BOLD_WHITE + "Game Design      : Dave Laurence R. Repe" + Color::RESET,
        Color::BOLD_WHITE + "Programming      : Arnold Michael P. Tabada" + Color::RESET + "\n" +
        Color::BOLD_WHITE + "               : Nirhevn Kyle Dialimas" + Color::RESET + "\n" +
        Color::BOLD_WHITE + "               : John Timothy Cabuguas" + Color::RESET + "\n" +
        Color::BOLD_WHITE + "               : Dave Laurence R. Repe" + Color::RESET + "\n",
        Color::BOLD_WHITE + "Artwork          : John Timothy Cabuguas" + Color::RESET,
        Color::BOLD_WHITE + "Story & Lore     : John Timothy Cabuguas" + Color::RESET,
        Color::BOLD_RED + "Special Thanks   : Rolando Supremo" + Color::RESET,
        Color::BOLD_YELLOW + "========================================" + Color::RESET,
        "",
        Color::BOLD_GREEN + "Thank you for playing Galactica Campus Brawl!" + Color::RESET,
        Color::BOLD_GREEN + "Hope you enjoyed your adventure!" + Color::RESET,
        "",
        Color::BRIGHT_WHITE + "Press Enter to go back..." + Color::RESET
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

    PlaySound(NULL, 0, 0);

    clearScreen();
}
