#include "Entity.hpp"

#include <effolkronium/random.hpp>

void Entity::_addFrames(AnimatedSprite &p_sprite)
{
    const int frameWidth { p_sprite.sprite.getTextureRect().width / p_sprite.numberOfFrames };
    for (int i { 0 }; i < p_sprite.numberOfFrames; ++i)
    {
        p_sprite.animation.addFrame({ frameWidth * i, 0, frameWidth, p_sprite.sprite.getTextureRect().height });
    }
}

Entity::Entity(const sf::RenderWindow &p_window, const AnimatedSprite &p_spriteWalk, const AnimatedSprite &p_spriteDie, std::size_t p_id, int p_score)
    : m_window { p_window }, m_walk { p_spriteWalk }, m_die { p_spriteDie }, m_id { p_id },
      m_score { p_score }
{
    using Random = effolkronium::random_static;
    m_walk.sprite.setPosition(
        Random::get<unsigned int>(
            0u, p_window.getSize().x - m_walk.sprite.getGlobalBounds().width / m_walk.numberOfFrames
        ),
        -m_walk.sprite.getTextureRect().height * m_walk.sprite.getScale().y
    );

    this->_addFrames(m_walk);
    this->_addFrames(m_die);
}

void Entity::changeAnimation()
{
    m_die.sprite.setPosition(m_walk.sprite.getPosition());
    m_animationChanged = true;
    m_deathClock.restart();
}

void Entity::update(float p_dt)
{
    if (!m_animationChanged)
    {
        m_walk.sprite.move(0.f, m_speed * p_dt);
        m_walk.animation.update();
    }
    else if (!this->isDead())
    {
        m_die.animation.update();
    }
}

void Entity::draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const
{
    p_target.draw((!m_animationChanged ? m_walk.sprite : m_die.sprite), p_states);
}
