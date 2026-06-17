#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== SFML Audio Architecture ===\n\n";

    // 🔹 1. SFX: Loaded entirely into RAM (fast, low latency, short duration)
    sf::SoundBuffer sfxBuffer;
    if (!sfxBuffer.loadFromFile("assets/hit.wav")) {
        std::cout << "⚠️  hit.wav missing. SFX playback disabled.\n";
    }
    sf::Sound paddleHit;
    paddleHit.setBuffer(sfxBuffer);
    paddleHit.setVolume(75.0f);

    // 🔹 2. BGM: Streamed from disk (low RAM usage, long duration, looping)
    sf::Music bgm;
    if (!bgm.openFromFile("assets/bgm.ogg")) {
        std::cout << "⚠️  bgm.ogg missing. BGM playback disabled.\n";
    } else {
        bgm.setLoop(true);
        bgm.setVolume(40.0f);
        bgm.play();
        std::cout << "🎵 BGM streaming from disk...\n";
    }

    // 🔹 3. Playback Control (Simulated game loop)
    std::cout << "Press ENTER to trigger paddle hit SFX.\n";
    std::cout << "Press Q to stop BGM and exit.\n\n";

    bool running = true;
    while (running) {
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "enter" || cmd == "") {
            if (sfxBuffer.getSize() > 0) {
                paddleHit.play();
                std::cout << "🔊 Played SFX\n";
            }
        } else if (cmd == "q" || cmd == "Q") {
            running = false;
            bgm.stop();
            std::cout << "🔇 BGM stopped.\n";
        }
    }

    // 🔹 RAII Cleanup: sf::Sound/SoundBuffer/Music destructors auto-free OpenAL resources
    std::cout << "✅ Audio system shutdown complete. Zero leaks.\n";
    return 0;
}