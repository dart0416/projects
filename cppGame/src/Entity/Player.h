#ifndef PLAYER_H
#define PLAYET_H

#include "Entity.h"

class Player :
    public Entity
{
    private:
        // Variables
        bool attacking;


        // Init functions
        void initVariables();
        void initComponents();


    public:
        Player(float x, float y, sf::Texture& texture_sheet);
        virtual ~Player();

        // Functions
        virtual void update(const float& dt);
};








#endif