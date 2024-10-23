#ifndef EDITORSTATE_H
#define EDITORSTATE_H


#include "State.h"
#include "../Other/Button.h"

class EditorState :
    public State
{
    private:

        //Variables
        sf::Font font;

        std::map<std::string, Button*> buttons;

        //Function
        void initVariables();
        void initBackground();
        void initFonts();
        void initKeyBinds();
        void initButtons();

    public:
        EditorState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
        virtual ~EditorState();

        // Functions
        void updateInput(const float& dt);
        void updateButtons();
        void update(const float& dt);
        void renderButtons(sf::RenderTarget& target);
        void render(sf::RenderTarget* target = nullptr);
};



#endif