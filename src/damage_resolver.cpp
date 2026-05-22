#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>

// 🔹 Pure logic: NO I/O inside. Easily unit-testable & network-sync friendly.
float calculate_damage(float atk, float def_percent, int atk_type, int armor_type) {
    float dmg = atk;

    // 1. Elemental multiplier
    if (atk_type == 1) { // Fire attack
        if (armor_type == 2) dmg *= 1.2f;      // +20% vs Ice armor
        else if (armor_type == 1) dmg *= 0.8f; // -20% vs Fire armor
    } else if (atk_type == 2) { // Ice attack
        if (armor_type == 1) dmg *= 1.2f;      // +20% vs Fire armor
        else if (armor_type == 2) dmg *= 0.8f; // -20% vs Ice armor
    }
    // Physical (3) = no modifier

    // 2. Defense reduction: capped at 50%
    float reduction = std::min(def_percent, 50.0f) / 100.0f;
    dmg *= (1.0f - reduction);

    // 3. Clamp to 0 (no negative damage)
    return std::max(dmg, 0.0f);
}

// 🔹 Safe input helper (reusable)
float get_float(const std::string& prompt) {
    float val;
    while (true) {
        std::cout << prompt;
        std::cin >> val;
        if (std::cin.fail() || val < 0.0f) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid. Enter a non-negative number.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
    }
}

int get_type(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        std::cin >> val;
        if (std::cin.fail() || val < 1 || val > 3) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid. Enter 1, 2, or 3.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
    }
}

int main() {
    std::cout << "=== Combat Damage Resolver ===\n";
    std::cout << "Types: 1=Fire, 2=Ice, 3=Physical\n\n";

    float atk = get_float("Player Attack: ");
    float def = get_float("Enemy Defense (%): ");
    int atk_type = get_type("Attack Type: ");
    int armor_type = get_type("Enemy Armor Type: ");

    float final_dmg = calculate_damage(atk, def, atk_type, armor_type);

    std::cout << "\n⚔️ Final Damage: " << std::fixed << std::setprecision(1) << final_dmg << "\n";
    return 0;
}