#include "game_threads.h"

void* terminal_display(void *pArg)
{
    gameData_t *pData = pArg;
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
                    if(i == pData->frog.y && j == pData->frog.x)
                    {
                        printf("F");
                    }
                    else if((*pData->pBoard)[j][i])
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
    return NULL;
}

void* input_thread(void *pArg)
{
   gameData_t *pData = pArg;
   char eventChar;
   
   changemode(BUFFERED_OFF);

   while(1)
   {
        if((kbhit()))
        {
            pthread_mutex_lock(&eventMutex);
            eventChar = getch();
            pData->event.flag = 1;
            switch(eventChar)
            {
                case KEYUP_ASCII: pData->event.type = UP_EVENT; break;
                case KEYDOWN_ASCII: pData->event.type = DOWN_EVENT; break;
                case KEYLEFT_ASCII: pData->event.type = LEFT_EVENT; break;
                case KEYRIGHT_ASCII: pData->event.type = RIGHT_EVENT; break;
                case ENTER_ASCII: pData->event.type = ENTER_EVENT; break;
            }
            pthread_mutex_unlock(&eventMutex);
        }

   }

  changemode(BUFFERED_ON);
}

void* game_thread(void *pArg)
{
    gameData_t *pData = pArg;
    while(1)
    {
        if(pData->lives <= 0)
        {
            pthread_mutex_lock(&eventMutex);
            pData->event.flag = 1;
            pData->event.type = GAME_OVER_EVENT;
            pthread_mutex_unlock(&eventMutex);
        }
        else if((*pData->pBoard)[pData->frog.x][pData->frog.y])
        {
            pthread_mutex_lock(&eventMutex);
            pData->event.flag = 1;
            pData->event.type = FROG_HIT_EVENT;
            pthread_mutex_unlock(&eventMutex);
        }
        else if(pData->frog.y == 0 && (pData->frog.x == 2 || pData->frog.x == 6 || pData->frog.x == 9 || pData->frog.x == 13))
        {
            pthread_mutex_lock(&eventMutex);
            pData->event.flag = 1;
            pData->event.type = LEVEL_UP_EVENT;
            pthread_mutex_unlock(&eventMutex);
        }
    }
}