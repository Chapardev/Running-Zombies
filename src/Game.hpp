#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include "Entity.hpp"

#include <SFML/Audio.hpp>

#include <map>
#include <memory>
#include <string>

template<typename T>
using Dictionary = std::map<std::string, T>;

class Game
{
public:
    static std::string getCurrentDateTime() noexcept;
    static void logError(const std::string &message);

private:
    void _loadImage(sf::Image &p_image, const std::string &p_fileName) const;
    void _loadTexture(const std::string &p_keyName, const std::string &p_fileName);
    void _loadFont(const std::string &p_fileName);
    void _loadMusic(const std::string &p_fileName);
    void _loadSoundBuffer(const std::string &p_keyName, const std::string &p_fileName);
    void _addEntity();

public:
    Game();

    void pollEvents();
    void update();
    void draw();

    void run();

private:
    sf::RenderWindow m_window { { 600, 700 }, "Running Zombies!", sf::Style::Titlebar | sf::Style::Close };

    sf::Time m_apparitionTime { sf::seconds(1.f) };
    sf::Clock m_apparitionClock;

    float m_dt { 0.f };
    sf::Clock m_dtClock;
    
    sf::Font m_font;
    sf::Text m_scoreText;
    int m_score { 0 };

    Dictionary<sf::Texture> m_textures;
    Dictionary<sf::Sprite> m_sprites;
    std::vector<std::unique_ptr<AnimatedSprite>> m_animatedSprites;

    sf::Music m_music;
    Dictionary<sf::SoundBuffer> m_soundBuffers;
    Dictionary<sf::Sound> m_sounds;

    // I use smart pointers since I cannot reassign an entity (because of reference members)
    std::vector<std::unique_ptr<Entity>> m_entities;

    int m_numberOfEntities { 0 };
};

#endif // SRC_GAME_HPP
