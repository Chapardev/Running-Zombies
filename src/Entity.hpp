#ifndef SRC_ENTITY_HPP
#define SRC_ENTITY_HPP

#include "Animation.hpp"

struct AnimatedSprite
{
    AnimatedSprite(sf::Sprite &p_sprite, float p_frameOffset, int p_numberOfFrames, bool p_loop=false)
        : sprite { p_sprite }, frameOffset { p_frameOffset }, numberOfFrames { p_numberOfFrames }, loop { p_loop }, 
          animation { p_sprite, p_frameOffset, p_loop }
    {
        
    }   

    AnimatedSprite(const AnimatedSprite &p_other)
        : AnimatedSprite { p_other.sprite, p_other.frameOffset, p_other.numberOfFrames, p_other.loop }
    {
        
    }

    sf::Sprite &sprite;
    float frameOffset;
    int numberOfFrames;
    bool loop;
    Animation animation;
};

class Entity : public sf::Drawable
{
private:
    void _addFrames(AnimatedSprite &p_sprite);

public:
    Entity(const sf::RenderWindow &p_window, const AnimatedSprite &p_spriteWalk, const AnimatedSprite &p_spriteDie, std::size_t p_id, int p_score);

    bool isTargeted(float p_mousePosX, float p_mousePosY) const noexcept
    {
        return m_walk.sprite.getGlobalBounds().contains(p_mousePosX, p_mousePosY);
    }

    bool animationChanged() const noexcept
    {
        return m_animationChanged;
    }

    bool isDead() const noexcept
    {
        return m_animationChanged && m_deathClock.getElapsedTime() > m_die.animation.getLength();
    }

    bool isOutOfBounds() const noexcept
    {
        return m_walk.sprite.getGlobalBounds().top > m_window.getSize().y;
    }

    int getScore() const noexcept { return m_score; }
    std::size_t getId() const noexcept { return m_id; }

    virtual void changeAnimation();

    virtual void update(float p_dt);
    virtual void draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const override;

protected:
    const sf::RenderWindow &m_window;

    AnimatedSprite m_walk;
    AnimatedSprite m_die;

    // Enables to calculate time elapsed before corpse disappears
    sf::Clock m_deathClock;

    bool m_animationChanged { false };
    float m_speed { 200.f };

    std::size_t m_id;
    int m_score;
};

#endif // SRC_ENTITY_HPP
