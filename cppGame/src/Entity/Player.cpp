#include "Player.h"

// Init functions
void Player::initVariables()
{
    this->attacking = false;
}

void Player::initComponents()
{

}

//===============================================

Player::Player(float x, float y, sf::Texture& texture_sheet)
{
    this->initVariables();

    this->setPosition(x, y);

    // this->createHitboxComponent(this->sprite, 95.f, 65.f, 65.f, 111.f);
    this->createHitboxComponent(this->sprite, 86.f, 74.f, 86.f, 111.f);
    this->createMovementComponent(350.f, 25.f, 8.f);
    this->createAnimationComponent(texture_sheet);

    this->animationComponent->addAnimation("IDLE", 11.f, 0, 0, 13, 0,  192, 192);
    this->animationComponent->addAnimation("WALK", 7.f, 0, 1, 11, 1,  192, 192);
    this->animationComponent->addAnimation("ATTACK", 6.f, 0, 2, 13, 2,  192*2, 192);
}

Player::~Player()
{

}

//=============================================

// Functions

void Player::update(const float & dt)
{
    this->movementComponent->update(dt);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        this->attacking = true;
        //this->animationComponent->play("ATTACK", dt, true);
    }
    if(this->attacking)
    {
        this->animationComponent->play("ATTACK", dt, true);
    }
    if(this->movementComponent->getState(IDLE))
        this->animationComponent->play("IDLE", dt);
    else if(this->movementComponent->getState(MOVING_LEFT))
    {
        this->sprite.setOrigin(0.f, 0.f);
        this->sprite.setScale(1.f, 1.f);
        this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
    }
    else if(this->movementComponent->getState(MOVING_RIGHT))
    {
        this->sprite.setOrigin(258.f, 0.f);
        this->sprite.setScale(-1.f, 1.f);
        this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
    }
    else if(this->movementComponent->getState(MOVING_UP))
    {
        this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
    }
    else if(this->movementComponent->getState(MOVING_DOWN))
    {
        this->animationComponent->play("WALK", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
    }
       
       
        

    this->hitboxComponent->update();
}
