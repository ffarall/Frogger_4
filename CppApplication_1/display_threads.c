#include "display_threads.h"

void terminal_display(void *pArg)
{
    gameData_t *pData = pArg;
    enum stateIDs 
    {
        START_MENU_PLAY_GAME, 
        START_MENU_VIEW_SCORE_BOARD, 
        START_MENU_QUIT, SCORE_BOARD, 
        GAME, 
        PAUSE_MENU_CONTINUE, 
        PAUSE_MENU_BACK_TO_START, 
        SAVE_SCORE_1_CHAR, 
        SAVE_SCORE_2_CHAR, 
        SAVE_SCORE_3_CHAR
    };
    
    while(!pData->quitGame)
    {
        
        switch(pData->currentState->stateID)
        {
            case START_MENU_PLAY_GAME: printf("START GAME\n"); break;
            case START_MENU_VIEW_SCORE_BOARD: printf("VIEW SCORE BOARD\n"); break;
            case START_MENU_QUIT: printf("QUIT GAME\n")

        }
    }
}