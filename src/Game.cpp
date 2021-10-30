#include "Game.hpp"

#include <boost/algorithm/string.hpp>
#include <effolkronium/random.hpp>

#include <fstream>
#include <iostream>

using Random = effolkronium::random_static;

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

void Game::_loadImage(sf::Image &p_image, const std::string &p_fileName) const
{
    if (!p_image.loadFromFile(p_fileName))
    {
        throw "Unable to load image \"" + p_fileName + '"';
    }
}

void Game::_loadTexture(const std::string &p_keyName, const std::string &p_fileName)
{
    if (!m_textures[p_keyName].loadFromFile(p_fileName))
    {
        throw "Unable to load texture \"" + p_fileName + '"';
    }
}

void Game::_loadFont(const std::string &p_fileName)
{
    if (!m_font.loadFromFile(p_fileName))
    {
        throw "Unable to load font \"" + p_fileName + '"';
    }   
}

void Game::_loadMusic(const std::string &p_fileName)
{
    if (!m_music.openFromFile(p_fileName))
    {
        throw "Unable to load music \"" + p_fileName + '"';
    }
}

void Game::_loadSoundBuffer(const std::string &p_keyName, const std::string &p_fileName)
{
    if (!m_soundBuffers[p_keyName].loadFromFile(p_fileName))
    {
        throw "Unable to load sound buffer \"" + p_fileName + '"';
    }
}

void Game::_addEntity()
{
    const int randomValue { Random::get(1, 100) };

    std::string walkPath {  };
    std::string diePath {  };
    int score {  };

    if (randomValue <= 70)
    {
        // Male zombie
        walkPath = "male_zombie_walk";
        diePath = "male_zombie_die";
        score = 1;
    }
    else if (randomValue <= 90)
    {
        // Female zombie
        walkPath = "female_zombie_walk";
        diePath = "female_zombie_die";
        score = 5;
    }
    else
    {
        // Human
        walkPath = "human_walk";
        diePath = "human_die";
        score = -10;
    }

    m_sprites.emplace(walkPath + std::to_string(m_numberOfEntities), m_textures.at(walkPath + "_spritesheet"));
    m_sprites.at(walkPath + std::to_string(m_numberOfEntities)).setScale(0.25f, 0.25f);
    m_sprites.emplace(diePath + std::to_string(m_numberOfEntities), m_textures.at(diePath + "_spritesheet"));
    m_sprites.at(diePath + std::to_string(m_numberOfEntities)).setScale(0.25f, 0.25f);

    m_animatedSprites.push_back(std::make_unique<AnimatedSprite>(m_sprites.at(walkPath + std::to_string(m_numberOfEntities)), 0.05f, 10));
    m_animatedSprites.push_back(
        std::make_unique<AnimatedSprite>(
            m_sprites.at(diePath + std::to_string(m_numberOfEntities)), 0.05f, (boost::algorithm::contains(diePath, "zombie") ? 12 : 10)
        )
    );

    m_entities.push_back(
        std::make_unique<Entity>(
            m_window,
            *(m_animatedSprites[m_animatedSprites.size()-2]),
            *(m_animatedSprites.back()),
            m_numberOfEntities,
            score
        )
    );

    ++m_numberOfEntities;
}

Game::Game()
{
    sf::Image icon;
    this->_loadImage(icon, "../assets/images/icon.png");
    m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    m_window.setMouseCursorVisible(false);

    this->_loadFont("../assets/fonts/PressStart2P.ttf");

    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(15);
    m_scoreText.setPosition(10.f, 10.f);
    m_scoreText.setOutlineThickness(2.f);
    m_scoreText.setOutlineColor(sf::Color::Black);

    this->_loadTexture("road", "../assets/images/road.jpg");
    this->_loadTexture("cursor", "../assets/images/cursor.png");
    this->_loadTexture("human_walk_spritesheet", "../assets/images/human_walk_spritesheet.png");
    this->_loadTexture("human_die_spritesheet", "../assets/images/human_die_spritesheet.png");
    this->_loadTexture("male_zombie_walk_spritesheet", "../assets/images/zombie/male_walk_spritesheet.png");
    this->_loadTexture("male_zombie_die_spritesheet", "../assets/images/zombie/male_die_spritesheet.png");
    this->_loadTexture("female_zombie_walk_spritesheet", "../assets/images/zombie/female_walk_spritesheet.png");
    this->_loadTexture("female_zombie_die_spritesheet", "../assets/images/zombie/female_die_spritesheet.png");

    m_sprites.emplace("road", m_textures.at("road"));
    m_sprites.emplace("cursor", m_textures.at("cursor"));

    m_sprites.at("cursor").setOrigin(m_sprites.at("cursor").getTextureRect().width / 2, m_sprites.at("cursor").getTextureRect().height / 2);
    m_sprites.at("cursor").setColor(sf::Color::Red);

    this->_loadSoundBuffer("punch", "../assets/sounds/punch.wav");
    m_sounds["punch"].setBuffer(m_soundBuffers.at("punch"));
    m_sounds.at("punch").setVolume(30.f);

    this->_loadMusic("../assets/musics/background.wav");
    m_music.setLoop(true);
    m_music.play();

    this->_addEntity();
}

void Game::pollEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        {
            m_window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            int i { 0 };
            for (auto &entity : m_entities)
            {
                if (!entity->animationChanged() && entity->isTargeted(event.mouseButton.x, event.mouseButton.y))
                {
                    entity->changeAnimation();
                    m_score += entity->getScore();
                    
                    m_sounds.at("punch").setPitch(Random::get(0.5f, 1.f));
                    m_sounds.at("punch").play();
                }
                ++i;
            }
        }
    }
}

void Game::update()
{
    m_dt = m_dtClock.restart().asSeconds();

    if (m_score < 0)
    {
        m_score = 0;
    }

    m_scoreText.setString("SCORE: " + std::to_string(m_score));

    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });

    if (m_apparitionClock.getElapsedTime() > m_apparitionTime)
    {
        this->_addEntity();
        m_apparitionTime = sf::seconds(Random::get(1, 3) / 2.f);
        m_apparitionClock.restart();
    }

    for (auto &entity : m_entities)
    {
        if (entity->isDead() || entity->isOutOfBounds())
        {
            if (m_sprites.find("male_zombie_walk" + std::to_string(entity->getId())) != m_sprites.end())
            {
                m_sprites.erase("male_zombie_walk" + std::to_string(entity->getId()));
                m_sprites.erase("male_zombie_die" + std::to_string(entity->getId()));
            }
            else if (m_sprites.find("female_zombie_walk" + std::to_string(entity->getId())) != m_sprites.end())
            {
                m_sprites.erase("female_zombie_walk" + std::to_string(entity->getId()));
                m_sprites.erase("female_zombie_die" + std::to_string(entity->getId()));
            }
            else
            {
                m_sprites.erase("human_walk" + std::to_string(entity->getId()));
                m_sprites.erase("human_die" + std::to_string(entity->getId()));
            }

            // Erases walk and die animations of the entity
            m_animatedSprites.erase(m_animatedSprites.begin() + entity->getId());
            m_animatedSprites.erase(m_animatedSprites.begin() + entity->getId() + 1);
        }
    }

    // Removes all dead/out zombies from the vector (from first dead/out to last dead/out)
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(), [](const auto &entity) { return entity->isDead() || entity->isOutOfBounds(); }),
        m_entities.end()
    );

    int i { 0 };
    for (auto &entity : m_entities)
    {
        entity->update(m_dt);
        ++i;
    }
}

void Game::draw()
{
    m_window.clear();

    m_window.draw(m_sprites.at("road"));

    for (const auto &entity : m_entities)
    {
        m_window.draw(*entity);
    }

    m_window.draw(m_sprites.at("cursor"));

    m_window.draw(m_scoreText);

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
