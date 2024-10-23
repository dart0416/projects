#include "Game.h"


//Static functions

//Initializer functions

//Constructors / destructors



Game::Game()
{
    this->initWindow();
    this->initKeys();
    this->initStates();
}

Game::~Game()
{
    delete this->window;

    while(!this->states.empty())
    {
        delete this->states.top();
        this->states.pop();
    }
}

//=======================================

void Game::initWindow()
{

    ConfigParser Settings("src/config/config.ini");

    int height = Settings.getIntValue("HEIGHT");
    int width = Settings.getIntValue("WIDTH");
    std::string title = Settings.getStringValue("TITLE");
    int framerateLimit = Settings.getIntValue("MAX_FRAMERATE");
    bool isVerticalSyncEnabled = Settings.getBoolValue("VERTICAL_SYNC_ENABLE");
    // bool isFullScreenEnabled = Settings.getBoolValue("FULLSCREEN");
    // bool isAntialiasing = Settings.getBoolValue("ANTIALIASING");
    // Init from config file

    this->window = new sf::RenderWindow(sf::VideoMode(width, height), title/*, sf::Style::Titlebar | sf::Style::Close*/);
    this->window->setFramerateLimit(framerateLimit);
    this->window->setVerticalSyncEnabled(isVerticalSyncEnabled);
}

void Game::initKeys()
{
    this->supportedKeys["Escape"] = sf::Keyboard::Key::Escape;
    this->supportedKeys["A"] = sf::Keyboard::Key::A;
    this->supportedKeys["D"] = sf::Keyboard::Key::D;
    this->supportedKeys["W"] = sf::Keyboard::Key::W;
    this->supportedKeys["S"] = sf::Keyboard::Key::S;


}

void Game::initStates()
{
    this->states.push(new MainMenuState(this->window, &this->supportedKeys, &this->states));
}

void Game::updateEvents()
{
    while(this->window->pollEvent(this->Event))
    {
        if(this->Event.type == sf::Event::Closed)
            this->window->close();
    }
}

void Game::update()
{
    this->updateEvents();
    if(!this->states.empty())
    {
        this->states.top()->update(this->dt);
        if(this->states.top()->getQuit())
        {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    }
    else
    {
        this->window->close();
        this->endApplication();
    }
}

void Game::updateDt()
{
    //time var update and render 1 frame


    this->dt = this->dtClock.restart().asSeconds();

}

void Game::render()
{
    this->window->clear(sf::Color::Black);

    //Render items
    if(!this->states.empty())
        this->states.top()->render(this->window);

    this->window->display();
}

void Game::run()
{
    while(this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
    }
}

void Game::endApplication()
{
    std::cout << "Ending...\n";
}


