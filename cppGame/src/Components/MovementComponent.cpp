#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::Sprite& sprite,
    float maxVelocity, float acceleration, float decelaration)
    : sprite(sprite),
    maxVelocity(maxVelocity), acceleration(acceleration), decelaration(decelaration)
{

}

MovementComponent::~MovementComponent()
{

}

const sf::Vector2f & MovementComponent::getVelocity() const
{
    return this->velocity;
}

// Functions
const bool MovementComponent::getState(const short unsigned state) const
{
    switch(state)
    {
        case IDLE:
            if(this->velocity.x == 0.f && this->velocity.y == 0.f)
                return true;

            break;

        case MOVING:
            if(this->velocity.x != 0.f || this->velocity.y != 0.f)
                return true;

            break;

        case MOVING_LEFT:
            if(this->velocity.x < 0.f)
                return true;

            break;

        case MOVING_RIGHT:
            if(this->velocity.x > 0.f)
                return true;

            break;

        case MOVING_UP:
            if(this->velocity.y < 0.f)
                return true;

            break;

        case MOVING_DOWN:
            if(this->velocity.y > 0.f)
                return true;

            break;
    }

    return false;
}

const float& MovementComponent::getMaxVelocity() const
{
    return this->maxVelocity;
}

void MovementComponent::move(const float dir_x, const float dir_y, const float& dt)
{
    
    /* Accelerating until reach max velocity */

    // Acceleration
    this->velocity.x += this->acceleration * dir_x;

    this->velocity.y += this->acceleration * dir_y;

}

void MovementComponent::update(const float& dt)
{
    /* Decelerates the entity and controls max velocity.
    Moves the sprite */

    if(this->velocity.x > 0.f) // Check for positive x
    {   
        // Max velocity check x positive
        if(this->velocity.x > this->maxVelocity)
            this->velocity.x = this->maxVelocity;


        // Deceleration
        this->velocity.x -= decelaration;
        if(this->velocity.x < 0.f)
            this->velocity.x = 0.f;

    }
    else if(this->velocity.x < 0.f) // Check for negative x
    {
        // Max velocity check x negative
        if(this->velocity.x < -this->maxVelocity)
            this->velocity.x = -this->maxVelocity;
        
        // Deceleration
        this->velocity.x += decelaration;
        if(this->velocity.x > 0.f)
            this->velocity.x = 0.f;
    }


    if(this->velocity.y > 0.f) // Check for positive y
    {   
        // Max velocity check y positive
        if(this->velocity.y > this->maxVelocity)
            this->velocity.y = this->maxVelocity;


        // Deceleration
        this->velocity.y -= decelaration;
        if(this->velocity.y < 0.f)
            this->velocity.y = 0.f;

    }
    else if(this->velocity.y < 0.f) // Check for negative y
    {
        // Max velocity check y negative
        if(this->velocity.y < -this->maxVelocity)
            this->velocity.y = -this->maxVelocity;
        
        // Deceleration
        this->velocity.y += decelaration;
        if(this->velocity.y > 0.f)
            this->velocity.y = 0.f;
    }
    
    // Final move
    this->sprite.move(this->velocity * dt); 
}