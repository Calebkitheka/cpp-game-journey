#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "json.hpp" // 🔹 Local header-only JSON library

using json = nlohmann::json;

// 🔹 Game Configuration & Save Data
struct GameData {
    std::string player_name = "Player1";
    int master_volume = 75;
    bool fullscreen = false;
    int high_score = 0;

    // 🔹 Serialization (Object → JSON)
    void to_json(json& j) const {
        j = json{
            {"player_name", player_name},
            {"master_volume", master_volume},
            {"fullscreen", fullscreen},
            {"high_score", high_score}
        };
    }

    // 🔹 Deserialization (JSON → Object)
    void from_json(const json& j) {
        // j.value(key, fallback) safely handles missing keys
        player_name = j.value("player_name", "Player1");
        master_volume = std::clamp(j.value("master_volume", 75), 0, 100);
        fullscreen = j.value("fullscreen", false);
        high_score = j.value("high_score", 0);
    }
};

// 🔹 Safe File I/O Wrapper
bool save_data(const std::string& path, const GameData& data) {
    json j;
    data.to_json(j);
    std::ofstream out(path);
    if (!out.is_open()) return false;
    out << j.dump(4); // Pretty-print with 4-space indentation
    return out.good();
}

bool load_data(const std::string& path, GameData& data) {
    std::ifstream in(path);
    if (!in.is_open()) return false;
    try {
        json j = json::parse(in);
        data.from_json(j);
        return true;
    } catch (const json::parse_error& e) {
        std::cerr << "❌ JSON parse error: " << e.what() << "\n";
        return false;
    }
}

// 🔹 COORDINATOR
int main() {
    const std::string save_path = "save_data.json";
    GameData data;

    std::cout << "=== JSON Config & Save System ===\n\n";

    if (load_data(save_path, data)) {
        std::cout << "✅ Loaded existing save/config.\n";
    } else {
        std::cout << "⚠️ No valid save found. Using defaults.\n";
    }

    // 🔹 Simulate gameplay progression
    std::cout << "📊 Before: Name=" << data.player_name
              << " | Vol=" << data.master_volume
              << " | HighScore=" << data.high_score << "\n";

    data.player_name = "SpeedRunner";
    data.master_volume = 85;
    data.high_score = std::max(data.high_score, 14200);

    std::cout << "📊 After:  Name=" << data.player_name
              << " | Vol=" << data.master_volume
              << " | HighScore=" << data.high_score << "\n";

    if (save_data(save_path, data)) {
        std::cout << "💾 Data saved to " << save_path << "\n";
    } else {
        std::cout << "❌ Failed to write to disk.\n";
    }

    return 0;
}