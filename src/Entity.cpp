#include "Entity.hpp"

#include <effolkronium/random.hpp>

void Entity::_addFrames(AnimatedSprite &p_sprite)
{
    for (int i { 0 }; i < p_sprite.numberOfFrames; ++i)
    {
        p_sprite.animation.addFrame(
            { p_sprite.frameInfo.left + p_sprite.frameInfo.width * i, p_sprite.frameInfo.top, p_sprite.frameInfo.width, p_sprite.frameInfo.height }
        );
    }
}

Entity::Entity(const sf::RenderWindow &p_window, std::size_t p_id, int p_score)
    : m_window { p_window }, m_id { p_id }, m_score { p_score }
{
    
}

void Entity::addAnimatedSprite(const std::string &p_keyName, const AnimatedSprite &p_sprite)
{
    m_animatedSprites[p_keyName] = std::make_unique<AnimatedSprite>(p_sprite);

    if (p_keyName == "walk")
    {
        using Random = effolkronium::random_static;
        m_animatedSprites.at("walk")->sprite.setPosition(
            Random::get<unsigned int>(
                0u, 
                m_window.getSize().x - m_animatedSprites.at("walk")->sprite.getGlobalBounds().width / m_animatedSprites.at("walk")->numberOfFrames
            ),
            -m_animatedSprites.at("walk")->sprite.getTextureRect().height * m_animatedSprites.at("walk")->sprite.getScale().y
        );
    }

    this->_addFrames(*m_animatedSprites.at(p_keyName));
}

void Entity::changeAnimation()
{
    m_animatedSprites.at("die")->sprite.setPosition(m_animatedSprites.at("walk")->sprite.getPosition());
    m_animationChanged = true;
    m_deathClock.restart();
}

void Entity::update(float p_dt)
{
    if (!m_animationChanged)
    {
        m_animatedSprites.at("walk")->sprite.move(0.f, m_speed * p_dt);
        m_animatedSprites.at("walk")->animation.update();
    }
    else if (!this->isDead())
    {
        m_animatedSprites.at("die")->animation.update();
    }
}

void Entity::draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const
{
    p_target.draw((!m_animationChanged ? m_animatedSprites.at("walk")->sprite : m_animatedSprites.at("die")->sprite), p_states);
}
