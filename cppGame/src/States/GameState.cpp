#include "GameState.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


// Init functions

void GameState::initPlayers()
{
    this->player = new Player(0, 0, this->textures["PLAYER_SHEET"]);
}

void GameState::initKeyBinds()
{

    this->keybinds["CLOSE"] = this->supportedKeys->at("Escape");

    this->keybinds["MOVE_LEFT"] = this->supportedKeys->at("A");
    this->keybinds["MOVE_RIGHT"] = this->supportedKeys->at("D");
    this->keybinds["MOVE_UP"] = this->supportedKeys->at("W");
    this->keybinds["MOVE_DOWN"] = this->supportedKeys->at("S");

}

void GameState::initTextures()
{
    //sf::Texture temp;
    //temp.loadFromFile("src/Resources/Images/Sprites/Player/Korwin.jpg");
    if(!this->textures["PLAYER_SHEET"].loadFromFile("src/Resources/Images/Sprites/Player/PLAYER_SHEET.png"))
    {
        throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
    }
}

// Constructors & Destructors

GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
    : State(window, supportedKeys, states)
{
    this->initKeyBinds();
    this->initTextures();
    this->initPlayers();
}

// Destructor definition
GameState::~GameState()
{
    delete this->player;
}

// update() definition
void GameState::update(const float& dt)
{
    this->updateMousePositions();
    this->updateInput(dt);

    this->player->update(dt);
}

// render() definition
void GameState::render(sf::RenderTarget* target)
{
    if(!target)
        target = this->window;

    this->player->render(*target);
}

void GameState::updateInput(const float & dt)
{


    //Update player input
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT")))) {
        this->player->move(-1.f, 0.f, dt);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT")))) {
        this->player->move(1.f, 0.f, dt);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP")))) {
        this->player->move(0.f, -1.f, dt);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN")))) {
        this->player->move(0.f, 1.f, dt);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE")))) {
        this->endState();
    }
}