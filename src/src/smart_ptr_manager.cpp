#include <iostream>
#include <memory>      // 🔹 Required for std::unique_ptr
#include <vector>
#include <string>

// 🔹 Game Object
struct Weapon {
    std::string name;
    int damage;

    Weapon(const std::string& n, int d) : name(n), damage(d) {
        std::cout << "🗡️  Crafted: " << name << "\n";
    }
    ~Weapon() {
        std::cout << "🔥  Destroyed: " << name << "\n";
    }
};

// 🔹 Game Manager (The Owner)
class GameManager {
private:
    // 🔹 Vector of UNIQUE pointers. 
    // The Manager OWNS these weapons. When the Manager dies, the weapons are deleted.
    std::vector<std::unique_ptr<Weapon>> arsenal; 

public:
    // 🔹 Add a weapon to the manager
    void add_weapon(const std::string& name, int damage) {
        // std::make_unique creates the object and wraps it immediately.
        // std::move transfers ownership into the vector.
        // We CANNOT copy a unique_ptr (two managers cannot own one weapon).
        arsenal.push_back(std::make_unique<Weapon>(name, damage));
    }

    // 🔹 Access without taking ownership
    void show_stats() const {
        std::cout << "\n📊 Arsenal Status:\n";
        // We access using -> just like raw pointers
        for (const auto& w : arsenal) {
            std::cout << "  - " << w->name << " (DMG: " << w->damage << ")\n";
        }
    }

    // 🔹 Remove a weapon (The object is AUTOMATICALLY deleted here)
    void drop_last_weapon() {
        if (!arsenal.empty()) {
            std::cout << "\n💧 Dropping weapon: " << arsenal.back()->name << "\n";
            arsenal.pop_back(); 
            // No "delete" needed. pop_back destroys the unique_ptr, 
            // which destroys the Weapon.
        }
    }
    
    // 🔹 NO DESTRUCTOR NEEDED.
    // ~GameManager() is empty. The vector's destructor handles everything.
};

// 🔹 COORDINATOR
int main() {
    { // Scope block to demonstrate automatic cleanup
        std::cout << "=== Smart Pointer Ownership ===\n";
        GameManager manager;

        manager.add_weapon("Rusty Sword", 10);
        manager.add_weapon("Fire Axe", 25);
        manager.add_weapon("Ice Staff", 15);

        manager.show_stats();
        
        manager.drop_last_weapon(); // Staff is destroyed here
        
        std::cout << "\n🔚 Manager scope is closing...\n";
    } // <--- Manager is destroyed here. Remaining Sword & Axe are destroyed AUTOMATICALLY.

    std::cout << "\n✅ Program finished. Zero memory leaks.\n";
    return 0;
}