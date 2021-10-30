#ifndef SRC_MAIN_MENU_STATE_HPP
#define SRC_MAIN_MENU_STATE_HPP

#include "State.hpp"

#include "Button.hpp"

class MainMenuState : public State
{
public:
    MainMenuState(sf::RenderWindow &p_window, std::stack<std::unique_ptr<State>> &p_states, sf::Font &p_font, Dictionary<sf::Text> &p_texts, 
        Dictionary<sf::Texture> &p_textures, Dictionary<sf::Sprite> &p_sprites, sf::Music &p_music, Dictionary<sf::SoundBuffer> &p_soundBuffers,
        Dictionary<sf::Sound> &p_sounds);

    void checkEvents(sf::Event &p_event) override;
    void update() override;
    void draw() override;

private:
    Button m_button;
};

#endif // SRC_MAIN_MENU_STATE_HPP
