#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// 🔹 GLSL Fragment Shader (Runs on GPU for every pixel)
const std::string FRAG_SHADER = R"(
uniform sampler2D texture;
uniform vec2 resolution;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    vec4 color = texture2D(texture, uv);
    
    // 🔹 CRT Scanlines
    float scanline = sin(uv.y * resolution.y * 3.14159) * 0.12;
    color.rgb -= scanline;
    
    // 🔹 Vignette (darken edges)
    float dist = distance(uv, vec2(0.5));
    color.rgb *= smoothstep(0.75, 0.15, dist);
    
    // 🔹 Subtle Color Grade (teal/orange hint)
    color.rgb = vec3(color.r * 0.85, color.g * 0.95, color.b * 1.15);
    
    gl_FragColor = color;
}
)";

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Day 23: Post-Processing Shaders");
    window.setFramerateLimit(60);

    // 🔹 1. Load Shader
    sf::Shader shader;
    if (!shader.loadFromMemory(FRAG_SHADER, sf::Shader::Fragment)) {
        std::cout << "❌ Shader failed to load. Check GPU/driver support.\n";
        return 1;
    }
    shader.setUniform("resolution", sf::Vector2f(800.0f, 600.0f));

    // 🔹 2. Off-Screen Render Texture (Post-Process Pipeline)
    sf::RenderTexture sceneRT;
    if (!sceneRT.create(800, 600)) {
        std::cout << "❌ Failed to create RenderTexture.\n";
        return 1;
    }

    // 🔹 3. Placeholder Scene
    sf::CircleShape ball(30.f);
    ball.setFillColor(sf::Color::Cyan);
    ball.setPosition(370.f, 270.f);
    
    sf::RectangleShape rect({400.f, 50.f});
    rect.setFillColor(sf::Color(220, 60, 60));
    rect.setPosition(200.f, 150.f);

    // Full-screen quad to display the post-processed result
    sf::RectangleShape screenQuad(sf::Vector2f(800.f, 600.f));
    screenQuad.setTexture(&sceneRT.getTexture());
    screenQuad.setPosition(0.f, 0.f);

    std::cout << "=== GPU Post-Processing Pipeline ===\n";
    std::cout << "Scene → RenderTexture → Fragment Shader → Screen\n\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // 🔄 STEP 1: Draw scene to off-screen buffer
        sceneRT.clear(sf::Color(30, 30, 40));
        sceneRT.draw(ball);
        sceneRT.draw(rect);
        sceneRT.display(); // 🔑 Commits draw calls to the texture

        // 🔄 STEP 2: Apply shader & render to screen
        window.clear();
        window.draw(screenQuad, &shader);
        window.display();
    }
    return 0;
}