#ifndef SRC_STATES_HPP
#define SRC_STATES_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <stack>
#include <string>

template<typename T>
using Dictionary = std::map<std::string, T>;

class State
{
protected:
    void loadTexture(const std::string &p_keyName, const std::string &p_fileName);
    void loadFont(const std::string &p_fileName);
    void loadSoundBuffer(const std::string &p_keyName, const std::string &p_fileName);
    void openMusic(const std::string &p_fileName);

public:
    State(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
        Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
        Dictionary<sf::Sound> &p_sounds);

    virtual void checkEvents(sf::Event &p_event);
    virtual void update() = 0;
    virtual void draw() = 0;

protected:
    sf::RenderWindow &m_window;
    std::stack<std::unique_ptr<State>> &m_states;

    sf::Font &m_font;
    Dictionary<sf::Text> &m_texts;

    Dictionary<sf::Texture> &m_textures;
    Dictionary<sf::Sprite> &m_sprites;

    sf::Music &m_music;
    Dictionary<sf::SoundBuffer> &m_soundBuffers;
    Dictionary<sf::Sound> &m_sounds;
};

#endif // SRC_STATES_HPP
