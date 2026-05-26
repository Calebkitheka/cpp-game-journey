#include <iostream>
#include <limits>
#include <iomanip>
#include <algorithm>

// 🔹 FORWARD DECLARATIONS (Header-style contract)
float get_valid_float(const std::string& prompt, float min_val = 0.0f);
int get_valid_int(const std::string& prompt, int min_val = 1);
float calculate_final_hp(float base_hp, int level);
float calculate_damage(float atk, float def, int atk_type, int armor_type);
void simulate_battle_turn(float& player_hp, float& enemy_hp);

// 🔹 FUNCTION DEFINITIONS
float get_valid_float(const std::string& prompt, float min_val) {
    float val;
    while (true) {
        std::cout << prompt;
        std::cin >> val;
        if (std::cin.fail() || val < min_val) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid. Enter a valid number.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
    }
}

int get_valid_int(const std::string& prompt, int min_val) {
    int val;
    while (true) {
        std::cout << prompt;
        std::cin >> val;
        if (std::cin.fail() || val < min_val) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid. Enter a valid number.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
    }
}

// 🔹 Pure logic: NO I/O, deterministic, easily testable
float calculate_final_hp(float base_hp, int level) {
    return base_hp + (static_cast<float>(level) * 10.0f);
}

float calculate_damage(float atk, float def, int atk_type, int armor_type) {
    float dmg = atk;
    if (atk_type == 1 && armor_type == 2) dmg *= 1.2f;
    else if (atk_type == 2 && armor_type == 1) dmg *= 1.2f;
    
    float reduction = std::min(def, 50.0f) / 100.0f;
    dmg *= (1.0f - reduction);
    return std::max(dmg, 0.0f);
}

// 🔹 Pass-by-reference (`&`) modifies caller's state directly
void simulate_battle_turn(float& player_hp, float& enemy_hp) {
    int p_type = get_valid_int("Player Attack (1=Fire, 2=Ice, 3=Phys): ", 1);
    int e_type = get_valid_int("Enemy Armor  (1=Fire, 2=Ice, 3=Phys): ", 1);
    p_type = std::min(p_type, 3); e_type = std::min(e_type, 3);

    float dmg_to_enemy = calculate_damage(25.0f, 15.0f, p_type, e_type);
    float dmg_to_player = calculate_damage(20.0f, 10.0f, e_type, p_type);

    enemy_hp -= dmg_to_enemy;
    player_hp -= dmg_to_player;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "⚔️ You dealt " << dmg_to_enemy << " | Enemy HP: " << std::max(enemy_hp, 0.0f) << "\n";
    std::cout << "🛡️ They dealt " << dmg_to_player << " | Your HP:  " << std::max(player_hp, 0.0f) << "\n";
}

// 🔹 COORDINATOR PATTERN: main() ONLY orchestrates
int main() {
    std::cout << "=== Turn-Based Battle System ===\n";
    
    float p_hp = calculate_final_hp(get_valid_float("Base HP: "), get_valid_int("Level: ", 1));
    float e_hp = 100.0f;
    int turn = 1;

    while (p_hp > 0.0f && e_hp > 0.0f) {
        std::cout << "\n--- Turn " << turn++ << " ---\n";
        simulate_battle_turn(p_hp, e_hp);
    }

    std::cout << "\n🏆 Battle Over! " << (p_hp > 0.0f ? "Player Wins!" : "Enemy Wins!") << "\n";
    return 0;
}