#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// ─────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────
const int PLAYER_MAX_HP      = 100;
const int PLAYER_ATTACK      = 18;
const int POWER_METER_MAX    = 3;   // fills after 3 normal attacks
const int SPECIAL_DAMAGE     = 45;
const int DEFEND_REDUCTION   = 10;  // flat damage blocked when defending

const int ENEMY_MAX_HP       = 120;
const int ENEMY_ATTACK_MIN   = 10;
const int ENEMY_ATTACK_MAX   = 22;

// ─────────────────────────────────────────────
//  Utility helpers
// ─────────────────────────────────────────────
void pause(int ms = 600) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void printLine(char c = '-', int len = 52) {
    std::cout << std::string(len, c) << "\n";
}

int clamp(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

int randRange(int lo, int hi) {
    return lo + std::rand() % (hi - lo + 1);
}

// ─────────────────────────────────────────────
//  Structs
// ─────────────────────────────────────────────
struct Player {
    std::string name;
    int hp;
    int maxHp;
    int attack;
    int powerMeter;     // current charge (0 – POWER_METER_MAX)

    bool isAlive() const { return hp > 0; }
};

struct Enemy {
    std::string name;
    int hp;
    int maxHp;
    int attackMin;
    int attackMax;

    bool isAlive() const { return hp > 0; }
};

// ─────────────────────────────────────────────
//  Display helpers
// ─────────────────────────────────────────────
std::string hpBar(int current, int max, int width = 20) {
    int filled = (current * width) / max;
    filled = clamp(filled, 0, width);
    return "[" + std::string(filled, '#') + std::string(width - filled, '.') + "]";
}

std::string powerBar(int current, int max) {
    std::string bar = "[";
    for (int i = 0; i < max; ++i)
        bar += (i < current) ? '*' : '-';
    bar += "]";
    return bar;
}

void displayStatus(const Player& p, const Enemy& e) {
    printLine('=');
    std::cout << " PLAYER  " << p.name << "\n";
    std::cout << "   HP    " << hpBar(p.hp, p.maxHp)
              << "  " << p.hp << "/" << p.maxHp << "\n";
    std::cout << "   POWER " << powerBar(p.powerMeter, POWER_METER_MAX)
              << "  " << p.powerMeter << "/" << POWER_METER_MAX << "\n";
    printLine('-');
    std::cout << " ENEMY   " << e.name << "\n";
    std::cout << "   HP    " << hpBar(e.hp, e.maxHp)
              << "  " << e.hp << "/" << e.maxHp << "\n";
    printLine('=');
}

void displayActions(const Player& p) {
    std::cout << "\n  Choose your action:\n";
    std::cout << "  [1] Attack        (deals ~" << p.attack << " damage, +1 power)\n";
    std::cout << "  [2] Defend        (reduces next hit by " << DEFEND_REDUCTION << ")\n";
    if (p.powerMeter >= POWER_METER_MAX)
        std::cout << "  [3] SPECIAL MOVE  (deals " << SPECIAL_DAMAGE << " damage!) [READY!]\n";
    else
        std::cout << "  [3] Special Move  (locked - power not full)\n";
    std::cout << "\n  > ";
}

// ─────────────────────────────────────────────
//  Core battle logic
// ─────────────────────────────────────────────
void playerTurn(Player& player, Enemy& enemy, bool& defending) {
    displayActions(player);

    int choice = 0;
    std::cin >> choice;

    // Validate input
    while (std::cin.fail() || choice < 1 || choice > 3 ||
           (choice == 3 && player.powerMeter < POWER_METER_MAX)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        if (choice == 3 && player.powerMeter < POWER_METER_MAX)
            std::cout << "  Power meter not full yet! Choose 1 or 2.\n  > ";
        else
            std::cout << "  Invalid choice. Enter 1, 2, or 3.\n  > ";
        std::cin >> choice;
    }
    std::cin.ignore(1000, '\n');

    std::cout << "\n";
    pause(400);

    switch (choice) {
        case 1: { // Normal attack
            int dmg = randRange(player.attack - 3, player.attack + 5);
            enemy.hp = clamp(enemy.hp - dmg, 0, enemy.maxHp);
            player.powerMeter = clamp(player.powerMeter + 1, 0, POWER_METER_MAX);
            defending = false;
            std::cout << "  ⚔  " << player.name << " attacks " << enemy.name
                      << " for " << dmg << " damage!\n";
            std::cout << "  (+1 power charge)\n";
            break;
        }
        case 2: { // Defend
            defending = true;
            std::cout << "  🛡  " << player.name << " takes a defensive stance!\n";
            std::cout << "  (Incoming damage reduced by " << DEFEND_REDUCTION << ")\n";
            break;
        }
        case 3: { // Special move
            int dmg = randRange(SPECIAL_DAMAGE - 5, SPECIAL_DAMAGE + 5);
            enemy.hp = clamp(enemy.hp - dmg, 0, enemy.maxHp);
            player.powerMeter = 0;
            defending = false;
            std::cout << "  ✨ " << player.name << " unleashes the SPECIAL MOVE!\n";
            std::cout << "  ★★★ CRITICAL HIT for " << dmg << " damage! ★★★\n";
            std::cout << "  (Power meter reset)\n";
            break;
        }
    }
    pause(700);
}

void enemyTurn(Player& player, const Enemy& enemy, bool defending) {
    int dmg = randRange(enemy.attackMin, enemy.attackMax);
    if (defending) {
        dmg = clamp(dmg - DEFEND_REDUCTION, 0, dmg);
        std::cout << "\n  💥 " << enemy.name << " attacks " << player.name << "!\n";
        std::cout << "  Shield absorbs " << DEFEND_REDUCTION
                  << " damage — " << player.name << " takes " << dmg << " damage.\n";
    } else {
        std::cout << "\n  💥 " << enemy.name << " attacks " << player.name
                  << " for " << dmg << " damage!\n";
    }
    player.hp = clamp(player.hp - dmg, 0, player.maxHp);
    pause(700);
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // ── Title screen ──
    printLine('=');
    std::cout << "        *** ARENA OF SHADOWS ***\n";
    std::cout << "           Turn-Based Battle\n";
    printLine('=');
    std::cout << "\n";

    // ── Get player name ──
    std::string playerName;
    std::cout << "  Enter your warrior's name: ";
    std::getline(std::cin, playerName);
    if (playerName.empty()) playerName = "Hero";

    // ── Initialise characters ──
    Player player;
    player.name       = playerName;
    player.hp         = PLAYER_MAX_HP;
    player.maxHp      = PLAYER_MAX_HP;
    player.attack     = PLAYER_ATTACK;
    player.powerMeter = 0;

    Enemy enemy;
    enemy.name      = "Dark Knight";
    enemy.hp        = ENEMY_MAX_HP;
    enemy.maxHp     = ENEMY_MAX_HP;
    enemy.attackMin = ENEMY_ATTACK_MIN;
    enemy.attackMax = ENEMY_ATTACK_MAX;

    std::cout << "\n  Welcome, " << player.name << "!\n";
    std::cout << "  You face the fearsome " << enemy.name << "!\n\n";
    pause(1000);

    // ── Battle loop ──
    int turn    = 1;
    bool defend = false;

    while (player.isAlive() && enemy.isAlive()) {
        std::cout << "\n";
        printLine('~');
        std::cout << "  TURN " << turn << "\n";
        printLine('~');

        displayStatus(player, enemy);

        // Player's turn
        playerTurn(player, enemy, defend);

        if (!enemy.isAlive()) break;   // enemy defeated mid-turn

        // Enemy's turn
        enemyTurn(player, enemy, defend);
        defend = false;  // defence only lasts one hit

        ++turn;
    }

    // ── End screen ──
    std::cout << "\n";
    printLine('#');
    if (player.isAlive()) {
        std::cout << "\n  🏆 VICTORY!\n";
        std::cout << "  " << player.name << " defeats the " << enemy.name << "!\n";
        std::cout << "  Remaining HP: " << player.hp << "/" << player.maxHp << "\n";
    } else {
        std::cout << "\n  💀 GAME OVER!\n";
        std::cout << "  " << player.name << " has fallen...\n";
        std::cout << "  The " << enemy.name << " stands victorious.\n";
    }
    std::cout << "\n";
    printLine('#');
    std::cout << "\n  Thanks for playing! Press Enter to quit.\n";
    std::cin.ignore(1000, '\n');
    std::cin.get();

    return 0;
}