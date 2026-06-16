#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <cmath>

enum class StateID { Menu, Play, GameOver };

// 🔹 Base State Interface
class GameState {
public:
    virtual void handleInput(const sf::Event&) = 0;
    virtual void update(float dt, StateID& next) = 0;
    virtual void render(sf::RenderWindow&) = 0;
    virtual ~GameState() = default;
};

// 🔹 Play State: Core Gameplay
class PlayState : public GameState {
    sf::RectangleShape paddleL, paddleR, ball;
    sf::Vector2f ballVel;
    int scoreL = 0, scoreR = 0;
    const int WIN_SCORE = 5;
    const float PADDLE_SPEED = 450.f, BALL_SPEED = 400.f;

    void resetBall() {
        ball.setPosition(390.f, 290.f);
        ballVel = { BALL_SPEED * (rand() % 2 == 0 ? 1 : -1), 200.f * (rand() % 2 == 0 ? 1 : -1) };
    }

public:
    PlayState() {
        // Paddles
        paddleL.setSize({15.f, 90.f}); paddleL.setFillColor(sf::Color::Green); paddleL.setPosition(20.f, 255.f);
        paddleR.setSize({15.f, 90.f}); paddleR.setFillColor(sf::Color::Red);   paddleR.setPosition(765.f, 255.f);
        
        // Ball
        ball.setRadius(10.f); ball.setFillColor(sf::Color::White);
        ball.setPosition(390.f, 290.f); ball.setOrigin(10.f, 10.f);
        resetBall();
    }

    void handleInput(const sf::Event&) override {
        // Continuous input handled in update() for smooth movement
    }

    void update(float dt, StateID& next) override {
        // Paddle Movement (W/S & Up/Down)
        float moveY = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveY -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveY += 1.f;
        if (moveY != 0.f) paddleL.move(0.f, moveY * PADDLE_SPEED * dt);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    moveY -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  moveY += 1.f;
        if (moveY != 0.f) paddleR.move(0.f, moveY * PADDLE_SPEED * dt);

        // Clamp paddles to window
        auto clamp = [](sf::Shape& s, float min, float max) {
            if (s.getPosition().y < min) s.setPosition(s.getPosition().x, min);
            if (s.getPosition().y + s.getGlobalBounds().height > max) s.setPosition(s.getPosition().x, max - s.getGlobalBounds().height);
        };
        clamp(paddleL, 0.f, 600.f); clamp(paddleR, 0.f, 600.f);

        // Ball Movement
        ball.move(ballVel * dt);

        // Wall Bounce (Top/Bottom)
        if (ball.getPosition().y < 10 || ball.getPosition().y > 590) ballVel.y *= -1.f;

        // Scoring (Left/Right)
        if (ball.getPosition().x < -20) { scoreR++; resetBall(); }
        if (ball.getPosition().x > 820) { scoreL++; resetBall(); }
        
        if (scoreL >= WIN_SCORE || scoreR >= WIN_SCORE) {
            std::cout << "🏆 Player " << (scoreL >= WIN_SCORE ? "1" : "2") << " Wins!\n";
            next = StateID::GameOver;
        }

        // Paddle Collision (AABB + Angle Reflection)
        auto checkPaddle = [&](sf::RectangleShape& p) {
            if (ball.getGlobalBounds().intersects(p.getGlobalBounds())) {
                float hitOffset = (ball.getPosition().y - (p.getPosition().y + 45.f)) / 45.f; // -1 to 1
                ballVel.x = -ballVel.x;
                ballVel.y = hitOffset * 300.f;
                // Prevent double hits
                ball.move(ballVel * dt * 2.f); 
            }
        };
        checkPaddle(paddleL); checkPaddle(paddleR);
    }

    void render(sf::RenderWindow& w) override {
        w.draw(paddleL); w.draw(paddleR); w.draw(ball);
        // Simple center line
        for (int i = 0; i < 600; i += 20) {
            sf::RectangleShape dash({2.f, 10.f});
            dash.setPosition(399.f, i); dash.setFillColor(sf::Color(100,100,120));
            w.draw(dash);
        }
        // Console UI (Font loading skipped for dependency-free capstone)
        if (scoreL > 0 || scoreR > 0) std::cout << "\r📊 Score: " << scoreL << " | " << scoreR << std::flush;
    }
};

// 🔹 Menu & GameOver States (Simplified for focus)
class MenuState : public GameState {
public:
    void handleInput(const sf::Event& e) override {
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
            std::cout << "🎮 Starting Game...\n";
    }
    void update(float, StateID& next) override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) next = StateID::Play;
    }
    void render(sf::RenderWindow& w) override {
        w.clear(sf::Color(25, 25, 35));
        sf::RectangleShape btn({180.f, 40.f}); btn.setFillColor(sf::Color::Green); btn.setPosition(310.f, 280.f);
        w.draw(btn);
    }
};

class GameOverState : public GameState {
public:
    void handleInput(const sf::Event& e) override {
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)
            std::cout << "🔄 Returning to Menu...\n";
    }
    void update(float, StateID& next) override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) next = StateID::Menu;
    }
    void render(sf::RenderWindow& w) override {
        w.clear(sf::Color(20, 15, 15));
        sf::RectangleShape btn({200.f, 40.f}); btn.setFillColor(sf::Color::Yellow); btn.setPosition(300.f, 280.f);
        w.draw(btn);
    }
};

// 🔹 MAIN LOOP: State Manager
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Day 21: Pong Capstone");
    window.setFramerateLimit(60);
    sf::Clock clock;
    StateID current = StateID::Menu;
    std::unique_ptr<GameState> state = std::make_unique<MenuState>();

    std::cout << "=== CONTROLS ===\nW/S: Left Paddle | Up/Down: Right Paddle | ESC: Quit\n\n";

    while (window.isOpen()) {
        StateID next = current;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            state->handleInput(event);
        }

        state->update(clock.restart().asSeconds(), next);
        if (next != current) {
            switch (next) {
                case StateID::Menu:    state = std::make_unique<MenuState>(); break;
                case StateID::Play:    state = std::make_unique<PlayState>(); break;
                case StateID::GameOver:state = std::make_unique<GameOverState>(); break;
            }
            current = next;
        }

        window.clear(sf::Color(20, 20, 25));
        state->render(window);
        window.display();
    }
    return 0;
}