#include <iostream>
#include <memory>
#include <vector>
#include <string>

// 🔹 Base Interface (Abstract Class)
class Unit {
protected:
    std::string name;
    float hp;
public:
    Unit(std::string n, float h) : name(std::move(n)), hp(h) {}
    
    // 🔑 VIRTUAL DESTRUCTOR: Required for safe deletion of derived objects via base pointer
    virtual ~Unit() = default;

    // 🔹 PURE VIRTUAL: Forces derived classes to implement this. Makes Unit abstract.
    virtual void update(float dt) = 0;

    virtual void take_damage(float amount) {
        hp -= amount;
        if (hp <= 0.0f) std::cout << "💀 " << name << " defeated!\n";
    }
};

// 🔹 Derived Class 1
class MeleeUnit : public Unit {
public:
    MeleeUnit(std::string n, float h) : Unit(std::move(n), h) {}
    
    // 🔒 `override` guarantees signature matches base. Compiler catches typos/mismatches.
    void update(float dt) override {
        std::cout << "⚔️ [Melee] " << name << " closes distance! (DT: " << dt << ")\n";
    }
};

// 🔹 Derived Class 2
class RangedUnit : public Unit {
public:
    RangedUnit(std::string n, float h) : Unit(std::move(n), h) {}
    
    void update(float dt) override {
        std::cout << "🏹 [Ranged] " << name << " fires projectile! (DT: " << dt << ")\n";
    }
};

// 🔹 COORDINATOR: Polymorphic Runtime Dispatch
int main() {
    std::cout << "=== Polymorphic Entity Loop ===\n\n";

    // 🔹 Heterogeneous container: stores different types via base pointer
    std::vector<std::unique_ptr<Unit>> squad;
    squad.reserve(4);
    squad.push_back(std::make_unique<MeleeUnit>("Knight", 100.0f));
    squad.push_back(std::make_unique<RangedUnit>("Archer", 70.0f));
    squad.push_back(std::make_unique<MeleeUnit>("Berserker", 120.0f));

    float dt = 0.016f; // Fixed timestep
    for (auto& unit : squad) {
        unit->update(dt);      // 🔍 Runtime dispatch: calls correct derived function
        unit->take_damage(25.0f);
    }

    std::cout << "\n🧹 Squad scope closing. All units auto-destruct.\n";
    return 0;
}