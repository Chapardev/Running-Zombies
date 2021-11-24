#ifndef SRC_ENTITY_HPP
#define SRC_ENTITY_HPP

#include "AnimatedSprite.hpp"

#include <boost/algorithm/string.hpp>

#include <memory>
#include <map>

template<typename T>
using Dictionary = std::map<std::string, T>; 

class Entity : public sf::Drawable
{
private:
    void _addFrames(AnimatedSprite &p_sprite);

public:
    Entity(const sf::RenderWindow &p_window, const std::string &p_type, std::size_t p_id, int p_score, float p_speed);

    void addAnimatedSprite(const std::string &p_keyName, const AnimatedSprite &p_sprite);

    bool isTargeted(float p_mousePosX, float p_mousePosY) const noexcept
    {
        return m_animatedSprites.at("walk")->sprite.getGlobalBounds().contains(p_mousePosX, p_mousePosY);
    }

    bool isDead() const noexcept
    {
        return m_animationChanged && m_animatedSprites.at("die")->animation.isOver() 
            && m_deathClock.getElapsedTime() > m_animatedSprites.at("die")->animation.getLength();
    }

    bool isOutOfBounds() const noexcept
    {
        return m_animatedSprites.at("walk")->sprite.getGlobalBounds().top > m_window.getSize().y;
    }

    bool animationChanged() const noexcept  { return m_animationChanged; }
    bool isZombie() const noexcept          { return boost::contains(m_type, "zombie"); }
    int getScore() const noexcept           { return m_score; }
    std::size_t getId() const noexcept      { return m_id; }

    virtual void changeAnimation();

    virtual void update(float p_dt);
    virtual void draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const override;

protected:
    const sf::RenderWindow &m_window;
    Dictionary<std::unique_ptr<AnimatedSprite>> m_animatedSprites;

    // Enables to calculate time elapsed before corpse disappears
    sf::Clock m_deathClock;

    bool m_animationChanged { false };

    const std::string m_type;
    const std::size_t m_id;
    const int m_score;
    const float m_speed;
};

#endif // SRC_ENTITY_HPP
