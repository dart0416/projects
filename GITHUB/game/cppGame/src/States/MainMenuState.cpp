#include "MainMenuState.h"
#include <fstream>
#include <string>


// Initializer
void MainMenuState::initVariables()
{
    this->background.setSize(
        sf::Vector2f
        (
            static_cast<float>(this->window->getSize().x),
            static_cast<float>(this->window->getSize().x)
        )
    );

    if(!this->backgroundTexture.loadFromFile("src/Resources/Images/Background/BG_1.png"))
    {
        throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
    }
    
    this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::initBackground()
{
    
}

void MainMenuState::initFonts()
{
    if(!this->font.loadFromFile("src/Resources/Fonts/YOUNGER_THAT_ME.TTF"))
    {
    throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
    }
}

void MainMenuState::initKeyBinds()
{

    this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");

    this->keybinds["MOVE_LEFT"] = this->supportedKeys->at("A");
    this->keybinds["MOVE_RIGHT"] = this->supportedKeys->at("D");
    this->keybinds["MOVE_UP"] = this->supportedKeys->at("W");
    this->keybinds["MOVE_DOWN"] = this->supportedKeys->at("S");


    std::ifstream ifs("src/config/keys.ini");
    ifs.close();

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

void MainMenuState::initButtons()
{

    this->buttons["GAME_STATE"] = new Button(
        10.f, 160.f, 150.f, 50.f,
        &this->font, "New Game", 20,
        sf::Color(70, 70, 70, 200), sf::Color(0, 255, 255, 250), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 30), sf::Color(20, 20, 20, 0)
    );

    this->buttons["SETTINGS"] = new Button(
        10.f, 230.f, 150.f, 50.f,
        &this->font, "Settings", 20,
        sf::Color(70, 70, 70, 200), sf::Color(0, 255, 255, 250), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 30), sf::Color(20, 20, 20, 0)
    );

    this->buttons["EDITOR_STATE"] = new Button(
        10.f, 300.f, 150.f, 50.f,
        &this->font, "Editor", 20,
        sf::Color(70, 70, 70, 200), sf::Color(0, 255, 255, 250), sf::Color(20, 20, 20, 50),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 30), sf::Color(20, 20, 20, 0)
    );

    this->buttons["EXIT_STATE"] = new Button(
        10.f, 500.f, 150.f, 50.f,
        &this->font, "Exit", 20,
        sf::Color(70, 70, 70, 195), sf::Color(255, 0, 0, 250), sf::Color(20, 20, 20, 210),
        sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 30), sf::Color(20, 20, 20, 0)
    );
}


//=================================================================================


MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
    : State(window, supportedKeys, states)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initKeyBinds();
    this->initButtons();
}

// Destructor definition
MainMenuState::~MainMenuState()
{
    auto it = this->buttons.begin();
    for(it = this->buttons.begin(); it != this->buttons.end(); ++it)
    {
        delete it->second;
    }
}


//=========================================================


void MainMenuState::update(const float& dt)
{
    this->updateMousePositions();
    this->updateInput(dt);
    this->updateButtons();

    // system("clear");    
    // std::cout << this->mousePosView.x << "   " << this->mousePosView.y << "\n";
    
}


void MainMenuState::updateInput(const float & dt)
{

}

void MainMenuState::updateButtons()
{
    // Update all buittons in state
    // Handle buttons functiolnality
    for(auto &it : this->buttons)
    {
        it.second->update(this->mousePosView);
    }

    // New game
    if(this->buttons["GAME_STATE"]->isPressed())
    {
        this->states->push(new GameState(this->window, this->supportedKeys, this->states));
    }

    // Setting


    // Editor
    if(this->buttons["EDITOR_STATE"]->isPressed())
    {
        this->states->push(new EditorState(this->window, this->supportedKeys, this->states));
    }

    //Quit
    if(this->buttons["EXIT_STATE"]->isPressed())
    {
        this->endState();
    }
}

// render() definition
void MainMenuState::render(sf::RenderTarget* target)
{
    if(!target)
        target = this->window;

    target->draw(this->background);

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


void MainMenuState::renderButtons(sf::RenderTarget& target)
{
    for(auto &it : this->buttons)
    {
        it.second->render(target);
    }
}