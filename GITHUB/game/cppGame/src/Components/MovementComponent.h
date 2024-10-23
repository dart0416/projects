#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H


#include <vector>    
#include <iostream>    
#include <ctime>    
#include <cstdlib>    
#include <stack>    
#include <map>
    
#include <SFML/System.hpp>    
#include <SFML/Window.hpp>    
#include <SFML/Graphics.hpp>    
#include <SFML/Audio.hpp>    
#include <SFML/Network.hpp>



enum movement_states {IDLE = 0, MOVING, MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN };

class MovementComponent
{
    private:

        sf::Sprite& sprite;

        float maxVelocity;
        float acceleration;
        float decelaration;


        sf::Vector2f velocity;

        // Init functions



    public:
        MovementComponent(sf::Sprite& sprite,
            float maxVelocity, float acceleration, float decelaration);
        virtual ~MovementComponent();

        // Accessors
        const float& getMaxVelocity() const;
        const sf::Vector2f& getVelocity() const;

        // Functions

        const bool getState(const short unsigned state) const;
        // const bool idle() const;
        // const bool moving() const;
        // const bool movingLeft() const;
        // const bool movingRight() const;

        void move(const float dir_x, const float dir_y, const float& dt);
        void update(const float& dt);
};

#endif