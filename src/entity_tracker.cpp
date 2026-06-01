#include <iostream>
#include <string>

// 🔹 Lightweight data container (struct = public-by-default class)
struct Entity {
    std::string name;
    float hp;
    bool alive;
};

// 🔹 Pass-by-REFERENCE: Guarantees a valid object. No null checks needed.
// Preferred for systems that KNOW the entity exists.
void update_health_ref(Entity& entity, float amount) {
    entity.hp += amount;
    entity.alive = (entity.hp > 0.0f);
}

// 🔹 Pass-by-POINTER: Explicitly nullable. Must validate before dereferencing.
// Used when ownership is ambiguous or the object might not exist.
void update_health_ptr(Entity* entity, float amount) {
    if (!entity) { // 🔒 Engine pattern: NEVER dereference without validation
        std::cout << "⚠️ Null entity passed to update_health_ptr\n";
        return;
    }
    entity->hp += amount;          // -> is shorthand for (*entity).
    entity->alive = (entity->hp > 0.0f);
}

// 🔹 COORDINATOR: Demonstrates reference vs pointer behavior
int main() {
    // 🔹 Stack-allocated objects (lifetime tied to scope)
    Entity hero{"Paladin", 100.0f, true};
    Entity goblin{"Goblin", 50.0f, true};

    std::cout << "=== Memory Access Patterns ===\n\n";

    // 1️⃣ Reference usage (safe, implicit, no copies)
    std::cout << "🛡️ Before: " << hero.name << " HP: " << hero.hp << "\n";
    update_health_ref(hero, -25.0f);
    std::cout << "🛡️ After:  " << hero.name << " HP: " << hero.hp << "\n\n";

    // 2️⃣ Pointer usage (explicit address-of `&`, requires null guard)
    Entity* enemy_ptr = &goblin; // Points to stack object
    update_health_ptr(enemy_ptr, -30.0f);
    std::cout << "👹 Enemy: " << goblin.name << " HP: " << goblin.hp 
              << " | Alive: " << (goblin.alive ? "Yes" : "No") << "\n\n";

    // 3️⃣ Null pointer safety demonstration
    Entity* dangling_ptr = nullptr;
    update_health_ptr(dangling_ptr, -10.0f); // Safely handled, no crash

    std::cout << "✅ Run complete. No undefined behavior.\n";
    return 0;
}