#include "main.h"

int main(int argc, char** argv) 
{
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_SOURCE lives;
    al_init_user_event_source(&lives); //Creación de un tipo de evento de software para cuando se acaban las vidas.
    ALLEGRO_EVENT_SOURCE levelUp;
    al_init_user_event_source(&levelUp); //Creación de un tipo de evento de software para cuando se llega al otro lado del cruce y se sube de nivel.
    

/************************** CREACIÓN DE TABLAS DE ESTADOS ****************************/
    state_t startMenuPlayGame[2+1]; //Cada arreglo de estado tiene tantos elementos state_t como posibles eventos que espera recibir +1 (END_TABLE).
    state_t startMenuViewScoreBoard[3+1];
    state_t startMenuQuit[2+1];
    state_t scoreBoard[1+1];
    state_t game[8+1];
    state_t pauseMenuContinue[2+1];
    state_t pauseMenuBackToStart[2+1];
    state_t saveScore1Char[3+1];
    state_t saveScore2Char[3+1];
    state_t saceScore3Char[3+1];
    
    startMenuPlayGame[0].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
    startMenuPlayGame[0].nextState = startMenuViewScoreBoard;
    startMenuPlayGame[1].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
    startMenuPlayGame[1].nextState = game;
    startMenuPlayGame[2].event.user.data1 = END_TABLE;
    startMenuPlayGame[2].nextState = startMenuPlayGame;
    
/************************************************************************************/
    
    return (EXIT_SUCCESS);
}

