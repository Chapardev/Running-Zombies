#include "GameState.hpp"

#include <boost/algorithm/string.hpp>
#include <effolkronium/random.hpp>

using Random = effolkronium::random_static;

void GameState::_setEntityCharacteritics(int p_randomValue, std::string &p_walkPath, std::string &p_diePath, int &p_score)
{
    if (p_randomValue <= 70)
    {
        // Male zombie
        p_walkPath = "male_zombie_walk";
        p_diePath = "male_zombie_die";
        p_score = 1;
    }
    else if (p_randomValue <= 90)
    {
        // Female zombie
        p_walkPath = "female_zombie_walk";
        p_diePath = "female_zombie_die";
        p_score = 5;
    }
    else
    {
        // Human
        p_walkPath = "human_walk";
        p_diePath = "human_die";
        p_score = -10;
    }
}

void GameState::_createEntitySprites(const std::string &p_walkPath, const std::string &p_diePath)
{
    m_sprites.emplace(p_walkPath + std::to_string(m_numberOfEntities), m_textures.at(p_walkPath + "_spritesheet"));
    m_sprites.at(p_walkPath + std::to_string(m_numberOfEntities)).setScale(0.25f, 0.25f);
    m_sprites.emplace(p_diePath + std::to_string(m_numberOfEntities), m_textures.at(p_diePath + "_spritesheet"));
    m_sprites.at(p_diePath + std::to_string(m_numberOfEntities)).setScale(0.25f, 0.25f);

    m_animatedSprites.push_back(
        std::make_unique<AnimatedSprite>(m_sprites.at(p_walkPath + std::to_string(m_numberOfEntities)), 0.05f, 10, true)
    );

    m_animatedSprites.push_back(
        std::make_unique<AnimatedSprite>(
            m_sprites.at(p_diePath + std::to_string(m_numberOfEntities)), 0.05f, (boost::algorithm::contains(p_diePath, "zombie") ? 12 : 10)
        )
    );
}

void GameState::_addEntity()
{
    std::string walkPath;
    std::string diePath;
    int score {  };

    this->_setEntityCharacteritics(Random::get(1, 100), walkPath, diePath, score);
    this->_createEntitySprites(walkPath, diePath);

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

void GameState::_removeUselessEntities()
{
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
            m_animatedSprites.erase(m_animatedSprites.begin() + entity->getId());
        }
    }

    // Removes all dead/out zombies from the vector (from first dead/out to last dead/out)
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(), [](const auto &entity) { return entity->isDead() || entity->isOutOfBounds(); }),
        m_entities.end()
    );
}

void GameState::_playSound(const std::string &p_keyName, float p_minValue)
{
    m_sounds.at(p_keyName).play();
    m_sounds.at(p_keyName).setPitch(Random::get(p_minValue, 1.f));
}

void GameState::_playEntityDeathSound(const Entity &p_entity)
{
    if (m_sprites.find("male_zombie_walk" + std::to_string(p_entity.getId())) != m_sprites.end())
    {
        this->_playSound("dead_male_zombie", 0.75f);
    }
    else if (m_sprites.find("female_zombie_walk" + std::to_string(p_entity.getId())) != m_sprites.end())
    {
        this->_playSound("dead_female_zombie", 0.75f);
    }
    else
    {
        this->_playSound("dead_human", 0.75f);
    }
}

GameState::GameState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
    Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
    Dictionary<sf::Sound> &p_sounds) 
    : State { p_window, p_states, p_font, p_texts, p_textures, p_sprites, p_music, p_soundBuffers, p_sounds }
{
    static bool createdOnce { false };
    if (!createdOnce)
    {
        this->createText("score", "", 10.f, 10.f, 20);
        createdOnce = true;
    } 
    this->_addEntity();
    m_music.play();
}

void GameState::checkEvents(sf::Event &p_event)
{
    if (p_event.type == sf::Event::KeyPressed && p_event.key.code == sf::Keyboard::Escape)
    {
        for (auto &pair : m_sounds)
        {
            if (pair.second.getStatus() == sf::Sound::Playing)
            {
                pair.second.stop();
            }
        }

        m_music.stop();
        m_states.pop();
    }
    else if (p_event.type == sf::Event::MouseButtonPressed && p_event.mouseButton.button == sf::Mouse::Left)
    {
        for (auto &entity : m_entities)
        {
            if (!entity->animationChanged() && entity->isTargeted(p_event.mouseButton.x, p_event.mouseButton.y))
            {
                entity->changeAnimation();
                m_score += entity->getScore();
                
                this->_playSound("punch", 0.5f);
                this->_playEntityDeathSound(*entity);
            }
        }
    }
}

void GameState::update()
{
    m_dt = m_dtClock.restart().asSeconds();
    m_sprites.at("cursor").setPosition(sf::Vector2f { sf::Mouse::getPosition(m_window) });

    if (m_score < 0)
    {
        m_score = 0;
    }

    m_texts.at("score").setString("SCORE: " + std::to_string(m_score));

    if (m_apparitionClock.getElapsedTime() > m_apparitionTime)
    {
        this->_addEntity();
        m_apparitionTime = sf::seconds(Random::get(1, 3) / 2.f);
        m_apparitionClock.restart();
    }

    this->_removeUselessEntities();

    for (auto &entity : m_entities)
    {
        entity->update(m_dt);
    }
}

void GameState::draw()
{
    m_window.draw(m_sprites.at("road"));
    
    // I use reverse iterators so that the last zombies walking won't be at the foreground in relation to the first ones
    for (auto it { m_entities.rbegin() }; it != m_entities.rend(); ++it)
    {
        m_window.draw(*(*it));
    }

    m_window.draw(m_sprites.at("cursor"));

    m_window.draw(m_texts.at("score"));
}
