#include <iostream>
#include <thread>      // 🔹 ADDED: Required for std::this_thread::sleep_for
#include <chrono>

int main() {
    std::cout << "=== 60 FPS Game Loop Simulator ===\n";
    std::cout << "Type 'q' and press Enter to quit early.\n\n";

    const int TARGET_FPS = 60;
    const int MAX_SECONDS = 3;
    const int TOTAL_FRAMES = TARGET_FPS * MAX_SECONDS;
    const auto FRAME_DELAY = std::chrono::milliseconds(1000 / TARGET_FPS);

    int frame_count = 0;
    bool quit_requested = false;
    auto start_time = std::chrono::steady_clock::now();

    std::cout << "🚀 Loop starting...\n";

    while (frame_count < TOTAL_FRAMES && !quit_requested) {
        frame_count++;

        if (std::cin.rdbuf()->in_avail() > 0) {
            char cmd;
            std::cin >> cmd;
            if (cmd == 'q' || cmd == 'Q') {
                quit_requested = true;
                std::cout << "\n🛑 Quit requested.\n";
                break;
            }
        }

        if (frame_count % TARGET_FPS == 0) {
            int sec = frame_count / TARGET_FPS;
            std::cout << "⏱️  Second " << sec << " passed | Frames: " << frame_count << "\n";
        }

        std::this_thread::sleep_for(FRAME_DELAY);
    }

    auto end_time = std::chrono::steady_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "\n✅ Loop finished.\n";
    std::cout << "⏱️ Target: 3000ms | Actual: " << duration_ms << "ms\n";
    std::cout << "🎯 Frames processed: " << frame_count << "\n";

    return 0;
}