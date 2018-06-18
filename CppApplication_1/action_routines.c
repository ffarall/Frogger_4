#include "fsm.h"

void non_act_routine(void *)
{
    
}

void cars_routine(void *)
{
    
}





void shift_rigt_row(boolean_t row[BOARD_SIZE])
{
    boolean_t aux1 = row[BOARD_SIZE-1], aux2;
    int i;
    for(i = 0; i < BOARD_SIZE; i++)
    {
        aux2 = row[i];
        row[i] = aux1;
        aux1 = aux2;
    }
}

void shift_left_row(boolean_t row[BOARD_SIZE])
{
    boolean_t aux1 = row[0], aux2;
    int i;
    for(i = 0; i < BOARD_SIZE; i++)
    {
        aux2 = row[15-i];
        row[15-i] = aux1;
        aux1 = aux2;
    }
}