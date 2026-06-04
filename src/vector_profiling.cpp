#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

struct Entity {
    int id;
    bool alive = true;
    float x = 0.0f, y = 0.0f;
    explicit Entity(int i) : id(i) {}
};

// 🔹 Profiler: Measures allocation cost & reallocation spikes
void profile_allocations(int count) {
    std::cout << "--- Profiling " << count << " Spawns ---\n";
    
    // Case 1: No reserve (triggers geometric reallocations: 0→1→2→4→8...)
    std::vector<Entity> dynamic_vec;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; ++i) dynamic_vec.emplace_back(i);
    auto end = std::chrono::steady_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "⚡ No reserve: " << us << "μs | Final capacity: " << dynamic_vec.capacity() << "\n";

    // Case 2: With reserve (zero reallocations, single allocation)
    std::vector<Entity> reserved_vec;
    reserved_vec.reserve(count);
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; ++i) reserved_vec.emplace_back(i);
    end = std::chrono::steady_clock::now();
    us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "🛡️ With reserve: " << us << "μs | Final capacity: " << reserved_vec.capacity() << "\n\n";
}

// 🔹 Safe Removal During Iteration (Reverse Swap-and-Pop)
// ⚠️ Forward loops + erase/remove = iterator invalidation & skipped elements
void safe_removal_demo() {
    std::vector<Entity> entities;
    for (int i = 1; i <= 10; ++i) entities.emplace_back(i);

    // Simulate combat: mark IDs 3, 6, 9 as dead
    entities[2].alive = false; entities[5].alive = false; entities[8].alive = false;
    std::cout << "--- Safe Removal Demo ---\n";

    // 🔒 Reverse iteration prevents invalidation when swapping with back()
    for (size_t i = entities.size(); i > 0; --i) {
        size_t idx = i - 1;
        if (!entities[idx].alive) {
            entities[idx] = std::move(entities.back()); // Swap last into dead slot
            entities.pop_back();                        // O(1) removal
        }
    }

    std::cout << "🗑️ Removed dead entities. Remaining: " << entities.size() << "\n";
    std::cout << "IDs: ";
    for (const auto& e : entities) std::cout << e.id << " ";
    std::cout << "\n\n";
}

// 🔹 COORDINATOR
int main() {
    std::cout << "=== Vector Profiling & Safe Iteration ===\n\n";
    profile_allocations(10000);
    safe_removal_demo();
    return 0;
}