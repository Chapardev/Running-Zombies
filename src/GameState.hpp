#ifndef SRC_GAME_STATE_HPP
#define SRC_GAME_STATE_HPP

#include "State.hpp"

#include "Entity.hpp"

#include <vector>

class GameState : public State
{
public:
    GameState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
        Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
        Dictionary<sf::Sound> &p_sounds);
    
    void setEntityCharacteritics(int p_randomValue, std::string &p_type, sf::IntRect &p_walkRect, sf::IntRect &p_dieRect, int &p_score);
    void createEntitySprites(const std::string &p_type, const sf::IntRect &p_walkRect, const sf::IntRect &p_dieRect);
    void addEntity();

    void updateClickedEntity(float p_x, float p_y);

    // Removes entities who are dead or out of the screen
    void removeUselessEntities();
    void playSound(const std::string &p_keyName, float p_minValue);

    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    // Here I use the `inline` keyword so that I can initialize directly the attribute
    inline static bool s_launchedOnce { false };

    sf::Time m_apparitionTime { sf::seconds(1.f) };
    sf::Clock m_apparitionClock;

    const sf::Clock m_clock;

    float m_dt { 0.f };
    sf::Clock m_dtClock;
    
    std::vector<std::unique_ptr<AnimatedSprite>> m_animatedSprites;

    // I use smart pointers since I cannot reassign an entity (because of reference members)
    std::vector<std::unique_ptr<Entity>> m_entities;

    int m_score { 0 };
    int m_numberOfEntities { 0 };

    static constexpr int m_livesMax { 3 };
    int m_lives { m_livesMax };

    float m_entitySpeed { 200.f };
};

#endif // SRC_GAME_STATE_HPP
