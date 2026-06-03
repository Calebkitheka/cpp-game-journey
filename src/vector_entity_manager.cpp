#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// 🔹 Game entity (RAII-friendly: vector handles destruction automatically)
struct Entity {
    std::string name;
    float hp;
    float x, y;

    Entity(const std::string& n, float h, float px, float py)
        : name(n), hp(h), x(px), y(py) {
        std::cout << "📦 Created: " << name << " at (" << x << "," << y << ")\n";
    }
    ~Entity() {
        std::cout << "🗑️ Destroyed: " << name << "\n";
    }
};

int main() {
    std::cout << "=== std::vector Entity Manager ===\n\n";

    // 🔹 1. Pre-allocation (Reserve vs Size)
    // reserve() allocates raw memory WITHOUT constructing objects.
    // Prevents expensive reallocations during gameplay (frame spikes).
    std::vector<Entity> entities;
    entities.reserve(10); // Capacity: 10, Size: 0

    // 🔹 2. Dynamic Spawning
    // emplace_back constructs objects IN-PLACE inside the vector.
    // Avoids temporary copies/moves. Faster than push_back for complex types.
    entities.emplace_back("Hero", 100.0f, 0.0f, 0.0f);
    entities.emplace_back("Goblin", 50.0f, 10.0f, 5.0f);
    entities.emplace_back("Orc", 80.0f, -5.0f, 12.0f);

    std::cout << "\n📊 Vector State -> Size: " << entities.size()
              << " | Capacity: " << entities.capacity() << "\n\n";

    // 🔹 3. Safe Iteration (Range-based for loop)
    // Cache-friendly sequential access. const& prevents accidental copies.
    std::cout << "🔄 Processing Entities:\n";
    for (const auto& e : entities) {
        std::cout << "  [" << e.name << "] HP: " << e.hp
                  << " | Pos: (" << e.x << "," << e.y << ")\n";
    }

    // 🔹 4. Safe Removal: Swap-and-Pop (O(1), Order-Agnostic)
    // ⚠️ NEVER remove from a vector while iterating forward. It invalidates iterators.
    // Engine standard: swap dead entity with the last one, then pop_back.
    std::cout << "\n⚔️ Goblin defeated!\n";
    auto it = std::find_if(entities.begin(), entities.end(),
        [](const Entity& e) { return e.name == "Goblin"; });

    if (it != entities.end()) {
        std::swap(*it, entities.back()); // Move last element to dead slot
        entities.pop_back();             // Remove last element (O(1), no shift)
    }

    std::cout << "\n📊 After Removal -> Size: " << entities.size()
              << " | Capacity: " << entities.capacity() << "\n";

    // 🔹 5. Automatic Cleanup (RAII)
    // Vector destructor calls ~Entity() for every remaining object.
    // Zero manual `delete`. Zero memory leaks.
    std::cout << "\n🧹 Scope ending. Vector auto-destroys remaining entities.\n";
    return 0;
}