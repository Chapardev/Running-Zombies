#include "MainMenuState.hpp"

#include "GameState.hpp"

MainMenuState::MainMenuState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
        Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
        Dictionary<sf::Sound> &p_sounds)
    : State { p_window, p_states, p_font, p_texts, p_textures, p_sprites, p_music, p_soundBuffers, p_sounds },
      m_button { m_window, sf::Vector2f { 100.f, 50.f }, sf::Color { sf::Color::Red }, sf::Color { 240, 85, 95 }, true }
{
    this->loadFont("../assets/fonts/PressStart2P.ttf");

    this->loadTexture("road", "../assets/images/road.jpg");
    this->loadTexture("cursor", "../assets/images/cursor.png");

    // Loading textures is long so you shouldn't make it in GameState
    this->loadTexture("human_walk_spritesheet", "../assets/images/human_walk_spritesheet.png");
    this->loadTexture("human_die_spritesheet", "../assets/images/human_die_spritesheet.png");
    this->loadTexture("male_zombie_walk_spritesheet", "../assets/images/zombie/male_walk_spritesheet.png");
    this->loadTexture("male_zombie_die_spritesheet", "../assets/images/zombie/male_die_spritesheet.png");
    this->loadTexture("female_zombie_walk_spritesheet", "../assets/images/zombie/female_walk_spritesheet.png");
    this->loadTexture("female_zombie_die_spritesheet", "../assets/images/zombie/female_die_spritesheet.png");

    this->loadSound("punch", "../assets/sounds/punch.wav", 20.f);
    this->loadSound("dead_male_zombie", "../assets/sounds/dead_male_zombie.wav", 30.f);
    this->loadSound("dead_female_zombie", "../assets/sounds/dead_female_zombie.wav", 30.f);
    this->loadSound("dead_human", "../assets/sounds/dead_human.wav", 80.f);

    this->openMusic("../assets/musics/background.wav");
    m_music.setLoop(true);

    m_sprites.emplace("road", m_textures.at("road"));
    m_sprites.emplace("cursor", m_textures.at("cursor"));

    m_sprites.at("cursor").setOrigin(m_sprites.at("cursor").getTextureRect().width / 2, m_sprites.at("cursor").getTextureRect().height / 2);
    m_sprites.at("cursor").setColor(sf::Color::Red);
}

void MainMenuState::checkEvents(sf::Event &p_event)
{
    if (p_event.type == sf::Event::KeyPressed && p_event.key.code == sf::Keyboard::Escape)
    {
        m_window.close();
    }
    else if (m_button.isHovered() && p_event.type == sf::Event::MouseButtonPressed && p_event.mouseButton.button == sf::Mouse::Left)
    {
        m_states.push(std::make_unique<GameState>(m_window, m_states, m_font, m_texts, m_textures, m_sprites, m_music, m_soundBuffers, m_sounds));
    }
}

void MainMenuState::update()
{
    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });
    m_button.update();
}

void MainMenuState::draw()
{
    m_window.draw(m_sprites.at("road"));
    m_window.draw(m_button);
    m_window.draw(m_sprites.at("cursor"));

    // m_window.draw(m_texts.at("title"));
    // m_window.draw(m_texts.at("title2"));
}
