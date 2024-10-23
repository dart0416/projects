#ifndef ENTITY_H
#define ENTITY_H

#include "../Components/MovementComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/HitboxComponent.h"

class Entity
{
    private:
        void initVariables();


    protected:
        sf::Sprite sprite;

        HitboxComponent* hitboxComponent;
        MovementComponent* movementComponent;
        AnimationComponent* animationComponent;

    public:
        Entity();
        virtual ~Entity();

        // Component functions
        void setTexture(sf::Texture& texture);
        void createHitboxComponent(sf::Sprite& sprite,
            float offset_x, float offset_y,
            float width, float height
        );
        void createMovementComponent(const float maxVelocity, const float acceleration, const float decelaration);
        void createAnimationComponent(/*sf::Sprite& sprite,*/ sf::Texture& texture_sheet);

        // Functions

        virtual void setPosition(const float x, const float y);
        virtual void move(const float x, const float y, const float& dt);

        virtual void update(const float& dt);
        virtual void render(sf::RenderTarget& target);




};

#endif
