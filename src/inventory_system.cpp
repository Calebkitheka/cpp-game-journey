#include <iostream>
#include <string>
#include <limits>

// 🔹 Fixed-size inventory (prelude to std::vector next week)
const int MAX_SLOTS = 5;
std::string inventory[MAX_SLOTS];
int current_size = 0;

// 🔹 Reusable input sanitizer
void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 🔹 Safe integer picker (min/max inclusive)
int get_choice(int min, int max) {
    int val;
    while (true) {
        std::cout << "Choice: ";
        std::cin >> val;
        if (std::cin.fail() || val < min || val > max) {
            clear_input();
            std::cout << "❌ Invalid. Enter " << min << "-" << max << ".\n";
        } else {
            clear_input(); // Consume trailing newline
            return val;
        }
    }
}

// 🔹 Pure-ish logic functions (minimal I/O)
bool add_item(const std::string& item) {
    if (current_size >= MAX_SLOTS) return false;
    inventory[current_size++] = item;
    return true;
}

bool remove_item(int index) {
    if (index < 0 || index >= current_size) return false;
    
    // Shift elements left to fill gap (O(n) operation)
    for (int i = index; i < current_size - 1; ++i) {
        inventory[i] = inventory[i + 1];
    }
    inventory[--current_size].clear();
    return true;
}

void list_items() {
    if (current_size == 0) {
        std::cout << "📦 Inventory is empty.\n";
        return;
    }
    std::cout << "🎒 Current Inventory:\n";
    for (int i = 0; i < current_size; ++i) {
        std::cout << "  [" << i << "] " << inventory[i] << "\n";
    }
}

// 🔹 COORDINATOR: main() routes menu actions
int main() {
    std::cout << "=== RPG Inventory Manager ===\n";
    bool running = true;

    while (running) {
        std::cout << "\n📜 Menu: 1.Add  2.Remove  3.List  4.Quit\n";
        switch (get_choice(1, 4)) {
            case 1: {
                std::string item;
                std::cout << "Item name: ";
                std::getline(std::cin, item);
                if (add_item(item)) std::cout << "✅ Added: " << item << "\n";
                else std::cout << "❌ Inventory full! Max " << MAX_SLOTS << " slots.\n";
                break;
            }
            case 2: {
                list_items();
                if (current_size > 0) {
                    int idx = get_choice(0, current_size - 1);
                    if (remove_item(idx)) std::cout << "🗑️ Removed.\n";
                }
                break;
            }
            case 3: list_items(); break;
            case 4: running = false; std::cout << "👋 Exiting...\n"; break;
        }
    }
    return 0;
}