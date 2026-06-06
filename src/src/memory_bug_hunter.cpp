#include <iostream>
#include <memory>
#include <vector>
#include <string>

// 🔹 Simple particle to demonstrate lifetime tracking
struct Particle {
    std::string id;
    float life;
    Particle(const std::string& i, float l) : id(i), life(l) {
        std::cout << "✨ Created: " << id << "\n";
    }
    ~Particle() {
        std::cout << "💨 Destroyed: " << id << "\n";
    }
};

// 🔹 UNSAFE PATTERN (Educational Only)
void demonstrate_dangling() {
    std::cout << "\n--- ⚠️ Dangling Pointer Scenario ---\n";
    Particle* raw_ptr = new Particle("Fireball", 2.0f);
    
    // Simulate another system deleting this object
    delete raw_ptr; 
    
    // 🔒 ENGINE RULE: ALWAYS nullify after delete
    raw_ptr = nullptr; 
    
    // ❌ DANGER: Accessing raw_ptr here would be Use-After-Free (UAF)
    // std::cout << "Life: " << raw_ptr->life << "\n"; // CRASH or garbage data
    
    if (!raw_ptr) {
        std::cout << "✅ Pointer safely nulled. UAF prevented.\n";
    }
}

// 🔹 SAFE PATTERN: Observer Reference (.get() with validation)
class ParticleManager {
    std::vector<std::unique_ptr<Particle>> particles;

public:
    void spawn(const std::string& id, float life) {
        particles.emplace_back(std::make_unique<Particle>(id, life));
    }

    // 🔍 Returns NON-OWNING raw pointer for observation only.
    // Lifetime is guaranteed ONLY as long as the Manager owns the object.
    const Particle* find(const std::string& id) const {
        for (const auto& p : particles) {
            if (p->id == id) return p.get(); // .get() extracts raw pointer
        }
        return nullptr;
    }

    void destroy(const std::string& id) {
        // Erase-remove idiom: safely removes without dangling iterators
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [&id](const auto& p) { return p->id == id; }), particles.end());
    }
};

int main() {
    std::cout << "=== Memory Bug Hunter & Safe References ===\n";
    
    demonstrate_dangling();
    
    std::cout << "\n--- ✅ Observer Pattern Demo ---\n";
    ParticleManager mgr;
    mgr.spawn("Spark", 1.0f);
    mgr.spawn("Smoke", 3.0f);

    const Particle* spark_ref = mgr.find("Spark");
    if (spark_ref) std::cout << "🔍 Observed: " << spark_ref->id << "\n";

    // Another system destroys it
    mgr.destroy("Spark");
    
    // ⚠️ spark_ref is now DANGLING if we still hold it.
    // 🔒 ENGINE PRACTICE: Never cache raw pointers across frames without validation.
    spark_ref = mgr.find("Spark"); // Re-validate
    if (!spark_ref) std::cout << "🔍 Spark correctly gone. Reference invalidated safely.\n";

    std::cout << "\n🧹 Engine shutting down...\n";
    return 0;
}