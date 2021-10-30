#include "Button.hpp"

Button::Button(const sf::RenderWindow &p_window, const sf::Vector2f &p_size, const sf::Color &p_defaultColor, const sf::Color &p_hoveredColor, bool p_centered)
    : m_window { p_window }, m_rect { p_size }, m_defaultColor { p_defaultColor }, m_hoveredColor { p_hoveredColor }
{
    m_rect.setFillColor(m_defaultColor);
    if (p_centered)
    {
        m_rect.setPosition((sf::Vector2f { m_window.getSize() } - p_size) / 2.f);
    }
}

void Button::setPosition(const sf::Vector2f &p_newPos)
{
    m_rect.setPosition(p_newPos);
}

void Button::update()
{
    if (this->isHovered())
    {
        if (m_rect.getFillColor() != m_hoveredColor)
        {
            m_rect.setFillColor(m_hoveredColor);
        }
    }
    else if (m_rect.getFillColor() != m_defaultColor)
    {
        m_rect.setFillColor(m_defaultColor);
    }
}

void Button::draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const
{
    p_target.draw(m_rect, p_states);
}
