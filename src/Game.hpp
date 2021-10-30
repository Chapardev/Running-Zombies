#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include "Entity.hpp"

#include "GameState.hpp"

class Game
{
public:
    static std::string getCurrentDateTime() noexcept;
    static void logError(const std::string &message);

public:
    Game();

    void pollEvents();
    void update();
    void draw();

    void run();

private:
    sf::RenderWindow m_window { { 600, 700 }, "Running Zombies!", sf::Style::Titlebar | sf::Style::Close };
    std::stack<std::unique_ptr<State>> m_states;
    
    sf::Font m_font;
    Dictionary<sf::Text> m_texts;

    Dictionary<sf::Texture> m_textures;
    Dictionary<sf::Sprite> m_sprites;

    sf::Music m_music;
    Dictionary<sf::SoundBuffer> m_soundBuffers;
    Dictionary<sf::Sound> m_sounds;
};

#endif // SRC_GAME_HPP
