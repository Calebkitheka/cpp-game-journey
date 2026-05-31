#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// 🔹 Global state (Will become structs/classes in Week 3)
const int MAX_POTIONS = 3;
int potions = 0;
float player_hp, max_hp;
float enemy_hp, max_enemy_hp = 100.0f;
int turn = 1;

// 🔹 Input sanitizers
void clear_cin() { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }

int get_int(const std::string& p, int mn, int mx) {
    int v;
    while(true) { 
        std::cout << p; std::cin >> v; 
        if(std::cin.fail() || v < mn || v > mx) { clear_cin(); std::cout << "❌ Enter " << mn << "-" << mx << "\n"; } 
        else { clear_cin(); return v; } 
    }
}

float get_float(const std::string& p) {
    float v;
    while(true) { 
        std::cout << p; std::cin >> v; 
        if(std::cin.fail() || v < 0.0f) { clear_cin(); std::cout << "❌ Enter number >= 0\n"; } 
        else { clear_cin(); return v; } 
    }
}

// 🔹 Pure logic (zero side effects, deterministic)
float calc_dmg(float atk, float def, int type, int armor) {
    float d = atk;
    if(type == 1 && armor == 2) d *= 1.2f;
    else if(type == 2 && armor == 1) d *= 1.2f;
    d *= (1.0f - std::min(def, 50.0f) / 100.0f);
    return std::max(d, 0.0f);
}

// 🔹 Game systems
void use_potion() {
    if(potions > 0 && player_hp < max_hp) {
        potions--;
        player_hp = std::min(player_hp + 30.0f, max_hp);
        std::cout << "💚 +30 HP\n";
    } else std::cout << "❌ None or full HP\n";
}

void player_turn() {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\n[Turn " << turn << "] You:" << player_hp << "/" << max_hp 
              << " | Boss:" << enemy_hp << "/" << max_enemy_hp << " | 🧪:" << potions << "\n";
    std::cout << "1.Attack  2.Heal  3.Search\n";
    int c = get_int("> ", 1, 3);

    if(c == 1) {
        int t = get_int("Type (1=Fire, 2=Ice, 3=Phys): ", 1, 3);
        enemy_hp = std::max(enemy_hp - calc_dmg(25.0f, 20.0f, t, 1), 0.0f);
    } else if(c == 2) {
        use_potion();
    } else if(c == 3) {
        if(rand() % 2 == 0) { 
            if(potions < MAX_POTIONS) potions++; 
            std::cout << "📦 Found potion!\n"; 
        } else std::cout << "🔍 Empty.\n";
    }
}

void enemy_turn() {
    if(enemy_hp <= 0) return;
    float dmg = calc_dmg(18.0f, 15.0f, 1, 2); // Boss uses Fire vs player's Ice armor
    player_hp = std::max(player_hp - dmg, 0.0f);
    std::cout << "👹 Boss hits for " << dmg << "\n";
}

// 🔹 COORDINATOR: <15 lines, purely orchestrates
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::cout << "=== Arena Survivor (Week 1 Capstone) ===\n";
    max_hp = player_hp = get_float("Base HP: ") + (get_int("Level: ", 1, 10) * 10.0f);

    while(player_hp > 0.0f && enemy_hp > 0.0f) {
        player_turn();
        if(enemy_hp > 0.0f) enemy_turn();
        turn++;
    }

    std::cout << "\n🏆 " << (player_hp > 0.0f ? "VICTORY!" : "DEFEAT!") << "\n";
    return 0;
}