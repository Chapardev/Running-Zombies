#ifndef SRC_ANIMATED_SPRITE_HPP
#define SRC_ANIMATED_SPRITE_HPP

#include "Animation.hpp"

class AnimatedSprite
{
public:
    AnimatedSprite(sf::Sprite &p_sprite, const sf::IntRect &p_frameInfo, int p_numberOfFrames, float p_frameOffset, bool p_loop=false)
        : sprite { p_sprite }, frameInfo { p_frameInfo }, numberOfFrames { p_numberOfFrames }, animation { p_sprite, p_frameOffset, p_loop }, 
          m_frameOffset { p_frameOffset }, m_loop { p_loop }
    {
        
    }   

    AnimatedSprite(const AnimatedSprite &p_other)
        : AnimatedSprite { p_other.sprite, p_other.frameInfo, p_other.numberOfFrames, p_other.m_frameOffset, p_other.m_loop }
    {
        
    }

    sf::Sprite &sprite;
    const sf::IntRect frameInfo;
    const int numberOfFrames;
    Animation animation;

private:
    const float m_frameOffset;
    const bool m_loop;
};

#endif // SRC_ANIMATED_SPRITE_HPP
