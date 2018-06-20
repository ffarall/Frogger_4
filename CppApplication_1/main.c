#include "main.h"

int main(int argc, char* argv[]) 
{
    event_t event;
    
    boolean_t board[BOARD_SIZE][BOARD_SIZE];
    init_board(board);
    scorer_t top10[10]; // Arreglo de top 10 de puntajes. 
    // ESTE ARREGLO DEBE SER LLENADO AL INICIO DEL PROGRAMA CON EL TOP10 GUARDADO EN UN ARCHIVO Y VACIADO EN ESE MISMO ARCHIVO AL FINALIZAR.
    // EL ARREGLO ESTÁ PARA FACILITAR OPERACIONES DE ORDENAMIENTO DEL TOP10, YA QUE DE ESTE MODO SE PUEDE USAR QSORT, POR EJEMPLO.
    // UN JUGADOR ENTRARÍA EN EL TOP10 SI SU PUNTAJE SUPERA AL ÚLTIMO DE LA LISTA ORDENADA, POSTERIORMENTE SE REORDENA CON QSORT Y EL JUGADOR QUEDA EN LA POSICIÓN CORRESPONDIENTE.
    
    gameData_t gameData; // Se crea este objeto que contiene toda la información necesaria para manejar el juego, de modo que las rutinas de acción puedan recibir un puntero a este objeto.
    gameData_t *pGameData = &gameData;
    /************* INICIALIZACIÓN DE GAMEDATA ***************/
    {
        gameData.pBoard = &board;
        gameData.pTop10 = &top10;
        
        gameData.quitGame = 0;
        gameData.frog.x = 7;
        gameData.frog.y = 15;// coordenadas x e y de la rana.
    }

/************************** CREACIÓN DE TABLAS DE ESTADOS ****************************/
    state_t startMenuPlayGame[2+1]; //Cada arreglo de estado tiene tantos elementos state_t como posibles eventos que espera recibir +1 (END_TABLE).
    state_t startMenuViewScoreBoard[3+1];
    state_t startMenuQuit[2+1];
    state_t scoreBoard[1+1];
    state_t game[9+1];
    state_t pauseMenuContinue[2+1];
    state_t pauseMenuBackToStart[2+1];
    state_t saveScore1Char[3+1];
    state_t saveScore2Char[3+1];
    state_t saveScore3Char[3+1];
    
    /*********** INICIALIZACIÓN DE TABLAS DE ESTADOS ***********/
    {
        startMenuPlayGame[0].event.type = DOWN_EVENT; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        startMenuPlayGame[0].nextState = startMenuViewScoreBoard;
        startMenuPlayGame[0].actionRoutine = non_act_routine;
        startMenuPlayGame[0].stateID = 0;
        startMenuPlayGame[1].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuPlayGame[1].nextState = game;
        startMenuPlayGame[1].actionRoutine = start_game;
        startMenuPlayGame[1].stateID = 0;
        startMenuPlayGame[2].event.type = END_TABLE;
        startMenuPlayGame[2].nextState = startMenuPlayGame;
        startMenuPlayGame[2].actionRoutine = non_act_routine;
        startMenuPlayGame[1].stateID = 0;

        startMenuViewScoreBoard[0].event.type = UP_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuViewScoreBoard[0].nextState = startMenuPlayGame;
        startMenuViewScoreBoard[0].actionRoutine = non_act_routine;
        startMenuViewScoreBoard[0].stateID = 1;
        startMenuViewScoreBoard[1].event.type = DOWN_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuViewScoreBoard[1].nextState = startMenuQuit;
        startMenuViewScoreBoard[1].actionRoutine = non_act_routine;
        startMenuViewScoreBoard[1].stateID = 1;
        startMenuViewScoreBoard[2].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuViewScoreBoard[2].nextState = scoreBoard;
        startMenuViewScoreBoard[2].actionRoutine = non_act_routine;
        startMenuViewScoreBoard[2].stateID = 1;
        startMenuViewScoreBoard[3].event.type = END_TABLE;
        startMenuViewScoreBoard[3].nextState = startMenuViewScoreBoard;
        startMenuViewScoreBoard[3].actionRoutine = non_act_routine;
        startMenuViewScoreBoard[3].stateID = 1;

        startMenuQuit[0].event.type = UP_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuQuit[0].nextState = startMenuViewScoreBoard;
        startMenuQuit[0].actionRoutine = non_act_routine;
        startMenuQuit[0].stateID = 2;
        startMenuQuit[1].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuQuit[1].nextState = NULL;
        startMenuQuit[1].actionRoutine = end_game;
        startMenuQuit[1].stateID = 2;
        startMenuQuit[2].event.type = END_TABLE;
        startMenuQuit[2].nextState = startMenuQuit;
        startMenuQuit[2].actionRoutine = non_act_routine;
        startMenuQuit[2].stateID = 2;

        scoreBoard[0].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        scoreBoard[0].nextState = startMenuPlayGame;
        scoreBoard[0].actionRoutine = non_act_routine;
        scoreBoard[0].stateID = 3;
        scoreBoard[1].event.type = END_TABLE;
        scoreBoard[1].nextState = scoreBoard;
        scoreBoard[1].actionRoutine = non_act_routine;
        scoreBoard[1].stateID = 3;

        game[0].event.type = UP_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[0].nextState = game;
        game[0].actionRoutine = frog_up;
        game[0].stateID = 4;
        game[1].event.type = DOWN_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[1].nextState = game;
        game[1].actionRoutine = frog_down;
        game[1].stateID = 4;
        game[2].event.type = RIGHT_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[2].nextState = game;
        game[2].actionRoutine = frog_right;
        game[2].stateID = 4;
        game[3].event.type = LEFT_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[3].nextState = game;
        game[3].actionRoutine = frog_left;
        game[3].stateID = 4;
        game[4].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[4].nextState = pauseMenuContinue;
        game[4].actionRoutine = non_act_routine;
        game[4].stateID = 4;
        game[5].event.type = LEVEL_UP_EVENT;
        game[5].nextState = game;
        game[5].actionRoutine = cars_routine;
        game[5].stateID = 4;
        game[6].event.type = TIMER_EVENT;
        game[6].nextState = game;
        game[6].actionRoutine = cars_routine;
        game[6].stateID = 4;
        game[7].event.type = GAME_OVER_EVENT;
        game[7].nextState = saveScore1Char;
        game[7].actionRoutine = game_over;
        game[7].stateID = 4;
        game[8].event.type = FROG_HIT_EVENT;
        game[8].nextState = game;
        game[8].actionRoutine = frog_hit;
        game[8].stateID = 4;
        game[9].event.type = END_TABLE;
        game[9].nextState = game;
        game[9].actionRoutine = non_act_routine;
        game[9].stateID = 4;

        pauseMenuContinue[0].event.type = DOWN_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuContinue[0].nextState = pauseMenuBackToStart;
        pauseMenuContinue[0].actionRoutine = non_act_routine;
        pauseMenuContinue[0].stateID = 5;
        pauseMenuContinue[1].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuContinue[1].nextState = game;
        pauseMenuContinue[1].actionRoutine = non_act_routine;
        pauseMenuContinue[1].stateID = 5;
        pauseMenuContinue[2].event.type = END_TABLE;
        pauseMenuContinue[2].nextState = pauseMenuContinue;
        pauseMenuContinue[2].actionRoutine = non_act_routine;
        pauseMenuContinue[2].stateID = 5;
        
        pauseMenuBackToStart[0].event.type = UP_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuBackToStart[0].nextState = pauseMenuContinue;
        pauseMenuBackToStart[0].actionRoutine = non_act_routine;
        pauseMenuBackToStart[0].stateID = 6;
        pauseMenuBackToStart[1].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuBackToStart[1].nextState = startMenuPlayGame;
        pauseMenuBackToStart[1].actionRoutine = non_act_routine;
        pauseMenuBackToStart[1].stateID = 6;
        pauseMenuBackToStart[2].event.type = END_TABLE;
        pauseMenuBackToStart[2].nextState = pauseMenuBackToStart;
        pauseMenuBackToStart[2].actionRoutine = non_act_routine;
        pauseMenuBackToStart[2].stateID = 6;

        saveScore1Char[0].event.type = UP_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore1Char[0].nextState = saveScore1Char;
        saveScore1Char[0].actionRoutine = fst_letter_up;
        saveScore1Char[0].stateID = 7;
        saveScore1Char[1].event.type = DOWN_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore1Char[1].nextState = saveScore1Char;
        saveScore1Char[1].actionRoutine = fst_letter_down;
        saveScore1Char[1].stateID = 7;
        saveScore1Char[2].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore1Char[2].nextState = saveScore2Char;
        saveScore1Char[2].actionRoutine = non_act_routine;
        saveScore1Char[2].stateID = 7;
        saveScore1Char[3].event.type = END_TABLE;
        saveScore1Char[3].nextState = saveScore1Char;
        saveScore1Char[3].actionRoutine = non_act_routine;
        saveScore1Char[3].stateID = 7;

        saveScore2Char[0].event.type = UP_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore2Char[0].nextState = saveScore2Char;
        saveScore2Char[0].actionRoutine = scd_letter_up;
        saveScore2Char[0].stateID = 8;
        saveScore2Char[1].event.type = DOWN_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore2Char[1].nextState = saveScore2Char;
        saveScore2Char[1].actionRoutine = scd_letter_down;
        saveScore2Char[1].stateID = 8;
        saveScore2Char[2].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore2Char[2].nextState = saveScore3Char;
        saveScore2Char[2].actionRoutine = non_act_routine;
        saveScore2Char[2].stateID = 8;
        saveScore2Char[3].event.type = END_TABLE;
        saveScore2Char[3].nextState = saveScore2Char;
        saveScore2Char[3].actionRoutine = non_act_routine;
        saveScore2Char[3].stateID = 8;

        saveScore3Char[0].event.type = UP_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore3Char[0].nextState = saveScore3Char;
        saveScore3Char[0].actionRoutine = trd_letter_up;
        saveScore3Char[0].stateID = 9;
        saveScore3Char[1].event.type = DOWN_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore3Char[1].nextState = saveScore3Char;
        saveScore3Char[1].actionRoutine = trd_letter_down;
        saveScore3Char[1].stateID = 9;
        saveScore3Char[2].event.type = ENTER_EVENT; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore3Char[2].nextState = startMenuPlayGame;
        saveScore3Char[2].actionRoutine = non_act_routine;
        saveScore3Char[2].stateID = 9;
        saveScore3Char[3].event.type = END_TABLE;
        saveScore3Char[3].nextState = saveScore3Char;
        saveScore3Char[3].actionRoutine = non_act_routine;
        saveScore3Char[3].stateID = 9;
    }
/************************************************************************************/
    pGameData->currentState = &startMenuPlayGame[0];
    
    pthread_t terminalDisplay, inputThread, timerThread;
    pthread_create(&terminalDisplay, NULL, &terminal_display, pGameData);
    pthread_create(&inputThread, NULL, &input_thread, pGameData);
    pthread_create(&timerThread, NULL, &timer_thread, pGameData);
    
    while(!gameData.quitGame)
    {
        if(gameData.lives <= 0) // Si se quedó sin vidas, game over.
        {
            event.type = ALLEGRO_EVENT_USER;
            al_emit_user_event(&gameData.lives, &event, NULL); // ES POSIBLE QUE ESTE EVENTO NO ESTE SIENDO ENVIADO A LA COLA, SINO DIRECTAMENTE A LA VARIABLE event, CONTROLAR ESO.
            pGameData->currentState = fsm_handler(pGameData->currentState, event, pGameData);
        }
        else if((*gameData.pBoard)[gameData.frog.y][gameData.frog.x]) // Si en la posición de la rana hay un 1, hay un choque.
        { 
            event.type = ALLEGRO_EVENT_USER;    
            al_emit_user_event(&gameData.frogHit, &event, NULL); // ES POSIBLE QUE ESTE EVENTO NO ESTE SIENDO ENVIADO A LA COLA, SINO DIRECTAMENTE A LA VARIABLE event, CONTROLAR ESO.
            pGameData->currentState = fsm_handler(pGameData->currentState, event, pGameData);
        }
        else if(gameData.frog.y == 0 && (gameData.frog.x == 2 || gameData.frog.x == 6 || gameData.frog.x == 9 || gameData.frog.x == 13)) // Si la rana llegó a alguno de los 4 lugares de llegada, se sube de nivel.
        {
            event.type = ALLEGRO_EVENT_USER;
            gameData.levelUp.__pad[0] = 1; // Se avisa que se va a subir de nivel.
            al_emit_user_event(&gameData.levelUp, &event, NULL); // ES POSIBLE QUE ESTE EVENTO NO ESTE SIENDO ENVIADO A LA COLA, SINO DIRECTAMENTE A LA VARIABLE event, CONTROLAR ESO.
            pGameData->currentState = fsm_handler(pGameData->currentState, event, pGameData);
        }
        
        else if(al_get_next_event(eventQueue, &event))
        {
            pGameData->currentState = fsm_handler(pGameData->currentState, event, pGameData);
        }
    }
    
    printf("YOU EXITED THE GAME\n"); // CAMBIAR ESTO A UN MENSAJE DE SALIDA DEL JUEGO O LO QUE SEA.
    pthread_join(terminalDisplay, NULL);
    al_destroy_event_queue(eventQueue);
    al_destroy_timer(carsTimer);
    al_uninstall_keyboard();
    
    return (EXIT_SUCCESS);
}




/******************************* INIT_BOARD FUNCTION *********************************/
void init_board(boolean_t board[][BOARD_SIZE])
{
    int i, j;
    
    for(i = 0; i < BOARD_SIZE; i++)
    {
        for(j = 0; j < BOARD_SIZE; j++)
        {
            board[j][i] = 0;
        }
    }
    for(i = 0; i < BOARD_SIZE; i++)
    {
        board[i][0] = 1;
    }
    
    board[2][0] = 0;
    board[6][0] = 0;
    board[9][0] = 0;
    board[13][0] = 0;
    
    board[15][14] = 1;
    board[14][14] = 1;
    board[7][14] = 1;
    board[6][14] = 1;
    
    board[1][13] = 1;
    board[5][13] = 1;
    board[9][13] = 1;
    board[13][13] = 1;

    board[1][12] = 1;
    board[2][12] = 1;
    board[9][12] = 1;
    board[10][12] = 1;

    board[3][11] = 1;
    board[4][11] = 1;
    board[5][11] = 1;
    board[11][11] = 1;
    board[12][11] = 1;
    board[13][11] = 1;

    board[1][10] = 1;
    board[5][10] = 1;
    board[9][10] = 1;
    board[13][10] = 1;

    board[0][9] = 1;
    board[1][9] = 1;
    board[2][9] = 1;
    board[8][9] = 1;
    board[9][9] = 1;
    board[10][9] = 1;

    board[0][7] = 1;
    board[1][7] = 1;
    board[2][7] = 1;
    board[3][7] = 1;
    board[8][7] = 1;
    board[9][7] = 1;
    board[10][7] = 1;
    board[11][7] = 1;

    board[1][6] = 1;
    board[2][6] = 1;
    board[9][6] = 1;
    board[10][6] = 1;

    board[0][5] = 1;
    board[1][5] = 1;
    board[2][5] = 1;
    board[4][5] = 1;
    board[5][5] = 1;
    board[6][5] = 1;
    board[8][5] = 1;
    board[9][5] = 1;
    board[10][5] = 1;
    board[12][5] = 1;
    board[13][5] = 1;
    board[14][5] = 1;

    board[2][4] = 1;
    board[3][4] = 1;
    board[4][4] = 1;
    board[5][4] = 1;
    board[10][4] = 1;
    board[11][4] = 1;
    board[12][4] = 1;
    board[13][4] = 1;

    board[3][3] = 1;
    board[4][3] = 1;
    board[11][3] = 1;
    board[12][3] = 1;

    board[1][2] = 1;
    board[2][2] = 1;
    board[3][2] = 1;
    board[5][2] = 1;
    board[6][2] = 1;
    board[7][2] = 1;
    board[9][2] = 1;
    board[10][2] = 1;
    board[11][2] = 1;
    board[13][2] = 1;
    board[14][2] = 1;
    board[15][2] = 1;

    board[4][1] = 1;
    board[5][1] = 1;
    board[6][1] = 1;
    board[7][1] = 1;
    board[12][1] = 1;
    board[13][1] = 1;
    board[14][1] = 1;
    board[15][1] = 1;
}



/****************************** FSM_HANDLER FUNCTION *********************************/
state_t* fsm_handler(state_t *currentState, ALLEGRO_EVENT newEvent, void *pActRoutineData)
{
    if(newEvent.type == ALLEGRO_EVENT_KEY_CHAR) // CAMBIAR ESTO AL TIPO DE EVENTO QUE SE USE PARA EL JOYSTICK (RECORDAR QUE SE PODRÍA DEFINIR UN ALLEGRO_EVENT_USER_JOYSTICK PROPIO, PARA DIFERENCIARLO DE LOS USER EVENTS QUE YO HICE)
    {
        while(currentState->event.keyboard.unichar != newEvent.keyboard.unichar && currentState->event.user.data1 != END_TABLE)
        {
            currentState++;
        }

        (*currentState->actionRoutine)(pActRoutineData);
        currentState = currentState->nextState;
        return currentState;
    }
    else if(newEvent.type == ALLEGRO_EVENT_USER)
    {
        while(currentState->event.user.source != newEvent.user.source && currentState->event.user.data1 != END_TABLE)
        {
            currentState++;
        }

        (*currentState->actionRoutine)(pActRoutineData);
        currentState = currentState->nextState;
        return currentState;
    }
    else if(newEvent.type == ALLEGRO_EVENT_TIMER)
    {
        while(currentState->event.timer.source != newEvent.timer.source && currentState->event.user.data1 != END_TABLE)
        {
            currentState++;
        }

        (*currentState->actionRoutine)(pActRoutineData);
        currentState = currentState->nextState;
        return currentState;
    }
}
