#ifndef SRC_BUTTON_HPP
#define SRC_BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
public:
    Button(const sf::RenderWindow &p_window, const sf::Vector2f &p_size, const sf::Color &p_defaultColor, const sf::Color &p_hoveredColor, bool p_centered=false);

    bool isHovered() const noexcept
    {
        return m_rect.getGlobalBounds().contains(sf::Vector2f { sf::Mouse::getPosition(m_window) });
    }

    void setPosition(const sf::Vector2f &p_newPos);
    void update();
    void draw(sf::RenderTarget &p_target, sf::RenderStates p_states) const override;

private:
    const sf::RenderWindow &m_window;
    sf::RectangleShape m_rect;
    sf::Color m_defaultColor;
    sf::Color m_hoveredColor;
};

#endif // SRC_BUTTON_HPP
