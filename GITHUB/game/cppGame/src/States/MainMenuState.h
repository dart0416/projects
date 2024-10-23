#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include "EditorState.h"
#include "../Other/Button.h"

class MainMenuState:
    public State
{
    private:

        //Variables
        sf::Texture backgroundTexture;
        sf::RectangleShape background;
        sf::Font font;

        std::map<std::string, Button*> buttons;

        Button* gamestate_btn;

        //Function
        void initVariables();
        void initBackground();
        void initFonts();
        void initKeyBinds();
        void initButtons();

    public:
        MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
        virtual ~MainMenuState();

        void updateInput(const float& dt);
        void updateButtons();
        void update(const float& dt);
        void renderButtons(sf::RenderTarget& target);
        void render(sf::RenderTarget* target = nullptr);
};

#endif