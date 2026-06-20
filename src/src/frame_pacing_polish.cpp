#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>

// 🔹 Frame Pacing & Telemetry Manager
class FramePacer {
public:
    FramePacer() : dt(0.f), smoothedDt(0.f), fps(0), frameCount(0), fpsTimer(0.f), vsync(true) {}

    void tick() {
        float rawDt = clock.restart().asSeconds();
        dt = rawDt;
        
        // 🔹 Exponential Moving Average (EMA) smoothing
        // Prevents sudden delta spikes from causing physics jitter
        smoothedDt = (rawDt * 0.1f) + (smoothedDt * 0.9f);

        frameCount++;
        fpsTimer += rawDt;
        
        // Update FPS metric once per second
        if (fpsTimer >= 1.0f) {
            fps = static_cast<int>(frameCount / fpsTimer);
            frameCount = 0;
            fpsTimer = 0.f;
            logTelemetry();
        }
    }

    float getDt() const { return smoothedDt; }
    int getFps() const { return fps; }

    void toggleVSync(sf::RenderWindow& window) {
        vsync = !vsync;
        if (vsync) {
            window.setVerticalSyncEnabled(true);
            window.setFramerateLimit(0); // Let GPU driver control pacing
            std::cout << "🟢 VSync ON (Tearing-free, slight input latency)\n";
        } else {
            window.setVerticalSyncEnabled(false);
            window.setFramerateLimit(60); // Hard cap to prevent CPU spin
            std::cout << "🔴 VSync OFF (60 FPS cap, lower input latency)\n";
        }
    }

private:
    sf::Clock clock;
    float dt, smoothedDt;
    int fps, frameCount;
    float fpsTimer;
    bool vsync;

    void logTelemetry() {
        std::cout << "\033[2K\r" // Clear current console line (cross-platform safe fallback)
                  << "📊 FPS: " << std::setw(3) << fps
                  << " | dt: " << std::fixed << std::setprecision(3) << smoothedDt * 1000.f << "ms"
                  << " | VSync: " << (vsync ? "ON " : "OFF") << std::flush;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Day 25: Frame Pacing & Telemetry");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    FramePacer pacer;

    // Visual FPS indicator (changes color based on performance)
    sf::CircleShape fpsIndicator(15.f);
    fpsIndicator.setOrigin(15.f, 15.f);
    fpsIndicator.setPosition(770.f, 30.f);

    // Simulated workload to stress-test pacing
    const int WORKLOAD_ITERATIONS = 50000;

    std::cout << "=== Frame Pacing & Telemetry ===\n";
    std::cout << "Controls: V = Toggle VSync | ESC = Quit\n\n";

    while (window.isOpen()) {
        // 1️⃣ EVENT LOOP
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V) {
                pacer.toggleVSync(window);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // 2️⃣ UPDATE & PACING
        pacer.tick(); // Calculates smoothed dt & FPS
        
        // 🔒 Use smoothedDt for all game logic
        float dt = pacer.getDt();
        int currentFps = pacer.getFps();

        // Simulate variable CPU workload (stress test)
        volatile float dummy = 0.f;
        for (int i = 0; i < WORKLOAD_ITERATIONS; ++i) dummy += std::sin(i * dt);

        // Update visual indicator based on FPS threshold
        if (currentFps >= 55)      fpsIndicator.setFillColor(sf::Color::Green);
        else if (currentFps >= 30) fpsIndicator.setFillColor(sf::Color::Yellow);
        else                       fpsIndicator.setFillColor(sf::Color::Red);

        // 3️⃣ RENDER
        window.clear(sf::Color(25, 25, 35));
        window.draw(fpsIndicator);
        window.display();
    }
    std::cout << "\n✅ Frame pacing session complete.\n";
    return 0;
}