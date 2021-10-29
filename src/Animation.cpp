#include "Animation.hpp"

Animation::Animation(sf::Sprite &p_target, float p_frameOffset, bool p_loop) 
    : m_target { p_target }, m_frameOffset { sf::seconds(p_frameOffset) }, m_loop { p_loop }
{
    
}

void Animation::addFrame(const sf::IntRect &p_frame)
{
    m_frames.push_back(std::move(p_frame));
    if (m_frames.size() == 1)
    {
        m_target.setTextureRect(m_frames.front());
    }
}

void Animation::update()
{
    if (m_clock.getElapsedTime() > m_frameOffset)
    {
        if (m_loop)
        {
            (++m_currentFrame) %= m_frames.size();
        }
        else if (m_currentFrame != m_frames.size() - 1)
        {
            ++m_currentFrame;
        }

        m_target.setTextureRect(m_frames[m_currentFrame]);
        m_clock.restart();
    }
}
