#include <iostream>
#include <string>
#include <algorithm>

class Character {
private:
    // 🔒 Encapsulation: External systems CANNOT bypass validation
    std::string name;
    float hp;
    float max_hp;

public:
    // 🔹 Initialization List: Faster, safer, required for const/ref members
    Character(std::string n, float health) 
        : name(std::move(n)), hp(health), max_hp(health) {
        std::cout << "🎮 Created: " << name << " (" << hp << " HP)\n";
    }

    // 🔑 VIRTUAL DESTRUCTOR: MANDATORY for any class intended to be inherited
    // Without this, deleting a derived object via a base pointer = UNDEFINED BEHAVIOR
    virtual ~Character() {
        std::cout << "💀 Destroyed: " << name << "\n";
    }

    // 🔹 Const Accessors (Read-only, thread-safe, zero copies)
    float get_hp() const { return hp; }
    float get_max_hp() const { return max_hp; }
    const std::string& get_name() const { return name; }
    bool is_alive() const { return hp > 0.0f; }

    // 🔹 Controlled State Mutation (Enforces invariants)
    void take_damage(float amount) {
        if (amount < 0.0f) amount = 0.0f; // Guard against negative damage
        hp = std::max(hp - amount, 0.0f);
        if (!is_alive()) {
            std::cout << "☠️ " << name << " has fallen!\n";
        }
    }

    void heal(float amount) {
        if (amount < 0.0f) amount = 0.0f;
        hp = std::min(hp + amount, max_hp);
        std::cout << "💚 " << name << " healed to " << hp << "/" << max_hp << "\n";
    }
};

int main() {
    std::cout << "=== OOP Character Foundation ===\n\n";
    
    Character hero("Paladin", 120.0f);
    Character enemy("Skeleton", 60.0f);

    // ✅ Safe API usage
    hero.take_damage(40.0f);
    enemy.take_damage(100.0f);
    hero.heal(15.0f);

    std::cout << "\n📊 Final: " << hero.get_name() << " (" 
              << hero.get_hp() << "/" << hero.get_max_hp() << " HP) - "
              << (hero.is_alive() ? "Alive" : "Dead") << "\n";

    return 0;
}