#ifndef SRC_GAME_STATE_HPP
#define SRC_GAME_STATE_HPP

#include "State.hpp"

#include "Entity.hpp"

#include <vector>

class GameState : public State
{
private:
    void _setEntityCharacteritics(int p_randomValue, std::string &p_walkPath, std::string &p_diePath, int &p_score);
    void _createEntitySprites(const std::string &p_walkPath, const std::string &p_diePath);
    void _addEntity();

    // Removes entities who are dead or out of the screen
    void _removeUselessEntities();
    void _updateAudio();
    void _playSound(const std::string &p_keyName, float p_minValue);
    void _playEntityDeathSound(const Entity &p_entity);

public:
    GameState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
        Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
        Dictionary<sf::Sound> &p_sounds);
    
    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    sf::Time m_apparitionTime { sf::seconds(1.f) };
    sf::Clock m_apparitionClock;

    float m_dt { 0.f };
    sf::Clock m_dtClock;
    
    std::vector<std::unique_ptr<AnimatedSprite>> m_animatedSprites;

    // I use smart pointers since I cannot reassign an entity (because of reference members)
    std::vector<std::unique_ptr<Entity>> m_entities;

    int m_score { 0 };
    int m_numberOfEntities { 0 };
};

#endif // SRC_GAME_STATE_HPP
