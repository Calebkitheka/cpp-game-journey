#include <iostream>
#include <string>

// 🔹 Simple entity with constructor/destructor for lifetime tracking
struct Entity {
    std::string name;
    int id;
    float x, y;

    Entity(const std::string& n, int i) : name(n), id(i), x(0.0f), y(0.0f) {
        std::cout << "📦 Created: " << name << "\n";
    }
    ~Entity() {
        std::cout << "🗑️ Destroyed: " << name << "\n";
    }
};

// 🔹 Raw heap allocation (educational only - engines avoid this in hot paths)
void spawn_raw_heap(int id) {
    Entity* e = new Entity("HeapEnemy", id);
    e->x = 10.0f; e->y = 20.0f;
    
    // 🔒 MUST call delete to free heap memory. Forgetting = MEMORY LEAK.
    delete e; 
}

// 🔹 Primitive Object Pool (Game Dev Standard Pattern)
// Pre-allocates slots to avoid runtime fragmentation & allocation spikes.
const int POOL_SIZE = 4;
Entity* pool[POOL_SIZE] = {nullptr}; // Explicitly initialize to null
bool pool_active[POOL_SIZE] = {false};

int pool_spawn(const std::string& name, int id) {
    for (int i = 0; i < POOL_SIZE; ++i) {
        if (!pool_active[i]) {
            pool[i] = new Entity(name, id);
            pool_active[i] = true;
            return i; // Return slot index for later reference
        }
    }
    std::cout << "❌ Pool exhausted! Engine would now trigger GC or drop spawns.\n";
    return -1;
}

void pool_destroy(int slot) {
    if (slot >= 0 && slot < POOL_SIZE && pool_active[slot]) {
        delete pool[slot];    // 🔑 Free heap memory
        pool[slot] = nullptr; // 🔒 Prevent dangling pointer
        pool_active[slot] = false;
    }
}

// 🔹 COORDINATOR: Demonstrates lifetime & pool behavior
int main() {
    std::cout << "=== Stack vs Heap vs Object Pool ===\n\n";

    // 1️⃣ STACK: Lifetime bound to scope `{}`
    std::cout << "--- Stack Lifetime ---\n";
    {
        Entity stack_hero("StackHero", 1);
        stack_hero.x = 5.0f;
        std::cout << "Active in scope...\n";
    } // ~Destructor called AUTOMATICALLY here

    // 2️⃣ HEAP: Manual `new`/`delete`
    std::cout << "\n--- Raw Heap Lifetime ---\n";
    spawn_raw_heap(101);
    std::cout << "(Heap cleaned up inside function)\n";

    // 3️⃣ POOL: Pre-allocated, reusable, zero-fragmentation
    std::cout << "\n--- Object Pool Lifetime ---\n";
    int g1 = pool_spawn("PoolGoblin", 201);
    int o1 = pool_spawn("PoolOrc", 202);
    
    std::cout << "Simulating combat...\n";
    pool_destroy(g1); // Reuse this slot later
    int s1 = pool_spawn("PoolSlime", 203);

    // Engine shutdown: clean up remaining
    std::cout << "\n🧹 Engine shutting down...\n";
    for (int i = 0; i < POOL_SIZE; ++i) {
        if (pool_active[i]) pool_destroy(i);
    }

    return 0;
}