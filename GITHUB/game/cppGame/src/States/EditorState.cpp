#include "EditorState.h"
#include <fstream>

void EditorState::initVariables()
{

}

void EditorState::initBackground()
{
    
}

void EditorState::initFonts()
{
    if(!this->font.loadFromFile("src/Resources/Fonts/YOUNGER_THAT_ME.TTF"))
    {
    throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT");
    }
}

void EditorState::initKeyBinds()
{

    this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");

    // this->keybinds["MOVE_LEFT"] = this->supportedKeys->at("A");
    // this->keybinds["MOVE_RIGHT"] = this->supportedKeys->at("D");
    // this->keybinds["MOVE_UP"] = this->supportedKeys->at("W");
    // this->keybinds["MOVE_DOWN"] = this->supportedKeys->at("S");


    // std::ifstream ifs("src/config/editorkeys.ini");
    // ifs.close();

    // if (ifs.is_open())
    // {
    //     std::string key = "";
    //     std::string key2 = "";

    //     while (ifs >> key >> key2)
    //     {
    //         if (this->supportedKeys->find(key2) != this->supportedKeys->end()) {
    //             this->keybinds[key] = this->supportedKeys->at(key2);
    //         } else {
    //             std::cerr << "Key binding error: " << key2 << " not found in supportedKeys.\n";
    //         }
    //     }
    //     ifs.close();
    // }
    // else
    // {
    //     std::cerr << "Unable to open key bindings configuration file.\n";
    // }



}

void EditorState::initButtons()
{

}


//=================================================================================


EditorState::EditorState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
    : State(window, supportedKeys, states)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initKeyBinds();
    this->initButtons();
}

// Destructor definition
EditorState::~EditorState()
{
    auto it = this->buttons.begin();
    for(it = this->buttons.begin(); it != this->buttons.end(); ++it)
    {
        delete it->second;
    }
}


//=========================================================


void EditorState::update(const float& dt)
{
    this->updateMousePositions();
    this->updateInput(dt);
    this->updateButtons();

    // system("clear");    
    // std::cout << this->mousePosView.x << "   " << this->mousePosView.y << "\n";
    
}


void EditorState::updateInput(const float & dt)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE")))) {
        this->endState();
    }
}

void EditorState::updateButtons()
{
    // Update all buittons in state
    // Handle buttons functiolnality
    for(auto &it : this->buttons)
    {
        it.second->update(this->mousePosView);
    }
}

// render() definition
void EditorState::render(sf::RenderTarget* target)
{
    if(!target)
        target = this->window;


    this->renderButtons(*target);


    // To remove later
    // sf::Text mouseText;
    // mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
    // mouseText.setFont(this->font);
    // mouseText.setCharacterSize(12);
    // std::stringstream ss;
    // ss << this->mousePosView.x << "  " << this->mousePosView.y;
    // mouseText.setString(ss.str());

    // target->draw(mouseText);
}


void EditorState::renderButtons(sf::RenderTarget& target)
{
    for(auto &it : this->buttons)
    {
        it.second->render(target);
    }
}