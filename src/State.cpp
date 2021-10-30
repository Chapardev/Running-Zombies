#include "State.hpp"

void State::loadTexture(const std::string &p_keyName, const std::string &p_fileName)
{
    if (!m_textures[p_keyName].loadFromFile(p_fileName))
    {
        throw "Unable to load \"" + p_keyName + "\" texture (path: \"" + p_fileName + "\")";
    }
}

void State::loadFont(const std::string &p_fileName)
{
    if (!m_font.loadFromFile(p_fileName))
    {
        throw "Unable to load font (\"" + p_fileName + "\")";
    }
}

void State::loadSoundBuffer(const std::string &p_keyName, const std::string &p_fileName)
{
    if (!m_soundBuffers[p_keyName].loadFromFile(p_fileName))
    {
        throw "Unable to load \"" + p_keyName + "\" sound (path: \"" + p_fileName + "\")";
    }
}

void State::openMusic(const std::string &p_fileName)
{
    if (!m_music.openFromFile(p_fileName))
    {
        throw "Unable to load music (\"" + p_fileName + "\")";
    }
}

State::State(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
        Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
        Dictionary<sf::Sound> &p_sounds)
    : m_window { p_window }, m_states { p_states }, m_font { p_font }, m_texts { p_texts }, m_textures { p_textures }, m_sprites { p_sprites },
      m_music { p_music }, m_soundBuffers { p_soundBuffers }, m_sounds { p_sounds }
{
    
}

void State::checkEvents(sf::Event &p_event)
{

}