#include <avr/io.h>            
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h> 
#include <util/atomic.h> 
#include "def.h"

int main()
 {    
    uint_16 time_ms = 75;

    LED_DDR |= (1<<LED_B);
    LED_DDR |= (1<<LED_A);
    LED_DDR |= (1<<LED_F);
    LED_DDR |= (1<<LED_G);
    LED_DDR |= (1<<LED_H);
    LED_DDR |= (1<<LED_C);
    LED_DDR |= (1<<LED_D);
    LED_DDR |= (1<<LED_E);

    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_F);
    LED_PORT &= ~(1<<LED_G);
    LED_PORT &= ~(1<<LED_H);
    LED_PORT &= ~(1<<LED_C);
    LED_PORT &= ~(1<<LED_D);
    LED_PORT &= ~(1<<LED_E);

    /*
    while(1){

    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_C);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_C); // display 1


    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_G);
    LED_PORT |= (1<<LED_E);
    LED_PORT |= (1<<LED_D);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_G);
    LED_PORT &= ~(1<<LED_E);
    LED_PORT &= ~(1<<LED_D); // display 2

    
    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_G);
    LED_PORT |= (1<<LED_C);
    LED_PORT |= (1<<LED_D);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_G);
    LED_PORT &= ~(1<<LED_C);
    LED_PORT &= ~(1<<LED_D); // display 3

    LED_PORT |= (1<<LED_F);
    LED_PORT |= (1<<LED_G);
    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_C);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_F);
    LED_PORT &= ~(1<<LED_G);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_C); // display 4


    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_F);
    LED_PORT |= (1<<LED_G);
    LED_PORT |= (1<<LED_C);
    LED_PORT |= (1<<LED_D);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_F);
    LED_PORT &= ~(1<<LED_G);
    LED_PORT &= ~(1<<LED_C);
    LED_PORT &= ~(1<<LED_D); //display 5


    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_F);
    LED_PORT |= (1<<LED_G);
    LED_PORT |= (1<<LED_E);
    LED_PORT |= (1<<LED_D);
    LED_PORT |= (1<<LED_C);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_F);
    LED_PORT &= ~(1<<LED_G);
    LED_PORT &= ~(1<<LED_E);
    LED_PORT &= ~(1<<LED_D);
    LED_PORT &= ~(1<<LED_C); // display 6


    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_C);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_C); // display 7


    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_C);
    LED_PORT |= (1<<LED_D);
    LED_PORT |= (1<<LED_E);
    LED_PORT |= (1<<LED_F);
    LED_PORT |= (1<<LED_G);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_C);
    LED_PORT &= ~(1<<LED_D);
    LED_PORT &= ~(1<<LED_E);
    LED_PORT &= ~(1<<LED_F);
    LED_PORT &= ~(1<<LED_G); // display 8


    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_C);
    LED_PORT |= (1<<LED_D);
    LED_PORT |= (1<<LED_F);
    LED_PORT |= (1<<LED_G);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_C);
    LED_PORT &= ~(1<<LED_D);
    LED_PORT &= ~(1<<LED_F);
    LED_PORT &= ~(1<<LED_G); // display 9


    LED_PORT |= (1<<LED_A);
    LED_PORT |= (1<<LED_B);
    LED_PORT |= (1<<LED_C);
    LED_PORT |= (1<<LED_D);
    LED_PORT |= (1<<LED_E);
    LED_PORT |= (1<<LED_F);
    _delay_ms(time_ms);
    LED_PORT &= ~(1<<LED_A);
    LED_PORT &= ~(1<<LED_B);
    LED_PORT &= ~(1<<LED_C);
    LED_PORT &= ~(1<<LED_D);
    LED_PORT &= ~(1<<LED_E);
    LED_PORT &= ~(1<<LED_F); // display 0
    }
    */
    
    while(1)
    {
        LED_PORT |= (1<<LED_A);
        _delay_ms(time_ms);
        LED_PORT &= ~(1<<LED_A);

        LED_PORT |= (1<<LED_B);
        _delay_ms(time_ms);
        LED_PORT &= ~(1<<LED_B);

        LED_PORT |= (1<<LED_C);
        _delay_ms(time_ms);
        LED_PORT &= ~(1<<LED_C);

        LED_PORT |= (1<<LED_D);
        _delay_ms(time_ms);
        LED_PORT &= ~(1<<LED_D);

        LED_PORT |= (1<<LED_E);
        _delay_ms(time_ms); 
        LED_PORT &= ~(1<<LED_E);

        LED_PORT |= (1<<LED_F);
        _delay_ms(time_ms);
        LED_PORT &= ~(1<<LED_F);

    }
    









}   
