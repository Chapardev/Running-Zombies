#include "Game.hpp"

#include <fstream>

std::string Game::getCurrentDateTime() noexcept
{
    constexpr const char *format { "%d/%m/%Y ; %X" };
    constexpr std::size_t bufferSize { sizeof("dd/mm/yyyy ; hh:mm:ss") };

    const std::time_t now { std::time(nullptr) };
    const std::tm tstruct { *std::localtime(&now) };

    std::array<char, bufferSize> buffer {  };
    std::strftime(buffer.data(), buffer.size(), format, &tstruct);

    return buffer.data();
}

void Game::logError(const std::string &message)
{
    std::ofstream ofs { "log.txt", std::ios::app };
    if (ofs.is_open())
    {
        ofs << Game::getCurrentDateTime() << " : " << message << '\n';
    }
}

Game::Game()
{
    sf::Image icon;
    if (!icon.loadFromFile("../assets/images/icon.png"))
    {
        throw std::string { "Unable to load icon image!" };
    }
    m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    m_window.setMouseCursorVisible(false);

    m_states.push(std::make_unique<MainMenuState>(m_window, m_states, m_font, m_texts, m_textures, m_sprites, m_music, m_soundBuffers, m_sounds));
}

void Game::pollEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
        else
        {
            m_states.top()->checkEvents(event);
        }
    }
}

void Game::update()
{
    m_states.top()->update();
}

void Game::draw()
{
    m_window.clear();
    
    m_states.top()->draw();

    m_window.display();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        this->pollEvents();
        this->update();
        this->draw();
    }
}
