#include "GameState.hpp"

#include <effolkronium/random.hpp>

using Random = effolkronium::random_static;

GameState::GameState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
    Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
    Dictionary<sf::Sound> &p_sounds) 
    : State { p_window, p_states, p_font, p_texts, p_textures, p_sprites, p_music, p_soundBuffers, p_sounds }
{
    if (!s_launchedOnce)
    {
        this->createText("score", "", 10, 10, 20);
        this->createText(
            "lives", "", m_texts.at("score").getPosition().x, 
            m_texts.at("score").getPosition().y + m_texts.at("score").getCharacterSize() + 10.f, 20
        );

        this->createText("begin", "HERE WE GO!", 0, 0);
        m_texts.at("begin").setPosition(
            (m_window.getSize().x - m_texts.at("begin").getGlobalBounds().width) / 2.f,
            (m_window.getSize().y - m_texts.at("begin").getGlobalBounds().height)  * 0.75f
        );

        this->createText("lost", "YOU LOST!", 0, 0);
        m_texts.at("lost").setPosition(
            (m_window.getSize().x - m_texts.at("lost").getGlobalBounds().width) / 2.f,
            (m_window.getSize().y - m_texts.at("lost").getGlobalBounds().height) / 2.f
        );

        this->createText("play_again", "Click to play again", 0, 0, 10);
        m_texts.at("play_again").setPosition(
            (m_window.getSize().x - m_texts.at("play_again").getGlobalBounds().width) / 2.f,
            m_texts.at("lost").getPosition().y + m_texts.at("play_again").getGlobalBounds().height + 30
        );
    }

    this->addEntity();
    m_music.play();
}

void GameState::setEntityCharacteritics(int p_randomValue, std::string &p_path, sf::IntRect &p_walkRect, sf::IntRect &p_dieRect, int &p_score)
{
    if (p_randomValue <= 70)
    {
        // Male zombie
        p_path = "male_zombie";
        p_walkRect = { 0, 0, 430, 519 };
        p_dieRect = { 0, 519, 629, 526 };
        p_score = 1;
    }
    else if (p_randomValue <= 90)
    {
        // Female zombie
        p_path = "female_zombie";
        p_walkRect = { 0, 1045, 521, 576 };
        p_dieRect = { 0, 1045 + 576, 684, 627 };
        p_score = 5;
    }
    else
    {
        // Human
        p_path = "human";
        p_walkRect = { 0, 2248, 415, 507 };
        p_dieRect = { 0, 2248 + 507, 588, 600 };
        p_score = 0;
    }
}

void GameState::createEntitySprites(const std::string &p_path, const sf::IntRect &p_walkRect, const sf::IntRect &p_dieRect)
{
    m_sprites.emplace(std::to_string(m_numberOfEntities), m_textures.at("spritesheet"));
    m_sprites.at(std::to_string(m_numberOfEntities)).setScale(0.25f, 0.25f);
    m_sprites.emplace(std::to_string(m_numberOfEntities) + "die", m_textures.at("spritesheet"));
    m_sprites.at(std::to_string(m_numberOfEntities) + "die").setScale(0.25f, 0.25f);

    m_animatedSprites.push_back(std::make_unique<AnimatedSprite>(m_sprites.at(std::to_string(m_numberOfEntities)), p_walkRect, 10, 0.05f, true));
    m_animatedSprites.push_back(
        std::make_unique<AnimatedSprite>(
            m_sprites.at(std::to_string(m_numberOfEntities) + "die"), p_dieRect, (boost::algorithm::contains(p_path, "zombie") ? 12 : 10), 0.05f
        )
    );
}

void GameState::addEntity()
{
    std::string type;
    sf::IntRect walkRect;
    sf::IntRect dieRect;
    int score {  };

    this->setEntityCharacteritics(Random::get(1, 100), type, walkRect, dieRect, score);
    this->createEntitySprites(type, walkRect, dieRect);

    m_entities.push_back(std::make_unique<Entity>(m_window, type, m_numberOfEntities, score, m_entitySpeed));
    m_entities.back()->addAnimatedSprite("walk", **(m_animatedSprites.end()-2));
    m_entities.back()->addAnimatedSprite("die", *m_animatedSprites.back());

    ++m_numberOfEntities;
}

void GameState::updateClickedEntity(float p_x, float p_y)
{
    const auto &clickedEntity { 
        std::find_if(
            m_entities.begin(), m_entities.end(), 
            [=](const auto &entity) { return !entity->animationChanged() && entity->isTargeted(p_x, p_y); }
        ) 
    };

    if (clickedEntity != m_entities.end())
    {
        (*clickedEntity)->changeAnimation();
        m_score += (*clickedEntity)->getScore();
        
        this->playSound("punch", 0.5f);

        if (!(*clickedEntity)->isZombie())
        {
            --m_lives;
            m_sounds.at("dead_human").play();
        }
        else if (Random::get<bool>())
        {
            m_sounds.at("dead_male_zombie").play();
        }
        else
        {
            m_sounds.at("dead_female_zombie").play();
        }
    }
}

void GameState::removeUselessEntities()
{
    for (auto &entity : m_entities)
    {
        if (entity->isOutOfBounds() && entity->isZombie())
        {
            --m_lives;
        }

        if (entity->isDead() || entity->isOutOfBounds())
        {
            m_sprites.erase(std::to_string(entity->getId()));
            m_sprites.erase(std::to_string(entity->getId()) + "die");

            // Erases walk and die animations of the entity
            m_animatedSprites.erase(m_animatedSprites.begin() + entity->getId());
            m_animatedSprites.erase(m_animatedSprites.begin() + entity->getId());
        }
    }

    // Removes all dead/out zombies from the vector (from first dead/out to last dead/out)
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(), [](const auto &entity) { return entity->isDead() || entity->isOutOfBounds(); }),
        m_entities.end()
    );
}

void GameState::playSound(const std::string &p_keyName, float p_minValue)
{
    m_sounds.at(p_keyName).play();
    m_sounds.at(p_keyName).setPitch(Random::get(p_minValue, 1.f));
}

void GameState::checkEvents(sf::Event &p_event)
{
    if (s_launchedOnce && p_event.type == sf::Event::KeyPressed && p_event.key.code == sf::Keyboard::Escape)
    {
        m_music.stop();
        m_states.pop();
    }
    else if (p_event.type == sf::Event::MouseButtonPressed && p_event.mouseButton.button == sf::Mouse::Left)
    {
        if (m_lives > 0)
        {
            this->updateClickedEntity(p_event.mouseButton.x, p_event.mouseButton.y);
        }
        else if (!m_entities.empty())
        {
            // Relaunch game
            m_entities.clear();
            m_music.play();
            m_lives = m_livesMax;
            m_score = 0;
            this->addEntity();
        }
    }
}

void GameState::update()
{
    m_dt = m_dtClock.restart().asSeconds();
    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });

    if (m_lives > 0)
    {
        if (m_score < 0)
        {
            m_score = 0;
        }
        
        m_texts.at("score").setString("SCORE: " + std::to_string(m_score));
        m_texts.at("lives").setString("LIVES: " + std::to_string(m_lives));

        if (m_apparitionClock.getElapsedTime() > m_apparitionTime)
        {
            this->addEntity();
            m_apparitionClock.restart();
        }

        this->removeUselessEntities();

        for (auto &entity : m_entities)
        {
            entity->update(m_dt);
        }   
    }
    else if (m_music.getStatus() == sf::Music::Playing)
    {
        m_music.stop();
    }
}

void GameState::draw()
{
    m_window.draw(m_sprites.at("road"));
    
    if (m_lives > 0)
    {
        // I use reverse iterators so that the last zombies walking won't be at the foreground in relation to the first ones
        std::for_each(m_entities.rbegin(), m_entities.rend(), [this](const auto &entity) { m_window.draw(*entity); });
    }

    m_window.draw(m_texts.at("score"));
    m_window.draw(m_texts.at("lives"));

    if (!s_launchedOnce)
    {
        if (m_clock.getElapsedTime().asSeconds() < 3.f)
        {
            m_window.draw(m_texts.at("begin"));
        }
        else
        {
            s_launchedOnce = true;
        }
    }
    else if (m_lives == 0)
    {
        m_window.draw(m_texts.at("lost"));
        m_window.draw(m_texts.at("play_again"));
    }

    m_window.draw(m_sprites.at("cursor"));
}
