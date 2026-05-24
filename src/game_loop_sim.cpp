#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== 60 FPS Game Loop Simulator ===\n";
    std::cout << "Type 'q' and press Enter to quit early.\n\n";

    // 🔹 Loop configuration
    const int TARGET_FPS = 60;
    const int MAX_SECONDS = 3;
    const int TOTAL_FRAMES = TARGET_FPS * MAX_SECONDS;
    const auto FRAME_DELAY = std::chrono::milliseconds(1000 / TARGET_FPS); 

    int frame_count = 0;
    bool quit_requested = false;
    auto start_time = std::chrono::steady_clock::now();

    // Set cin to non-blocking behavior if possible, or use peek
    // For console apps, we check if there is data waiting
    std::cin.sync_with_stdio(false); 

    std::cout << "🚀 Loop starting...\n";

    while (frame_count < TOTAL_FRAMES && !quit_requested) {
        frame_count++;

        // 🔹 Improved Input polling
        if (std::cin.peek() != EOF) {
            char cmd;
            std::cin >> cmd;
            if (cmd == 'q' || cmd == 'Q') {
                quit_requested = true;
                std::cout << "\n🛑 Quit requested.\n";
                break;
            }
        }

        // 🔹 Game logic simulation
        if (frame_count % TARGET_FPS == 0) {
            int sec = frame_count / TARGET_FPS;
            std::cout << "⏱️  Second " << sec << " passed | Frames: " << frame_count << "\n";
        }

        // 🔹 Frame pacing
        std::this_thread::sleep_for(FRAME_DELAY);
    }

    auto end_time = std::chrono::steady_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "\n✅ Loop finished.\n";
    std::cout << "⏱️ Target: ~3000ms | Actual: " << duration_ms << "ms\n";
    std::cout << "🎯 Frames processed: " << frame_count << "\n";

    return 0;
}