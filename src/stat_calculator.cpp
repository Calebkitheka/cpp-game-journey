#include <iostream>
#include <limits>
#include <iomanip>

int main() {
    std::cout << "=== RPG Stat Calculator ===\n";

    float base_hp, base_mana;
    int level;

    // --- Input: Base HP ---
    while (true) {
        std::cout << "Enter Base HP: ";
        std::cin >> base_hp;

        if (std::cin.fail()) {
            std::cin.clear(); // Reset error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
            std::cout << "❌ Invalid input. Please enter a number.\n";
        } else if (base_hp < 0) {
            std::cout << "❌ HP cannot be negative.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear trailing newline
            break;
        }
    }

    // --- Input: Base Mana ---
    while (true) {
        std::cout << "Enter Base Mana: ";
        std::cin >> base_mana;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid input. Please enter a number.\n";
        } else if (base_mana < 0) {
            std::cout << "❌ Mana cannot be negative.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }

    // --- Input: Level ---
    while (true) {
        std::cout << "Enter Level: ";
        std::cin >> level;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Invalid input. Please enter a number.\n";
        } else if (level < 1) {
            std::cout << "❌ Level must be at least 1.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }

    // --- Calculations ---
    // Cast int level to float to avoid mixed-type warnings
    float final_hp = base_hp + (static_cast<float>(level) * 10.0f);
    float final_mana = base_mana * 1.5f;

    // --- Output ---
    std::cout << "\n=== Final Stats ===\n";
    std::cout << std::fixed << std::setprecision(1); // Force 1 decimal place
    std::cout << "Final HP: " << final_hp << "\n";
    std::cout << "Final Mana: " << final_mana << "\n";

    return 0;
}