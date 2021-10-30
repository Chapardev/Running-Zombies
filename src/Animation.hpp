#ifndef SRC_ANIMATION_HPP
#define SRC_ANIMATION_HPP

#include <SFML/Graphics.hpp>

#include <vector>

class Animation
{
public:
    Animation(sf::Sprite &p_target, float p_frameOffset, bool p_loop=false);

    sf::Time getLength() const noexcept
    {
        return m_frameOffset * static_cast<float>(m_frames.size());
    }
    
    void addFrame(const sf::IntRect &p_frame);
    void update();

private:
    sf::Sprite &m_target;
    const sf::Time m_frameOffset;
    bool m_loop;
    
    std::vector<sf::IntRect> m_frames;
    sf::Clock m_clock;
    std::size_t m_currentFrame { 0 };
};

#endif // SRC_ANIMATION_HPP
