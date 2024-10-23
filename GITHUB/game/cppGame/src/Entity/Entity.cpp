#include "Entity.h"

void Entity::initVariables()
{
    this->hitboxComponent = NULL;
    this->movementComponent = NULL;
    this->animationComponent = NULL;
}

Entity::Entity()
{
    this->initVariables();
}

Entity::~Entity()
{
    delete this->hitboxComponent;
    delete this->movementComponent;
    delete this->animationComponent;
}

// Component functions

void Entity::setTexture(sf::Texture& texture)
{
    this->sprite.setTexture(texture);

    // this->sprite = new sf::Sprite();

    // if(this->sprite)
    // {
    //     this->texture = texture;
    //     this->sprite->setTexture(*this->texture);
    // } else
    // {
    //     std::cout << "Failed to create sprite\n";
    // }
}



void Entity::createHitboxComponent(sf::Sprite& sprite,
        float offset_x, float offset_y,
        float width, float height)
{
    this->hitboxComponent = new HitboxComponent(sprite, offset_x, offset_y, width, height);
}

void Entity::createMovementComponent(const float maxVelocity, const float acceleration, const float decelaration)
{
    this->movementComponent = new MovementComponent(this->sprite, maxVelocity, acceleration, decelaration);
}

void Entity::createAnimationComponent(/*sf::Sprite& sprite, */sf::Texture & texture_sheet)
{
    this->animationComponent = new AnimationComponent(this->sprite, texture_sheet);
}





// Functions

void Entity::setPosition(const float x, const float y)
{
    this->sprite.setPosition(x, y);
}

void Entity::move(const float dir_x, const float dir_y, const float& dt)
{
    if(this->movementComponent)
    {
        this->movementComponent->move(dir_x, dir_y, dt); // Sets velocity
    }
}

void Entity::update(const float & dt)
{
    
}

void Entity::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);

    if(this->hitboxComponent)
        this->hitboxComponent->render(target);
}
