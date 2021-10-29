#include "Entity.hpp"

#include <effolkronium/random.hpp>

void Entity::_addFrames(Animation &p_animation, sf::Sprite &p_sprite, int p_numberOfFrames)
{
    const int frameWidth { p_sprite.getTextureRect().width / p_numberOfFrames };
    for (int i { 0 }; i < p_numberOfFrames; ++i)
    {
        p_animation.addFrame({ frameWidth * i, 0, frameWidth, p_sprite.getTextureRect().height });
    }
}

Entity::Entity(const sf::RenderWindow &p_window, const AnimatedSprite &p_spriteWalk, const AnimatedSprite &p_spriteDie, std::size_t p_id, int p_score)
    : m_window { p_window }, m_spriteWalk { p_spriteWalk.sprite }, m_spriteDie { p_spriteDie.sprite }, 
      m_animationWalk { m_spriteWalk, p_spriteWalk.frameOffset, true }, m_animationDie { m_spriteDie, p_spriteDie.frameOffset }, m_id { p_id },
      m_score { p_score }
{
    using Random = effolkronium::random_static;
    m_spriteWalk.setPosition(
        Random::get<unsigned int>(
            0u, p_window.getSize().x - (m_spriteWalk.getTextureRect().width * m_spriteWalk.getScale().x) / p_spriteWalk.numberOfFrames
        ),
        -m_spriteWalk.getTextureRect().height * m_spriteWalk.getScale().y
    );

    this->_addFrames(m_animationWalk, m_spriteWalk, p_spriteWalk.numberOfFrames);
    this->_addFrames(m_animationDie, m_spriteDie, p_spriteDie.numberOfFrames);
}

void Entity::changeAnimation()
{
    m_spriteDie.setPosition(m_spriteWalk.getPosition());
    m_animationChanged = true;
    m_deathClock.restart();
}

void Entity::update(float p_dt)
{
    if (!m_animationChanged)
    {
        m_spriteWalk.move(0.f, m_speed * p_dt);
        m_animationWalk.update();
    }
    else if (!this->isDead())
    {
        m_animationDie.update();
    }
}

void Entity::draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const
{
    p_target.draw((!m_animationChanged ? m_spriteWalk : m_spriteDie), p_states);
}
