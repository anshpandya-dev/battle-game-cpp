#include <iostream>
#include <string>
#include <cstdlib>
// #include <ctime>

using namespace std;

// ── Constants ──────────────────────────────────────────
const int POWER_METER_MAX = 3;

// ── Player Class ───────────────────────────────────────
class Player {
public:
    string name;
    int hp;
    int attack;
    int powerMeter;

    Player(string playerName) {
        name       = playerName;
        hp         = 100;
        attack     = 15;
        powerMeter = 0;
    }

    bool isAlive() {
        return hp > 0;
    }

    void showStatus() {
        cout << "Player Health: " << hp << endl;
        cout << "Power: " << powerMeter << "/" << POWER_METER_MAX << endl;
    }

    // Normal attack: deals fixed damage and increases power meter
    void attackEnemy(int &enemyHp) {
        int damage = attack;
        enemyHp -= damage;
        if (enemyHp < 0) enemyHp = 0;

        if (powerMeter < POWER_METER_MAX)
            powerMeter++;

        cout << name << " attacks for " << damage << " damage!" << endl;
        cout << "Power meter: " << powerMeter << "/" << POWER_METER_MAX << endl;
    }

    // Special move: deals high damage, resets power meter
    void specialMove(int &enemyHp) {
        int damage = 40;
        enemyHp -= damage;
        if (enemyHp < 0) enemyHp = 0;

        powerMeter = 0;

        cout << name << " uses SPECIAL MOVE for " << damage << " damage!" << endl;
        cout << "Power meter reset to 0." << endl;
    }

    // Defend: reduces incoming damage by a fixed amount
    void defend() {
        cout << name << " defends! Incoming damage will be reduced." << endl;
    }
};

// ── Enemy Class ────────────────────────────────────────
class Enemy {
public:
    string name;
    int hp;

    Enemy() {
        name = "Dark Knight";
        hp   = 120;
    }

    bool isAlive() {
        return hp > 0;
    }

    void showStatus() {
        cout << "Enemy Health: " << hp << endl;
    }

    // Enemy attacks the player with a random damage value
    void attackPlayer(int &playerHp, bool playerDefending) {
        int damage = 10 + rand() % 11;  // random damage between 10 and 20

        if (playerDefending) {
            damage -= 8;
            if (damage < 0) damage = 0;
            cout << name << " attacks, but the defense reduced it to " << damage << " damage!" << endl;
        } else {
            cout << name << " attacks for " << damage << " damage!" << endl;
        }

        playerHp -= damage;
        if (playerHp < 0) playerHp = 0;
    }
};

// ── Main ───────────────────────────────────────────────
int main() {
    srand(time(0));

    // Get player name
    string playerName;
    cout << "Enter your name: ";
    getline(cin, playerName);
    if (playerName.empty()) playerName = "Hero";

    // Create player and enemy
    Player player(playerName);
    Enemy  enemy;

    cout << "\nWelcome, " << player.name << "!" << endl;
    cout << "You are fighting: " << enemy.name << endl;
    cout << "-------------------------------" << endl;

    int turn = 1;

    // ── Battle loop ────────────────────────────────────
    while (player.isAlive() && enemy.isAlive()) {

        cout << "\n--- Turn " << turn << " ---" << endl;
        player.showStatus();
        enemy.showStatus();

        // Show available actions
        cout << "\nChoose an action:" << endl;
        cout << "1. Attack" << endl;
        cout << "2. Defend" << endl;
        if (player.powerMeter >= POWER_METER_MAX)
            cout << "3. Special Move (READY!)" << endl;
        else
            cout << "3. Special Move (locked - need full power)" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        // Validate input
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
            continue;
        }
        if (choice == 3 && player.powerMeter < POWER_METER_MAX) {
            cout << "Power meter is not full yet!" << endl;
            continue;
        }

        // Track if player chose to defend this turn
        bool playerDefending = false;

        // ── Player's action ────────────────────────────
        if (choice == 1) {
            player.attackEnemy(enemy.hp);
        } else if (choice == 2) {
            player.defend();
            playerDefending = true;
        } else if (choice == 3) {
            player.specialMove(enemy.hp);
        }

        // Check if enemy is defeated
        if (!enemy.isAlive()) {
            break;
        }

        // ── Enemy's action ─────────────────────────────
        enemy.attackPlayer(player.hp, playerDefending);

        turn++;
    }

    // ── End result ─────────────────────────────────────
    cout << "\n===============================" << endl;
    if (player.isAlive()) {
        cout << "VICTORY! " << player.name << " defeated " << enemy.name << "!" << endl;
        cout << "Remaining HP: " << player.hp << endl;
    } else {
        cout << "GAME OVER! " << player.name << " was defeated." << endl;
    }
    cout << "===============================" << endl;

    return 0;
}
