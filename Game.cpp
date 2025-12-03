#include "Game.h"
#include "Character.h"
#include "Skill.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

// --- Platform Specific Includes for clearScreen and typeText ---
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// --- ANSI Escape Code Definitions (Cross-Platform) ---
namespace Color {
    // Basic ANSI control codes
    const std::string RESET = "\033[0m";
    
    // Foreground Colors (Normal)
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string WHITE = "\033[37m"; 
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string GRAY = "\033[90m";       // Light Gray
    const std::string ORANGE = "\033[93m";
    const std::string BROWN = "\033[33m";      // Brown uses same ANSI as yellow
    const std::string VIOLET = "\033[38;2;138;43;226m";

    // Foreground Colors (Bold/Bright)
    const std::string BOLD = "\033[1m";
    const std::string BOLD_RED = "\033[1;31m";
    const std::string BOLD_GREEN = "\033[1;32m";
    const std::string BOLD_YELLOW = "\033[1;33m";
    const std::string BOLD_BLUE = "\033[1;34m";
    const std::string BRIGHT_WHITE = "\033[97m";  // Bright white
    const std::string BOLD_WHITE = "\033[1;97m";   // Bold bright white
    const std::string BOLD_MAGENTA = "\033[1;35m";
    const std::string BOLD_CYAN = "\033[1;36m";
    const std::string BOLD_ORANGE = "\033[1;93m";
    const std::string BOLD_BROWN = "\033[1;33m";
    const std::string BOLD_VIOLET = "\033[1;38;2;138;43;226m";
}


// --- Character-Specific Color Macros (Updated to Use ANSI Strings) ---
#define COLOR_GRAY Color::GRAY
#define COLOR_ARNOLD Color::BOLD_YELLOW
#define COLOR_KYLE Color::BOLD_BLUE
#define COLOR_LAURENCE Color::BOLD_MAGENTA
#define COLOR_TIMOTHY Color::WHITE
#define COLOR_ROLANDO Color::BOLD_RED
#define COLOR_ORANGE Color::ORANGE
#define COLOR_BROWN Color::BROWN
#define COLOR_VIOLET Color::VIOLET
#define COLOR_WHITE Color::WHITE

using namespace std;

// --- Helper Functions for Colors (Simplified to use cout) ---

// THIS IS THE CRITICAL FUNCTION FOR WINDOWS
#ifdef _WIN32
bool enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return false;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return false;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) return false;
    return true;
}
#endif

// Helper function to print the color string
void printColor(const std::string& colorCode) {
    cout << colorCode;
}

void resetConsoleColor() { 
    cout << Color::RESET; 
}

// Helper function to get color string for a character name
const std::string& getCharColorCode(const std::string& name) {
    if (name == "Arnold") return COLOR_ARNOLD;
    if (name == "Kyle") return COLOR_KYLE;
    if (name == "Laurence") return COLOR_LAURENCE;
    if (name == "Timothy") return COLOR_TIMOTHY;
    if (name == "Rolando") return COLOR_ROLANDO;
    return Color::RESET;
}

void typeText(const string &text, int delayMs) {
    for (char c : text) {
        cout << c << flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

// --- Game Constructor and Utilities ---

Game::Game() {
#ifdef _WIN32
    // Enable ANSI/Virtual Terminal Processing on Windows
    enableVirtualTerminalProcessing();
#endif
    std::random_device rd;
    rng.seed(rd());
    initRoster();
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

bool Game::confirmExit() {
    while (true) {
        printColor(Color::BOLD_YELLOW);
        cout << "\nAre you sure to exit the game? Type ";
        printColor(Color::BOLD_GREEN);
        cout << "yes";
        resetConsoleColor();
        cout << "/";
        printColor(Color::BOLD_RED);
        cout << "no";
        resetConsoleColor();
        cout << ": ";
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
            printColor(Color::BOLD_RED);
            cout << "Please type yes or no.\n";
            resetConsoleColor();
        }
    }
}

void Game::showIntro() {
    printColor(Color::BOLD_YELLOW);
    typeText("==================================================\n", 2);
    printColor(Color::BOLD_CYAN);
    typeText("            ROLANDO GALACTIC GRAVEYARD              \n", 2);
    printColor(Color::BOLD_YELLOW);
    typeText("==================================================\n\n", 2);
    resetConsoleColor();

    printColor(Color::WHITE);
    typeText("  Welcome to Rolando Galactic Graveyard, a floating school\n", 2);
    typeText("  in deep space where students settle their rivalries\n", 2);
    typeText("  in the legendary battle arena.\n\n", 2);
    resetConsoleColor();

    printColor(Color::WHITE);
    typeText("  Choose your fighter, unleash your skills,\n", 2);
    typeText("  and prove who rules the stars.\n\n", 2);
    resetConsoleColor();

    cout << "  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    clearScreen();
}

void Game::showCredits() {
    clearScreen();
    printColor(Color::BOLD_MAGENTA);
    cout << "================== CREDITS ==================\n";
    resetConsoleColor();
    printColor(Color::CYAN);
    cout << "Game Design & Concept: [Your Name/Team]\n";
    cout << "Programming: [Your Name/Team]\n";
    cout << "Story/Lore: Created for Galactica Campus Brawl\n";
    resetConsoleColor();
    cout << "\n";
    printColor(Color::GREEN);
    cout << "Special Thanks to: The Galactica Academy Roster\n";
    resetConsoleColor();
    printColor(Color::BOLD_MAGENTA);
    cout << "===========================================\n\n";
    resetConsoleColor();

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

int Game::getRandomInt(int min, int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// --- Main Menu and Game Mode Selection ---

void Game::mainMenu() {
    while (true) {
        printColor(Color::BOLD_MAGENTA);
        cout << "================ MAIN MENU ================\n";
        resetConsoleColor();
        printColor(Color::CYAN);
        cout << "  1. Player vs Player\n";
        resetConsoleColor();
        printColor(Color::GRAY);
        cout << "  2. Player vs Computer\n";
        resetConsoleColor();
        printColor(Color::ORANGE);
        cout << "  3. View All Characters\n";
        resetConsoleColor();
        printColor(Color::BROWN);
        cout << "  4. Credits\n";
        resetConsoleColor();
        printColor(Color::RED);
        cout << "  5. Exit Game\n";
        resetConsoleColor();
        printColor(Color::BOLD_MAGENTA);
        cout << "===========================================\n";
        resetConsoleColor();
        printColor(Color::BOLD_YELLOW);
        cout << "Enter choice: ";
        resetConsoleColor();

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            printColor(Color::BOLD_RED);
            cout << "Invalid input. Please try again.\n\n";
            resetConsoleColor();
            continue;
        }

        clearScreen();

        if (choice == 1) {
            playPVP();
        } else if (choice == 2) {
            playPVC();
        } else if (choice == 3) {
            viewAllCharacters();
        } else if (choice == 4) {
            showCredits();
        } else if (choice == 5) {
            if (confirmExit()) {
                clearScreen();
                printColor(Color::RED);
                cout << "Exiting Galactica Campus Brawl...\n";
                cout << "See you next orbit, cadet.\n\n";
                resetConsoleColor();
                break;
            } else {
                clearScreen();
            }
        } else {
            printColor(Color::BOLD_RED);
            cout << "Invalid choice. Please try again.\n\n";
            resetConsoleColor();
        }
    }
}

// --- Character Roster and Viewing (initRoster is unchanged, omitted for brevity) ---
// ... (initRoster function remains the same) ...

void Game::initRoster() {
    // Arnold
    Character arnold(
        "Arnold", "The Lover Boy", 100, 80, 18,
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
    arnold.addSkill({"Love Delete", "A forbidden letter that deletes the enemy from his story.", 0, true});

    // Kyle
    Character kyle(
        "Kyle", "The Master Beater", 120, 60, 20,
        "Combo Master: 20% chance to double his attack.",
        "Kyle dominates every training exam and combat class, "
        "famous for finishing practice fights before the timer starts.",
        "Kyle cannot stand Arnold's drama, Laurence's attitude, "
        "and Timothy's trash talk. He swore to beat them all in the arena."
    );
    kyle.addSkill({"Meteor Jab", "Fast galactic punches straight to the jaw.", 10, false});
    kyle.addSkill({"Asteroid Uppercut", "A heavy uppercut that launches rivals skyward.", 15, false});
    kyle.addSkill({"Orbit Breaker", "A ruthless combo that breaks enemy rhythm.", 20, false});
    kyle.addSkill({"Galaxy Eraser", "A clean, brutal hit that erases the opponent.", 0, true});

    // Laurence
    Character laurence(
        "Laurence", "The Bitch Slayer", 110, 90, 17,
        "Relentless: Deals +5 damage against low HP opponents.",
        "Laurence was once quiet, until everyone pushed too far. "
        "Now no one dares cross him twice.",
        "Laurence has history with everyone. He remembers every insult "
        "from Timothy, every flex from Kyle, and every dramatic scene from Arnold."
    );
    laurence.addSkill({"Nebula Slash", "A sharp strike that cuts through space dust.", 8, false});
    laurence.addSkill({"Supernova Spin", "A spinning slash powered by starfire.", 14, false});
    laurence.addSkill({"Void Pressure", "Gravity crushes the enemy from all sides.", 18, false});
    laurence.addSkill({"Oblivion Cut", "A one-hit dimensional cut that sends foes into nothingness.", 0, true});

    // Timothy
    Character timothy(
        "Timothy", "The Trash Talker", 100, 100, 15,
        "Mind Games: 30% chance to add extra psychic damage.",
        "Timothy never throws the first punch, just the first insult. "
        "Most battles start because of his mouth.",
        "Timothy roasts everyone: Arnold's heartbreak, Kyle's ego, "
        "Laurence's temper, and Rolando's dramatic entrances."
    );
    timothy.addSkill({"Verbal Meteor", "Insults that land like falling rocks.", 8, false});
    timothy.addSkill({"Psychic Echo", "Words that echo painfully in the mind.", 12, false});
    timothy.addSkill({"Galaxy Roast", "An interstellar insult that burns pride and HP.", 20, false});
    timothy.addSkill({"Silence of Space", "One cursed sentence that deletes the target.", 0, true});

    // Rolando
    Character rolando(
        "Rolando", "Galactic Slayer", 130, 70, 19,
        "Galactic Fury: 25% chance to deal +5 bonus damage.",
        "Rolando is a transfer student rumored to have cleared whole simulations alone.",
        "Rolando feels above petty drama, but deep down he is tired of Kyle's bragging, "
        "Timothy's comments, and being treated like a final boss by everyone."
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


void Game::printCharacterCard(const Character &ch, int index) {
    const std::string& color = getCharColorCode(ch.getName());
    printColor(Color::YELLOW);
    cout << "  " << index << ") ";
    resetConsoleColor();
    printColor(color);
    cout << ch.getName() << " " << ch.getTitle();
    resetConsoleColor();
    cout << "\n";
    printColor(Color::GREEN);
    cout << "    HP: " << ch.getMaxHP();
    resetConsoleColor();
    cout << "  ";
    printColor(Color::BLUE);
    cout << "Mana: " << ch.getMaxMana();
    resetConsoleColor();
    cout << "  ";
    printColor(Color::YELLOW);
    cout << "Base Damage: " << ch.getBaseDamage();
    resetConsoleColor();
    cout << "\n";
    printColor(Color::MAGENTA);
    cout << "    Passive: " << ch.getPassiveDesc();
    resetConsoleColor();
    cout << "\n";
}

void Game::displayCharacterDetails(int index) {
    using std::cout;
    using std::vector;

    if (index < 0 || index >= static_cast<int>(roster.size())) return;

    const Character &ch = roster[index];
    // Assumes getCharColorCode is defined, perhaps in Game.h
    const std::string& color = getCharColorCode(ch.getName()); 
    // Assumes Skill struct is defined and Character::getSkills() returns it

    clearScreen();
    printColor(Color::BOLD_GREEN);
    cout << "============================================\n";
    resetConsoleColor();
    
    cout << " "; // Use single space for alignment
    printColor(color);
    cout << ch.getName() << " " << ch.getTitle();
    resetConsoleColor();
    cout << "\n";
    
    printColor(Color::BOLD_GREEN);
    cout << "============================================\n\n";
    resetConsoleColor();

    // STATS
    printColor(Color::WHITE);
    cout << "STATS:\n";
    resetConsoleColor();
    cout << "\n"; // Added newline for clear separation
    
    // Formatting stats to ensure labels are colored along with values
    cout << " ";
    printColor(Color::GREEN);
    cout << "HP: " << ch.getMaxHP();
    resetConsoleColor();
    
    cout << " | "; // Separator
    
    printColor(Color::BLUE);
    cout << "Mana: " << ch.getMaxMana();
    resetConsoleColor();
    
    cout << " | "; // Separator
    
    printColor(Color::RED);
    cout << "Base Damage: " << ch.getBaseDamage();
    resetConsoleColor();
    cout << "\n\n";

    // PASSIVE
    printColor(Color::MAGENTA);
    cout << "PASSIVE:\n";
    resetConsoleColor();
    cout << " ";
    printColor(COLOR_GRAY);
    cout << ch.getPassiveDesc();
    resetConsoleColor();
    cout << "\n\n";

    // STORY
    printColor(Color::BLUE);
    cout << "STORY:\n";
    resetConsoleColor();
    printColor(COLOR_GRAY);
    cout << " " << ch.getBio() << "\n\n";
    resetConsoleColor();

    // GRUDGES
    printColor(COLOR_VIOLET); // Changed from Color::VIOLET to COLOR_VIOLET (macro)
    cout << "GRUDGES (Brawl Targets):\n";
    resetConsoleColor();
    printColor(COLOR_GRAY);
    cout << " " << ch.getGrudge() << "\n\n";
    resetConsoleColor();

    // SKILLS
    printColor(Color::CYAN);
    cout << "SKILLS:\n";
    resetConsoleColor();
    const vector<Skill> &skills = ch.getSkills();
    for (size_t i = 0; i < skills.size(); ++i) {
        cout << "\n "; // Newline before each skill item for separation
        
        // Skill Number, Name, and Mana Cost
        printColor(Color::CYAN);
        cout << i + 1 << ") " << skills[i].name;
        resetConsoleColor();
        
        printColor(Color::YELLOW);
        cout << " (Mana: " << skills[i].manaCost << ")";
        resetConsoleColor();
        cout << "\n";

        // Skill Description
        printColor(COLOR_GRAY);
        cout << "    " << skills[i].description;
        resetConsoleColor();
        
        // Ultimate Tag
        if (skills[i].isOneHitDelete) {
            printColor(Color::BOLD_RED);
            cout << " [ULTIMATE / ONE-HIT]";
            resetConsoleColor();
        }
        cout << "\n"; // End of skill block
    }
}

void Game::viewAllCharacters() {
    while (true) {
        printColor(Color::BOLD_BLUE);
        cout << "================ GALACTIC ROSTER =================\n\n";
        resetConsoleColor();
        for (size_t i = 0; i < roster.size(); ++i) {
            printCharacterCard(roster[i], static_cast<int>(i) + 1);
        }
        printColor(Color::BOLD_BLUE);
        cout << "================================================\n";
        resetConsoleColor();

        printColor(Color::BOLD_YELLOW);
        cout << "Enter character number for details (or 0 to return): ";
        resetConsoleColor();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            printColor(Color::BOLD_RED);
            cout << "Invalid input. Please try again.\n\n";
            resetConsoleColor();
            continue;
        }

        if (choice == 0) {
            clearScreen();
            return;
        }

        if (choice >= 1 && choice <= static_cast<int>(roster.size())) {
            displayCharacterDetails(choice - 1);
            cout << "Press Enter to return to the roster...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        } else {
            clearScreen();
            printColor(Color::BOLD_RED);
            cout << "Invalid choice. Please try again.\n\n";
            resetConsoleColor();
        }
    }
}

// --- Combat Flow and Selection ---

Character Game::chooseCharacter(int playerNumber, bool showGrudges, int forbiddenIndex) {
    while (true) {
        printColor(Color::BOLD_BLUE);
        cout << "=============== PLAYER " << playerNumber << " ===============\n";
        resetConsoleColor();
        printColor(Color::BLUE);
        cout << "Choose your fighter from Galactica Academy:\n\n";
        resetConsoleColor();

        for (size_t i = 0; i < roster.size(); ++i) {
            int idx = static_cast<int>(i);
            const std::string& color = getCharColorCode(roster[i].getName());
            printColor(Color::YELLOW);
            cout << "  " << idx + 1 << ") ";
            resetConsoleColor();
            printColor(color);
            cout << roster[i].getName()
                      << " " << roster[i].getTitle();
            resetConsoleColor();
            if (idx == forbiddenIndex) {
                printColor(Color::BOLD_RED);
                cout << "  [TAKEN]";
                resetConsoleColor();
            }
            cout << "\n";
        }

        printColor(Color::BOLD_YELLOW);
        cout << "\nEnter choice (1-" << roster.size() << "): ";
        resetConsoleColor();

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            printColor(Color::BOLD_RED);
            cout << "Invalid input. Please try again.\n\n";
            resetConsoleColor();
            continue;
        }

        int index = choice - 1;
        if (index < 0 || index >= static_cast<int>(roster.size())) {
            clearScreen();
            printColor(Color::BOLD_RED);
            cout << "Invalid choice. Please try again.\n\n";
            resetConsoleColor();
            continue;
        }

        if (index == forbiddenIndex) {
            clearScreen();
            printColor(Color::BOLD_RED);
            cout << "That character is already chosen by the other player.\n";
            cout << "Please select another fighter.\n\n";
            resetConsoleColor();
            continue;
        }

        Character chosen = roster[index];
        chosen.resetForNewRound();

        displayCharacterDetails(index); // Reusing details display for confirmation

        cout << "\nPress Enter to CONFIRM selection...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        clearScreen();
        return chosen;
    }
}

int Game::chooseSkill(const Character &ch) {
    const vector<Skill> &skills = ch.getSkills();
    const std::string& color = getCharColorCode(ch.getName());

    printColor(Color::BOLD_YELLOW);
    cout << "Select a skill for ";
    printColor(color);
    cout << ch.getName()
              << " " << ch.getTitle();
    resetConsoleColor();
    printColor(Color::BOLD_YELLOW);
    cout << ":\n\n";
    resetConsoleColor();

    for (size_t i = 0; i < skills.size(); ++i) {
        printColor(Color::GREEN);
        cout << "  " << i + 1 << ") ";
        resetConsoleColor();
        printColor(Color::CYAN);
        cout << skills[i].name;
        resetConsoleColor();
        printColor(Color::YELLOW);
        cout << "  (Mana: " << skills[i].manaCost << ")";
        resetConsoleColor();
        cout << "\n";
        printColor(COLOR_GRAY);
        cout << "    " << skills[i].description;
        resetConsoleColor();
        if (skills[i].isOneHitDelete) {
            printColor(Color::BOLD_RED);
            cout << "  [ONE-HIT DELETE]";
            resetConsoleColor();
        }
        cout << "\n\n";
    }

    int choice;
    while (true) {
        printColor(Color::BOLD_YELLOW);
        cout << "Enter skill number (1-" << skills.size() << "): ";
        resetConsoleColor();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printColor(Color::BOLD_RED);
            cout << "Invalid input. Please try again.\n";
            resetConsoleColor();
            continue;
        }
        if (choice >= 1 && choice <= static_cast<int>(skills.size())) {
            break;
        }
        printColor(Color::BOLD_RED);
        cout << "Invalid skill. Please try again.\n";
        resetConsoleColor();
    }

    clearScreen();
    return choice - 1;
}

int Game::computeDamage(Character &attacker, Character &defender, const Skill &skill) {
    const std::string& attackerColor = getCharColorCode(attacker.getName());

    // One-hit delete: no mana requirement
    if (skill.isOneHitDelete) {
        printColor(Color::BOLD_RED);
        cout << "*** ONE-HIT DELETE ACTIVATED! ***\n";
        resetConsoleColor();
        cout << "  ";
        printColor(attackerColor);
        cout << attacker.getName();
        resetConsoleColor();
        cout << " uses " << skill.name
             << " to erase the opponent in a single strike.\n\n";
        return defender.getHP();
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
            printColor(attackerColor);
            cout << "  Passive triggered: Romantic Aura! +5 damage.\n";
            resetConsoleColor();
            damage += 5;
            attacker.heal(5);
            printColor(attackerColor);
            cout << "  " << attacker.getName()
                 << " heals 5 HP from his charm. (Current HP: " << attacker.getHP() << ")\n";
            resetConsoleColor();
        }
    } else if (attacker.getName() == "Kyle") {
        if (getRandomInt(1, 100) <= 20) {
            printColor(attackerColor);
            cout << "  Passive triggered: Combo Master! Damage doubled.\n";
            resetConsoleColor();
            damage *= 2;
        }
    } else if (attacker.getName() == "Laurence") {
        if (defender.getHP() < 40) {
            printColor(attackerColor);
            cout << "  Passive triggered: Relentless! +5 damage.\n";
            resetConsoleColor();
            damage += 5;
        }
    } else if (attacker.getName() == "Timothy") {
        if (getRandomInt(1, 100) <= 30) {
            printColor(attackerColor);
            cout << "  Passive triggered: Mind Games! Extra 5 psychic damage.\n";
            resetConsoleColor();
            damage += 5;
        }
    } else if (attacker.getName() == "Rolando") {
        if (getRandomInt(1, 100) <= 25) {
            printColor(attackerColor);
            cout << "  Passive triggered: Galactic Fury! +5 damage.\n";
            resetConsoleColor();
            damage += 5;
        }
    }

    if (damage < 0) {
        damage = 0;
    }

    printColor(Color::BOLD_RED);
    cout << "  Final damage after passives: " << damage << "\n\n";
    resetConsoleColor();
    return damage;
}

bool Game::handleLowHP(Character &ch, int playerNumber, GameMode mode, bool isHuman) {
    using namespace std; // Using namespace std locally for convenience

    (void)mode; // Suppress unused parameter warning
    if (!isHuman) return true; // Computer automatically continues

    // Check if HP is low and character is alive
    if (ch.getHP() <= 15 && ch.isAlive()) {
        int choice;

        // --- Print the Warning and Options (Printed Once) ---
        cout << "\n";
        printColor(Color::BOLD_RED);
        cout << "STATUS\n"; // Added STATUS header for clarity, matching the image
        cout << "Player " << playerNumber
             << " (" << ch.getName() << "): HP: " << ch.getHP() << " | Mana: 25\n"; // Mock Mana display based on image
        cout << "Warning: Player " << playerNumber
             << " (" << ch.getName() << ") has only " << ch.getHP() << " HP left.\n";
        resetConsoleColor();
        cout << "Do you want to continue the battle or surrender?\n";
        
        printColor(Color::GREEN);
        cout << " 1. Continue\n";
        resetConsoleColor();
        
        printColor(Color::RED);
        cout << " 2. Surrender\n";
        resetConsoleColor();

        // --- Input and Validation Loop ---
        do {
            // Prompt for choice (Printed on loop entry and after invalid input)
            printColor(Color::BOLD_YELLOW);
            cout << "Choose: ";
            resetConsoleColor();

            if (cin >> choice) {
                // Input was successfully read as an integer
                if (choice == 1) {
                    clearScreen();
                    return true; // Continue
                } else if (choice == 2) {
                    clearScreen();
                    printColor(Color::BOLD_RED);
                    cout << "Player " << playerNumber << " (" << ch.getName() << ") has surrendered!\n";
                    resetConsoleColor();
                    return false; // Surrender
                }
            } else {
                // Input failed (e.g., user entered text)
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            // If we reach this point, the input was invalid (not 1 or 2, or not an integer)
            printColor(Color::BOLD_RED);
            // Display error message, immediately followed by the prompt in the next iteration
            cout << "Invalid choice. Please select again.\n"; 
            resetConsoleColor();
            
        } while (true); // Loop continues until a valid choice (1 or 2) is returned.
    }

    return true; // If HP is not low enough to trigger the warning, continue the game.
}

// --- Game Modes ---

void Game::playPVP() {
    clearScreen();
    printColor(Color::BOLD_MAGENTA);
    cout << "=============== PLAYER VS PLAYER ===============\n\n";
    resetConsoleColor();

    Character p1 = chooseCharacter(1, true, -1);
    
    int forbiddenIndex = -1;
    for (size_t i = 0; i < roster.size(); ++i) {
        if (roster[i].getName() == p1.getName()) {
            forbiddenIndex = static_cast<int>(i);
            break;
        }
    }

    Character p2 = chooseCharacter(2, true, forbiddenIndex);
    const std::string& p1Color = getCharColorCode(p1.getName());
    const std::string& p2Color = getCharColorCode(p2.getName());

    int p1Wins = 0;
    int p2Wins = 0;
    const int roundsToWin = 3;

    for (int round = 1; round <= 5 && p1Wins < roundsToWin && p2Wins < roundsToWin; ++round) {

        p1.resetForNewRound();
        p2.resetForNewRound();

        clearScreen();
        printColor(Color::BOLD_CYAN);
        cout << "================= ROUND " << round << " =================\n\n";
        resetConsoleColor();

        bool roundOver = false;

        while (p1.isAlive() && p2.isAlive() && !roundOver) {

            printColor(Color::BOLD_YELLOW);
            cout << "STATUS\n";
            resetConsoleColor();
            cout << "  Player 1 ("; printColor(p1Color); cout << p1.getName(); resetConsoleColor();
            cout << "): "; printColor(Color::GREEN); cout << "HP: " << p1.getHP(); resetConsoleColor();
            cout << " | "; printColor(Color::BLUE); cout << "Mana: " << p1.getMana(); resetConsoleColor(); cout << "\n";
            
            cout << "  Player 2 ("; printColor(p2Color); cout << p2.getName(); resetConsoleColor();
            cout << "): "; printColor(Color::GREEN); cout << "HP: " << p2.getHP(); resetConsoleColor();
            cout << " | "; printColor(Color::BLUE); cout << "Mana: " << p2.getMana(); resetConsoleColor(); cout << "\n\n";

            // --- Player 1 turn ---
            if (!handleLowHP(p1, 1, GameMode::PVP, true)) {
                p2Wins++;
                roundOver = true;
                break;
            }

            printColor(Color::BOLD_MAGENTA);
            cout << "------------- PLAYER 1 TURN (YOURS) -------------\n\n";
            resetConsoleColor();
            int skillIndex1 = chooseSkill(p1);
            const Skill &s1 = p1.getSkills()[skillIndex1];

            if (p1.getMana() < s1.manaCost && !s1.isOneHitDelete) {
                printColor(Color::RED);
                cout << "Not enough mana for " << s1.name
                     << ". Using basic attack (no mana cost) instead.\n\n";
                resetConsoleColor();
            } else if (!s1.isOneHitDelete) {
                p1.useMana(s1.manaCost);
            }

            int dmg1 = computeDamage(p1, p2, s1);
            printColor(p1Color); cout << "Player 1 uses " << s1.name; resetConsoleColor();
            cout << " and deals ";
            printColor(Color::RED); cout << dmg1; resetConsoleColor();
            cout << " damage!\n\n";
            p2.takeDamage(dmg1);

            printColor(Color::CYAN);
            cout << "Remaining HP and Mana:\n";
            resetConsoleColor();
            cout << "  P1 - HP: " << p1.getHP() << " | Mana: " << p1.getMana() << "\n";
            cout << "  P2 - HP: " << p2.getHP() << " | Mana: " << p2.getMana() << "\n\n";

            if (!p2.isAlive()) break;

            // --- Player 2 turn ---
            if (!handleLowHP(p2, 2, GameMode::PVP, true)) {
                p1Wins++;
                roundOver = true;
                break;
            }

            printColor(Color::BOLD_MAGENTA);
            cout << "------------- PLAYER 2 TURN (OPPONENT) -------------\n\n";
            resetConsoleColor();
            int skillIndex2 = chooseSkill(p2);
            const Skill &s2 = p2.getSkills()[skillIndex2];

            if (p2.getMana() < s2.manaCost && !s2.isOneHitDelete) {
                printColor(Color::RED);
                cout << "Not enough mana for " << s2.name
                     << ". Using basic attack (no mana cost) instead.\n\n";
                resetConsoleColor();
            } else if (!s2.isOneHitDelete) {
                p2.useMana(s2.manaCost);
            }

            int dmg2 = computeDamage(p2, p1, s2);
            printColor(p2Color); cout << "Player 2 uses " << s2.name; resetConsoleColor();
            cout << " and deals ";
            printColor(Color::RED); cout << dmg2; resetConsoleColor();
            cout << " damage!\n\n";
            p1.takeDamage(dmg2);

            printColor(Color::CYAN);
            cout << "Remaining HP and Mana:\n";
            resetConsoleColor();
            cout << "  P1 - HP: " << p1.getHP() << " | Mana: " << p1.getMana() << "\n";
            cout << "  P2 - HP: " << p2.getHP() << " | Mana: " << p2.getMana() << "\n\n";
            
            cout << "Press Enter to continue to the next turn...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }

        // Round results
        clearScreen();
        if (p1.isAlive() && !p2.isAlive()) {
            printColor(Color::BOLD_GREEN);
            cout << p1.getName() << " wins Round " << round << "!\n\n";
            resetConsoleColor();
            p1Wins++;
        } else if (!p1.isAlive() && p2.isAlive()) {
            printColor(Color::BOLD_RED);
            cout << p2.getName() << " wins Round " << round << "!\n\n";
            resetConsoleColor();
            p2Wins++;
        } 

        printColor(Color::BOLD_CYAN);
        cout << "Scoreboard:\n";
        resetConsoleColor();
        cout << "  Player 1 ("; printColor(p1Color); cout << p1.getName(); resetConsoleColor(); cout << "): " << p1Wins << " wins\n";
        cout << "  Player 2 ("; printColor(p2Color); cout << p2.getName(); resetConsoleColor(); cout << "): " << p2Wins << " wins\n\n";

        if (round < 5 && p1Wins < roundsToWin && p2Wins < roundsToWin) {
            cout << "Press Enter to proceed to the next round...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }

    printColor(Color::BOLD_BLUE);
    cout << "================ MATCH RESULT ================\n\n";
    resetConsoleColor();
    if (p1Wins > p2Wins) {
        printColor(Color::BOLD_GREEN);
        cout << "Player 1 (" << p1.getName() << ") is the champion of Galactica Academy!\n\n";
        resetConsoleColor();
    } else if (p2Wins > p1Wins) {
        printColor(Color::BOLD_RED);
        cout << "Player 2 (" << p2.getName() << ") is the champion of Galactica Academy!\n\n";
        resetConsoleColor();
    } else {
        printColor(Color::YELLOW);
        cout << "It ends in a cosmic draw.\n\n";
        resetConsoleColor();
    }

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}

void Game::playPVC() {
    clearScreen();
    printColor(Color::BOLD_MAGENTA);
    cout << "============= PLAYER VS COMPUTER =============\n\n";
    resetConsoleColor();

    Character player = chooseCharacter(1, false, -1);

    int forbiddenIndex = -1;
    for (size_t i = 0; i < roster.size(); ++i) {
        if (roster[i].getName() == player.getName()) {
            forbiddenIndex = static_cast<int>(i);
            break;
        }
    }

    int aiIndex;
    do {
        aiIndex = getRandomInt(0, static_cast<int>(roster.size()) - 1);
    } while (aiIndex == forbiddenIndex);

    Character computer = roster[aiIndex];

    int playerWins = 0;
    int computerWins = 0;
    const int roundsToWin = 3;
    const std::string& playerColor = getCharColorCode(player.getName());
    const std::string& computerColor = getCharColorCode(computer.getName());

    printColor(Color::BOLD_RED);
    cout << "The Computer has appeared as:\n\n";
    resetConsoleColor();
    cout << "  "; printColor(computerColor);
    cout << computer.getName() << " " << computer.getTitle();
    resetConsoleColor();
    cout << "\n\nPress Enter to begin the trial...";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    for (int round = 1; round <= 5 && playerWins < roundsToWin && computerWins < roundsToWin; ++round) {

        player.resetForNewRound();
        computer.resetForNewRound();

        // Boost Computer HP to 240 each round (from original logic)
        computer.setMaxHP(240);
        computer.setHP(240);

        clearScreen();
        printColor(Color::BOLD_CYAN);
        cout << "================= ROUND " << round << " (Computer HP: 240) =================\n\n";
        resetConsoleColor();

        while (player.isAlive() && computer.isAlive()) {

            printColor(Color::BOLD_YELLOW);
            cout << "STATUS\n";
            resetConsoleColor();
            
            cout << "  You ("; printColor(playerColor); cout << player.getName(); resetConsoleColor();
            cout << "): "; printColor(Color::GREEN); cout << "HP: " << player.getHP(); resetConsoleColor();
            cout << " | "; printColor(Color::BLUE); cout << "Mana: " << player.getMana(); resetConsoleColor(); cout << "\n";
            
            cout << "  Computer ("; printColor(computerColor); cout << computer.getName(); resetConsoleColor();
            cout << "): "; printColor(Color::GREEN); cout << "HP: " << computer.getHP(); resetConsoleColor();
            cout << " | "; printColor(Color::BLUE); cout << "Mana: " << computer.getMana(); resetConsoleColor(); cout << "\n\n";

            // --- Player turn ---
            if (!handleLowHP(player, 1, GameMode::PVC, true)) {
                computerWins++;
                break;
            }

            printColor(Color::BOLD_MAGENTA);
            cout << "------------- YOUR TURN -------------\n\n";
            resetConsoleColor();
            int skillIndexPlayer = chooseSkill(player);
            const Skill &sp = player.getSkills()[skillIndexPlayer];

            if (player.getMana() < sp.manaCost && !sp.isOneHitDelete) {
                printColor(Color::RED);
                cout << "Not enough mana for " << sp.name
                     << ". Using basic attack instead.\n\n";
                resetConsoleColor();
            } else if (!sp.isOneHitDelete) {
                player.useMana(sp.manaCost);
            }

            int dmgP = computeDamage(player, computer, sp);
            printColor(playerColor); cout << "You use " << sp.name; resetConsoleColor();
            cout << " and deals ";
            printColor(Color::RED); cout << dmgP; resetConsoleColor();
            cout << " damage!\n\n";
            computer.takeDamage(dmgP);

            printColor(Color::CYAN);
            cout << "Remaining HP and Mana:\n";
            resetConsoleColor();
            cout << "  You      - HP: " << player.getHP()
                 << " | Mana: " << player.getMana() << "\n";
            cout << "  Computer - HP: " << computer.getHP()
                 << " | Mana: " << computer.getMana() << "\n\n";

            if (!computer.isAlive()) break;

            // --- Computer turn ---
            printColor(Color::BOLD_MAGENTA);
            cout << "------------- COMPUTER TURN -------------\n\n";
            resetConsoleColor();
            
            const auto& aiSkills = computer.getSkills();
            std::vector<int> affordableSkills;
            for(size_t i = 0; i < aiSkills.size(); ++i) {
                if(aiSkills[i].isOneHitDelete || computer.getMana() >= aiSkills[i].manaCost) {
                    affordableSkills.push_back(static_cast<int>(i));
                }
            }

            if (affordableSkills.empty()) {
                 affordableSkills.push_back(0); 
            }
            
            int aiSkillIndex = affordableSkills[getRandomInt(0, static_cast<int>(affordableSkills.size()) - 1)];

            const Skill &sa = computer.getSkills()[aiSkillIndex];

            if (!sa.isOneHitDelete) {
                computer.useMana(sa.manaCost);
            }

            int dmgA = computeDamage(computer, player, sa);

            // Scale computer damage lower each round 
            double scale = 1.0 - 0.1 * (round - 1);
            if (scale < 0.5) scale = 0.5;
            int scaledDmgA = static_cast<int>(dmgA * scale);
            if (scaledDmgA < 0) scaledDmgA = 0;
            
            printColor(Color::BOLD_RED);
            cout << " (Computer Damage Scaled by " << (scale * 100) << "%)" << "\n";
            resetConsoleColor();

            printColor(computerColor); cout << "Computer uses " << sa.name; resetConsoleColor();
            cout << " and deals ";
            printColor(Color::RED); cout << scaledDmgA; resetConsoleColor();
            cout << " damage!\n\n";
            player.takeDamage(scaledDmgA);

            printColor(Color::CYAN);
            cout << "Remaining HP and Mana:\n";
            resetConsoleColor();
            cout << "  You      - HP: " << player.getHP()
                 << " | Mana: " << player.getMana() << "\n";
            cout << "  Computer - HP: " << computer.getHP()
                 << " | Mana: " << computer.getMana() << "\n\n";
                 
            cout << "Press Enter to continue to the next turn...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }

        // Round End Logic
        clearScreen();
        if (player.isAlive() && !computer.isAlive()) {
            printColor(Color::BOLD_GREEN);
            cout << "You win Round " << round << "!\n\n";
            resetConsoleColor();
            playerWins++;
        } else if (!player.isAlive() && computer.isAlive()) {
            printColor(Color::BOLD_RED);
            cout << "The Computer wins Round " << round << "!\n\n";
            resetConsoleColor();
            computerWins++;
        }

        printColor(Color::BOLD_CYAN);
        cout << "Scoreboard:\n";
        resetConsoleColor();
        cout << "  You: "; printColor(playerColor); cout << playerWins; resetConsoleColor(); cout << " wins\n";
        cout << "  Computer: "; printColor(computerColor); cout << computerWins; resetConsoleColor(); cout << " wins\n\n";

        if (round < 5 && playerWins < roundsToWin && computerWins < roundsToWin) {
            cout << "Press Enter to proceed to the next round...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }

    printColor(Color::BOLD_BLUE);
    cout << "================ MATCH RESULT ================\n\n";
    resetConsoleColor();
    if (playerWins > computerWins) {
        printColor(Color::BOLD_GREEN);
        cout << "You are the champion of Galactica Academy!\n\n";
        resetConsoleColor();
    } else if (computerWins > playerWins) {
        printColor(Color::BOLD_RED);
        cout << "The Computer is the champion of Galactica Academy!\n\n";
        resetConsoleColor();
    } else {
        printColor(Color::YELLOW);
        cout << "It ends in a cosmic draw.\n\n";
        resetConsoleColor();
    }

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clearScreen();
}