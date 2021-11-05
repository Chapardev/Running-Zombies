#include "MainMenuState.hpp"

#include "GameState.hpp"

MainMenuState::MainMenuState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
        Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
        Dictionary<sf::Sound> &p_sounds)
    : State { p_window, p_states, p_font, p_texts, p_textures, p_sprites, p_music, p_soundBuffers, p_sounds }
{
    this->loadFont("../assets/fonts/PressStart2P.ttf");
    this->loadTexture("spritesheet", "../assets/images/spritesheet.png");
    
    this->loadSound("punch", "../assets/sounds/punch.wav", 20.f);
    this->loadSound("dead_male_zombie", "../assets/sounds/dead_male_zombie.wav", 30.f);
    this->loadSound("dead_female_zombie", "../assets/sounds/dead_female_zombie.wav", 30.f);
    this->loadSound("dead_human", "../assets/sounds/dead_human.wav", 80.f);

    this->openMusic("../assets/musics/background.wav");
    m_music.setLoop(true);

    m_sprites["road"].setTexture(m_textures.at("spritesheet"));
    m_sprites.at("road").setTextureRect({ 8208, 0, 600, 700 });

    m_sprites["cursor"].setTexture(m_textures.at("spritesheet"));
    m_sprites.at("cursor").setTextureRect({ 8208, 700, 32, 32 });
    m_sprites.at("cursor").setColor(sf::Color::Red);

    m_sprites["logo"].setTexture(m_textures.at("spritesheet"));
    m_sprites.at("logo").setTextureRect({ 8208, 732, 300, 400 });
    m_sprites.at("logo").setPosition((m_window.getSize().x - m_sprites.at("logo").getGlobalBounds().width) / 2.f, 50.f);

    this->createText("title", "CLICK TO PLAY", 0.f, 0.f, 20);
    m_texts.at("title").setPosition(
        (m_window.getSize().x - m_texts.at("title").getGlobalBounds().width) / 2.f,
        (m_window.getSize().y - m_texts.at("title").getGlobalBounds().height) * 0.75f
    );
}

void MainMenuState::checkEvents(sf::Event &p_event)
{
    if (p_event.type == sf::Event::KeyPressed && p_event.key.code == sf::Keyboard::Escape)
    {
        m_window.close();
    }
    else if (p_event.type == sf::Event::MouseButtonPressed && p_event.mouseButton.button == sf::Mouse::Left)
    {
        m_states.push(std::make_unique<GameState>(m_window, m_states, m_font, m_texts, m_textures, m_sprites, m_music, m_soundBuffers, m_sounds));
    }
}

void MainMenuState::update()
{
    if (m_blinkClock.getElapsedTime() > m_blinkTime * 2.f)
    {
        m_blinkClock.restart();
    }

    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });
}

void MainMenuState::draw()
{
    m_window.draw(m_sprites.at("road"));

    m_window.draw(m_sprites.at("logo"));

    if (m_blinkClock.getElapsedTime() < m_blinkTime)
    {
        m_window.draw(m_texts.at("title"));
    }

    m_window.draw(m_sprites.at("cursor"));
}
