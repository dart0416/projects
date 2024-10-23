#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <ctime>
#include <cstdlib>

#include "States/State.h"
#include "States/GameState.h"
#include "States/MainMenuState.h"

#include "Other/ConfigParser.cpp"


class Game
{
    private:
    //Variables
    
        sf::RenderWindow *window;
        sf::Event Event;

        sf::Clock dtClock;
        float dt;

        std::stack<State*> states;

        std::map<std::string, int> supportedKeys;

        void initWindow();
        void initStates();
        void initKeys();

    public:
    
        //Constructors / destructors

        Game();
        virtual ~Game();

        //regular
        void endApplication();
    
        //update
        void updateDt();
        void updateEvents();
        void update();

        //render
        void render();

        //core
        void run();

};

#endif
