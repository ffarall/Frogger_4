#include "display_threads.h"

void* terminal_display(void *pArg)
{
    gameData_t *pData = (gameData_t*)pArg;
    int i,j;
    enum stateIDs 
    {
        START_MENU_PLAY_GAME, 
        START_MENU_VIEW_SCORE_BOARD, 
        START_MENU_QUIT, 
        SCORE_BOARD, 
        GAME, 
        PAUSE_MENU_CONTINUE, 
        PAUSE_MENU_BACK_TO_START, 
        SAVE_SCORE_1_CHAR, 
        SAVE_SCORE_2_CHAR, 
        SAVE_SCORE_3_CHAR
    };
    
    while(!(pData->quitGame))
    {
        usleep(1000); // El display se actualiza cada 1 milisegundo.
        switch(pData->currentState->stateID)
        {
            case START_MENU_PLAY_GAME: 
            {
                clear_disp();
                printf("START GAME\n");
            }break;
            case START_MENU_VIEW_SCORE_BOARD: 
            {
                clear_disp();
                printf("VIEW SCORE BOARD\n");
            }break;
            case START_MENU_QUIT: 
            {
                clear_disp();
                printf("QUIT GAME\n");
            }break;
            case SCORE_BOARD:
            {
                clear_disp();
                printf("HERE YOU'D FIND THE SCOREBOARD IF THERE WHERE ANY\n");
            }break;
            case GAME:
            {
                clear_disp();
                for(i = 0; i < BOARD_SIZE; i++)
                {
                    for(j = 0; j < BOARD_SIZE; j++)
                    {
                        if(i == pData->frog[0] && j == pData->frog[1])
                        {
                            printf("F");
                        }
                        else if(pData->pBoard[i][j])
                        {
                            printf("*");
                        }
                        else
                        {
                            printf(" ");
                        }
                    }
                    printf("\n");
                }
                
            }break;
            case PAUSE_MENU_CONTINUE:
            {
                clear_disp();
                printf("CONTINUE\n");
            }break;
            case PAUSE_MENU_BACK_TO_START:
            {
                clear_disp();
                printf("BACK TO START\n");
            }break;
            case SAVE_SCORE_1_CHAR:
            {
                clear_disp();
                printf("%c %c %c\n", pData->player[0], pData->player[1], pData->player[2]);
            }break;
            case SAVE_SCORE_2_CHAR:
            {
                clear_disp();
                printf("%c %c %c\n", pData->player[0], pData->player[1], pData->player[2]);
            }break;
            case SAVE_SCORE_3_CHAR:
            {
                clear_disp();
                printf("%c %c %c\n", pData->player[0], pData->player[1], pData->player[2]);
            }break;
        }
    }
    return NULL;
}