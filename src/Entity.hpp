#ifndef SRC_ENTITY_HPP
#define SRC_ENTITY_HPP

#include "Animation.hpp"

struct AnimatedSprite
{
    AnimatedSprite(sf::Sprite &p_sprite, float p_frameOffset, int p_numberOfFrames)
        : sprite { p_sprite }, frameOffset { p_frameOffset }, numberOfFrames { p_numberOfFrames }
    {

    }

    sf::Sprite &sprite;
    float frameOffset;
    int numberOfFrames;
};

class Entity : public sf::Drawable
{
private:
    void _addFrames(Animation &p_animation, sf::Sprite &p_sprite, int p_numberOfFrames);

public:
    Entity(const sf::RenderWindow &p_window, const AnimatedSprite &p_spriteWalk, const AnimatedSprite &p_spriteDie, std::size_t p_id, int p_score);

    bool isTargeted(float p_mousePosX, float p_mousePosY) const noexcept
    {
        return m_spriteWalk.getGlobalBounds().contains(p_mousePosX, p_mousePosY);
    }

    bool animationChanged() const noexcept
    {
        return m_animationChanged;
    }

    bool isDead() const noexcept
    {
        return m_animationChanged && m_deathClock.getElapsedTime() > m_animationDie.getAnimationLength();
    }

    bool isOutOfBounds() const noexcept
    {
        return m_spriteWalk.getGlobalBounds().top > m_window.getSize().y;
    }

    int getScore() const noexcept { return m_score; }
    std::size_t getId() const noexcept { return m_id; }

    virtual void changeAnimation();

    virtual void update(float p_dt);
    virtual void draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const override;

protected:
    const sf::RenderWindow &m_window;

    sf::Sprite &m_spriteWalk;
    sf::Sprite &m_spriteDie;

    Animation m_animationWalk;
    Animation m_animationDie;

    // Enables to calculate time elapsed before corpse disappears
    sf::Clock m_deathClock;

    bool m_animationChanged { false };
    float m_speed { 200.f };

    std::size_t m_id;
    int m_score;
};

#endif // SRC_ENTITY_HPP
