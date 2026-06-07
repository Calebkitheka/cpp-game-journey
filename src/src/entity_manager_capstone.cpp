#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>

// 🔹 Data-only struct (Position + State)
struct Entity {
    std::string id;
    float x = 0.0f, y = 0.0f, hp = 100.0f;
    Entity(std::string i, float px, float py) : id(std::move(i)), x(px), y(py) {}
};

// 🔹 Ownership Manager (RAII + Safe Lifecycle)
class EntityManager {
    std::vector<std::unique_ptr<Entity>> entities;

public:
    EntityManager() { entities.reserve(64); } // 🔒 Pre-allocate to prevent frame spikes

    void spawn(std::string id, float x, float y) {
        entities.push_back(std::make_unique<Entity>(std::move(id), x, y));
        std::cout << "📦 Spawned: " << id << "\n";
    }

    // 🔒 Safe Update: Erase-Remove Idiom
    // Works flawlessly with unique_ptr. No iterator invalidation.
    void update(float dt) {
        auto dead_it = std::remove_if(entities.begin(), entities.end(), [dt](const std::unique_ptr<Entity>& e) {
            e->x += 15.0f * dt;   // Simulate movement
            e->hp -= 10.0f * dt;  // Simulate damage over time
            return e->hp <= 0.0f; // Mark for removal
        });
        
        size_t removed = std::distance(dead_it, entities.end());
        entities.erase(dead_it, entities.end()); // O(N) but safe & standard
        if (removed) std::cout << "💀 Removed " << removed << " entity(s).\n";
    }

    void render() const {
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "🖼️ Live Entities (" << entities.size() << "):\n";
        for (const auto& e : entities) {
            std::cout << "  [" << e->id << "] HP:" << e->hp 
                      << " | Pos:(" << e->x << "," << e->y << ")\n";
        }
    }

    size_t count() const { return entities.size(); }
};

// 🔹 COORDINATOR: Fixed Timestep Simulation Loop
int main() {
    std::cout << "=== Dynamic Entity Manager (Week 2 Capstone) ===\n\n";
    
    EntityManager world;
    world.spawn("Hero", 0.0f, 0.0f);
    world.spawn("Goblin_A", 10.0f, 5.0f);
    world.spawn("Goblin_B", -8.0f, 2.0f);

    const float FIXED_DT = 0.016666f; // ~60 FPS timestep
    const int FRAMES_TO_SIMULATE = 6;

    for (int f = 0; f < FRAMES_TO_SIMULATE; ++f) {
        std::cout << "\n--- Frame " << (f + 1) << " ---\n";
        world.update(FIXED_DT);
        world.render();
        
        // Simulate frame pacing (optional, remove for pure logic testing)
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }

    std::cout << "\n🧹 Engine shutting down... Vector auto-destroys remaining entities.\n";
    return 0;
}