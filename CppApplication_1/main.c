#include "main.h"

int main(int argc, char* argv[]) 
{
    ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
    if(eventQueue)
    {
        fprintf(stderr, "Error loading event queue.");
        return -1;
    }
    ALLEGRO_EVENT event;
    
    ALLEGRO_TIMER *carsTimer = al_create_timer(1.0/FPS); // Timer para actualizar la posición de los autos.
    if(carsTimer)
    {
        fprintf(stderr, "Error loading carsTimer.");
        al_destroy_event_queue(eventQueue);
        return -1;
    }
    al_register_event_source(eventQueue, al_get_timer_event_source(carsTimer));
    
    if(al_install_keyboard()) // USO EL TECLADO EN LUGAR DEL JOYSTICK, ESTO DEBE CAMBIARSE.
    {
        fprintf(stderr, "Error installing keyboard.");
        al_destroy_event_queue(eventQueue);
        al_destroy_timer(carsTimer);
        return -1;
    }
    
    boolean_t board[BOARD_SIZE][BOARD_SIZE];
    init_board(board);
    scorer_t top10[10]; // Arreglo de top 10 de puntajes. 
    // ESTE ARREGLO DEBE SER LLENADO AL INICIO DEL PROGRAMA CON EL TOP10 GUARDADO EN UN ARCHIVO Y VACIADO EN ESE MISMO ARCHIVO AL FINALIZAR.
    // EL ARREGLO ESTÁ PARA FACILITAR OPERACIONES DE ORDENAMIENTO DEL TOP10, YA QUE DE ESTE MODO SE PUEDE USAR QSORT, POR EJEMPLO.
    // UN JUGADOR ENTRARÍA EN EL TOP10 SI SU PUNTAJE SUPERA AL ÚLTIMO DE LA LISTA ORDENADA, POSTERIORMENTE SE REORDENA CON QSORT Y EL JUGADOR QUEDA EN LA POSICIÓN CORRESPONDIENTE.
    
    gameData_t gameData; // Se crea este objeto que contiene toda la información necesaria para manejar el juego, de modo que las rutinas de acción puedan recibir un puntero a este objeto.
    gameData_t *pGameData;
    /************* INICIALIZACIÓN DE GAMEDATA ***************/
    {
        al_init_user_event_source(&gameData.lives); // Creación de un tipo de evento de software para cuando se acaban las vidas.
        al_register_event_source(eventQueue, &gameData.lives);
        
        al_init_user_event_source(&gameData.levelUp); // Creación de un tipo de evento de software para cuando se llega al otro lado del cruce y se sube de nivel.
        al_register_event_source(eventQueue, &gameData.levelUp);
        
        al_init_user_event_source(&gameData.frogHit); // Creación de un tipo de evento de software para cuando la rana pierda una vida.
        al_register_event_source(eventQueue, &gameData.frogHit);
        
        gameData.pBoard = &board;
        gameData.pTop10 = &top10;
        
        gameData.quitGame = 0;
        gameData.frog = {7, 15}; // coordenadas x e y de la rana.
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
        startMenuPlayGame[0].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        startMenuPlayGame[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuPlayGame[0].nextState = startMenuViewScoreBoard;
        startMenuPlayGame[0].actionRoutine = non_act_routine(NULL);
        startMenuPlayGame[0].stateID = 0;
        startMenuPlayGame[1].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        startMenuPlayGame[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuPlayGame[1].nextState = game;
        startMenuPlayGame[1].actionRoutine = start_game(pGameData);
        startMenuPlayGame[1].stateID = 0;
        startMenuPlayGame[2].event.user.data1 = END_TABLE;
        startMenuPlayGame[2].event.type = ALLEGRO_EVENT_USER;
        startMenuPlayGame[2].nextState = startMenuPlayGame;
        startMenuPlayGame[2].actionRoutine = non_act_routine(NULL);
        startMenuPlayGame[1].stateID = 0;

        startMenuViewScoreBoard[0].event.keyboard.unichar = KEYUP_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ARRIBA DEL JOYSTICK.
        startMenuViewScoreBoard[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuViewScoreBoard[0].nextState = startMenuPlayGame;
        startMenuViewScoreBoard[0].actionRoutine = non_act_routine(NULL);
        startMenuViewScoreBoard[0].stateID = 1;
        startMenuViewScoreBoard[1].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        startMenuViewScoreBoard[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuViewScoreBoard[1].nextState = startMenuQuit;
        startMenuViewScoreBoard[1].actionRoutine = non_act_routine(NULL);
        startMenuViewScoreBoard[1].stateID = 1;
        startMenuViewScoreBoard[2].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        startMenuViewScoreBoard[2].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuViewScoreBoard[2].nextState = scoreBoard;
        startMenuViewScoreBoard[2].actionRoutine = non_act_routine(NULL);
        startMenuViewScoreBoard[2].stateID = 1;
        startMenuViewScoreBoard[3].event.user.data1 = END_TABLE;
        startMenuViewScoreBoard[3].event.type = ALLEGRO_EVENT_USER;
        startMenuViewScoreBoard[3].nextState = startMenuViewScoreBoard;
        startMenuViewScoreBoard[3].actionRoutine = non_act_routine(NULL);
        startMenuViewScoreBoard[3].stateID = 1;

        startMenuQuit[0].event.keyboard.unichar = KEYUP_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ARRIBA DEL JOYSTICK.
        startMenuQuit[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuQuit[0].nextState = startMenuViewScoreBoard;
        startMenuQuit[0].actionRoutine = non_act_routine(NULL);
        startMenuQuit[0].stateID = 2;
        startMenuQuit[1].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        startMenuQuit[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        startMenuQuit[1].nextState = NULL;
        startMenuQuit[1].actionRoutine = end_game(pGameData);
        startMenuQuit[1].stateID = 2;
        startMenuQuit[2].event.user.data1 = END_TABLE;
        startMenuQuit[2].event.type = ALLEGRO_EVENT_USER;
        startMenuQuit[2].nextState = startMenuQuit;
        startMenuQuit[2].actionRoutine = non_act_routine(NULL);
        startMenuQuit[2].stateID = 2;

        scoreBoard[0].event.keyboard.unichar = ENTER_ASCII;
        scoreBoard[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        scoreBoard[0].nextState = startMenuPlayGame;
        scoreBoard[0].actionRoutine = non_act_routine(NULL);
        scoreBoard[0].stateID = 3;
        scoreBoard[1].event.user.data1 = END_TABLE;
        scoreBoard[1].event.type = ALLEGRO_EVENT_USER;
        scoreBoard[1].nextState = scoreBoard;
        scoreBoard[1].actionRoutine = non_act_routine(NULL);
        scoreBoard[1].stateID = 3;


        game[0].event.keyboard.unichar = KEYUP_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ARRIBA DEL JOYSTICK.
        game[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[0].nextState = game;
        game[0].actionRoutine = frog_up(pGameData);
        game[0].stateID = 4;
        game[1].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        game[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[1].nextState = game;
        game[1].actionRoutine = frog_down(pGameData);
        game[1].stateID = 4;
        game[2].event.keyboard.unichar = KEYRIGHT_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA DERECHA DEL JOYSTICK.
        game[2].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[2].nextState = game;
        game[2].actionRoutine = frog_right(pGameData);
        game[2].stateID = 4;
        game[3].event.keyboard.unichar = KEYLEFT_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA IZQUIERDA DEL JOYSTICK.
        game[3].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[3].nextState = game;
        game[3].actionRoutine = frog_left(pGameData);
        game[3].stateID = 4;
        game[4].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        game[4].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        game[4].nextState = pauseMenuContinue;
        game[4].actionRoutine = non_act_routine(NULL);
        game[4].stateID = 4;
        game[5].event.timer.source = carsTimer;
        game[5].event.type = ALLEGRO_EVENT_TIMER;
        game[5].nextState = game;
        game[5].actionRoutine = cars_routine(pGameData);
        game[5].stateID = 4;
        game[6].event.user.source = gameData.levelUp;
        game[6].event.type = ALLEGRO_EVENT_USER;
        game[6].nextState = game;
        game[6].actionRoutine = cars_routine(pGameData);
        game[6].stateID = 4;
        game[7].event.user.source = gameData.lives;
        game[7].event.type = ALLEGRO_EVENT_USER;
        game[7].nextState = saveScore1Char;
        game[7].actionRoutine = game_over(NULL);
        game[7].stateID = 4;
        game[8].event.user.source = gameData.frogHit;
        game[8].event.type = ALLEGRO_EVENT_USER;
        game[8].nextState = game;
        game[8].actionRoutine = frog_hit(pGameData);
        game[8].stateID = 4;
        game[9].event.user.data1 = END_TABLE;
        game[9].event.type = ALLEGRO_EVENT_USER;
        game[9].nextState = game;
        game[9].actionRoutine = non_act_routine(NULL);
        game[9].stateID = 4;

        pauseMenuContinue[0].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        pauseMenuContinue[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuContinue[0].nextState = pauseMenuBackToStart;
        pauseMenuContinue[0].actionRoutine = non_act_routine(NULL);
        pauseMenuContinue[0].stateID = 5;
        pauseMenuContinue[1].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        pauseMenuContinue[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuContinue[1].nextState = game;
        pauseMenuContinue[1].actionRoutine = non_act_routine(NULL);
        pauseMenuContinue[1].stateID = 5;
        pauseMenuContinue[2].event.user.data1 = END_TABLE;
        pauseMenuContinue[2].event.type = ALLEGRO_EVENT_USER;
        pauseMenuContinue[2].nextState = pauseMenuContinue;
        pauseMenuContinue[2].actionRoutine = non_act_routine(NULL);
        pauseMenuContinue[2].stateID = 5;
        
        pauseMenuBackToStart[0].event.keyboard.unichar = KEYUP_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        pauseMenuBackToStart[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuBackToStart[0].nextState = pauseMenuContinue;
        pauseMenuBackToStart[0].actionRoutine = non_act_routine(NULL);
        pauseMenuBackToStart[0].stateID = 6;
        pauseMenuBackToStart[1].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        pauseMenuBackToStart[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        pauseMenuBackToStart[1].nextState = startMenuPlayGame;
        pauseMenuBackToStart[1].actionRoutine = non_act_routine(NULL);
        pauseMenuBackToStart[1].stateID = 6;
        pauseMenuBackToStart[2].event.user.data1 = END_TABLE;
        pauseMenuBackToStart[2].event.type = ALLEGRO_EVENT_USER;
        pauseMenuBackToStart[2].nextState = pauseMenuBackToStart;
        pauseMenuBackToStart[2].actionRoutine = non_act_routine(NULL);
        pauseMenuBackToStart[2].stateID = 6;

        saveScore1Char[0].event.keyboard.unichar = KEYUP_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ARRIBA DEL JOYSTICK.
        saveScore1Char[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore1Char[0].nextState = saveScore1Char;
        saveScore1Char[0].actionRoutine = fst_letter_up(pGameData);
        saveScore1Char[0].stateID = 7;
        saveScore1Char[1].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        saveScore1Char[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore1Char[1].nextState = saveScore1Char;
        saveScore1Char[1].actionRoutine = fst_letter_down(pGameData);
        saveScore1Char[1].stateID = 7;
        saveScore1Char[2].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        saveScore1Char[2].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore1Char[2].nextState = saveScore2Char;
        saveScore1Char[2].actionRoutine = non_act_routine(NULL);
        saveScore1Char[2].stateID = 7;
        saveScore1Char[3].event.user.data1 = END_TABLE;
        saveScore1Char[3].event.type = ALLEGRO_EVENT_USER;
        saveScore1Char[3].nextState = saveScore1Char;
        saveScore1Char[3].actionRoutine = non_act_routine(NULL);
        saveScore1Char[3].stateID = 7;

        saveScore2Char[0].event.keyboard.unichar = KEYUP_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ARRIBA DEL JOYSTICK.
        saveScore2Char[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore2Char[0].nextState = saveScore2Char;
        saveScore2Char[0].actionRoutine = scd_letter_up(pGameData);
        saveScore2Char[0].stateID = 8;
        saveScore2Char[1].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        saveScore2Char[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore2Char[1].nextState = saveScore2Char;
        saveScore2Char[1].actionRoutine = scd_letter_down(pGameData);
        saveScore2Char[1].stateID = 8;
        saveScore2Char[2].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        saveScore2Char[2].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore2Char[2].nextState = saveScore3Char;
        saveScore2Char[2].actionRoutine = non_act_routine(NULL);
        saveScore2Char[2].stateID = 8;
        saveScore2Char[3].event.user.data1 = END_TABLE;
        saveScore2Char[3].event.type = ALLEGRO_EVENT_USER;
        saveScore2Char[3].nextState = saveScore2Char;
        saveScore2Char[3].actionRoutine = non_act_routine(NULL);
        saveScore2Char[3].stateID = 8;

        saveScore3Char[0].event.keyboard.unichar = KEYUP_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ARRIBA DEL JOYSTICK.
        saveScore3Char[0].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore3Char[0].nextState = saveScore3Char;
        saveScore3Char[0].actionRoutine = trd_letter_up(pGameData);
        saveScore3Char[0].stateID = 9;
        saveScore3Char[1].event.keyboard.unichar = KEYDOWN_ASCII; // CAMBIAR ESTO A UN EVENTO PALANCA ABAJO DEL JOYSTICK.
        saveScore3Char[1].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore3Char[1].nextState = saveScore3Char;
        saveScore3Char[1].actionRoutine = trd_letter_down(pGameData);
        saveScore3Char[1].stateID = 9;
        saveScore3Char[2].event.keyboard.unichar = ENTER_ASCII; // CAMBIAR ESTO A UN EVENTO PRESS JOYSTICK.
        saveScore3Char[2].event.type = ALLEGRO_EVENT_KEY_CHAR; // CAMBIAR ESTO POR TIPO DE EVENTO ALLEGRO_EVENT_USER (O EL QUE CORRESPONDA).
        saveScore3Char[2].nextState = startMenuPlayGame;
        saveScore3Char[2].actionRoutine = non_act_routine(NULL);
        saveScore3Char[2].stateID = 9;
        saveScore3Char[3].event.user.data1 = END_TABLE;
        saveScore3Char[3].event.type = ALLEGRO_EVENT_USER;
        saveScore3Char[3].nextState = saveScore3Char;
        saveScore3Char[3].actionRoutine = non_act_routine(NULL);
        saveScore3Char[3].stateID = 9;
    }
/************************************************************************************/
    pGameData.currentState = startMenuPlayGame;
    al_start_timer(carsTimer);
    
    while(!gameData.quitGame)
    {
        if(gameData.lives.__pad[0] <= 0) // Si se quedó sin vidas, game over.
        {
            event.type = ALLEGRO_EVENT_USER;
            al_emit_user_event(gameData.lives, &event, NULL); // ES POSIBLE QUE ESTE EVENTO NO ESTE SIENDO ENVIADO A LA COLA, SINO DIRECTAMENTE A LA VARIABLE event, CONTROLAR ESO.
        }
        else if(gameData.pBoard[gameData.frog[1]][gameData.frog[0]]) // Si en la posición de la rana hay un 1, hay un choque.
        { 
            event.type = ALLEGRO_EVENT_USER;    
            al_emit_user_event(gameData.frogHit, &event, NULL); // ES POSIBLE QUE ESTE EVENTO NO ESTE SIENDO ENVIADO A LA COLA, SINO DIRECTAMENTE A LA VARIABLE event, CONTROLAR ESO.
        }
        else if(gameData.frog[1] == 0 && (gameData.frog[0] == 2 || gameData.frog[0] == 6 || gameData.frog[0] == 9 || gameData.frog[0] = 13)) // Si la rana llegó a alguno de los 4 lugares de llegada, se sube de nivel.
        {
            event.type = ALLEGRO_EVENT_USER;
            gameData.levelUp.__pad[0] = 1; // Se avisa que se va a subir de nivel.
            al_emit_user_event(gameData.levelUp, &event, NULL); // ES POSIBLE QUE ESTE EVENTO NO ESTE SIENDO ENVIADO A LA COLA, SINO DIRECTAMENTE A LA VARIABLE event, CONTROLAR ESO.
        }
        
        if(al_get_next_event(eventQueue, &event))
        {
            pGameData.currentState = fsm_handler(pGameData.currentState, event, pGameData);
        }
    }
    
    printf("YOU EXITED THE GAME\n"); // CAMBIAR ESTO A UN MENSAJE DE SALIDA DEL JUEGO O LO QUE SEA.
    
    
    return (EXIT_SUCCESS);
}




/******************************* INIT_BOARD FUNCTION *********************************/
void init_board(boolean_t board[BOARD_SIZE][])
{
    int i, j;
    
    for(i = 0; i < BOARD_SIZE; i++)
    {
        for(j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = 0;
        }
    }
    for(i = 0; i < BOARD_SIZE; i++)
    {
        board[0][i] = 1;
    }
    
    board[0][2] = 0;
    board[0][6] = 0;
    board[0][9] = 0;
    board[0][13] = 0;
    
    board[14][15] = 1;
    board[14][14] = 1;
    board[14][7] = 1;
    board[14][6] = 1;
    
    board[13][1] = 1;
    board[13][5] = 1;
    board[13][9] = 1;
    board[13][13] = 1;

    board[12][1] = 1;
    board[12][2] = 1;
    board[12][9] = 1;
    board[12][10] = 1;

    board[11][3] = 1;
    board[11][4] = 1;
    board[11][5] = 1;
    board[11][11] = 1;
    board[11][12] = 1;
    board[11][13] = 1;

    board[10][1] = 1;
    board[10][5] = 1;
    board[10][9] = 1;
    board[10][13] = 1;

    board[9][0] = 1;
    board[9][1] = 1;
    board[9][2] = 1;
    board[9][8] = 1;
    board[9][9] = 1;
    board[9][10] = 1;

    board[7][0] = 1;
    board[7][1] = 1;
    board[7][2] = 1;
    board[7][3] = 1;
    board[7][8] = 1;
    board[7][9] = 1;
    board[7][10] = 1;
    board[7][11] = 1;

    board[6][1] = 1;
    board[6][2] = 1;
    board[6][9] = 1;
    board[6][10] = 1;

    board[5][0] = 1;
    board[5][1] = 1;
    board[5][2] = 1;
    board[5][4] = 1;
    board[5][5] = 1;
    board[5][6] = 1;
    board[5][8] = 1;
    board[5][9] = 1;
    board[5][10] = 1;
    board[5][12] = 1;
    board[5][13] = 1;
    board[5][14] = 1;

    board[4][2] = 1;
    board[4][3] = 1;
    board[4][4] = 1;
    board[4][5] = 1;
    board[4][10] = 1;
    board[4][11] = 1;
    board[4][12] = 1;
    board[4][13] = 1;

    board[3][3] = 1;
    board[3][4] = 1;
    board[3][11] = 1;
    board[3][12] = 1;

    board[2][1] = 1;
    board[2][2] = 1;
    board[2][3] = 1;
    board[2][5] = 1;
    board[2][6] = 1;
    board[2][7] = 1;
    board[2][9] = 1;
    board[2][10] = 1;
    board[2][11] = 1;
    board[2][13] = 1;
    board[2][14] = 1;
    board[2][15] = 1;

    board[1][4] = 1;
    board[1][5] = 1;
    board[1][6] = 1;
    board[1][7] = 1;
    board[1][12] = 1;
    board[1][13] = 1;
    board[1][14] = 1;
    board[1][15] = 1;
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
