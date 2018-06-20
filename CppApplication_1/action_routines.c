#include "fsm.h"

void non_act_routine(void *pArg)
{
    
}

void cars_routine(void *pArg)
{
    gameData_t *pData = pArg;
    static int dividersMax[BOARD_SIZE] = {0, 15, 20, 8, 15, 20, 8, 15, 0, 12, 7, 12, 10, 7, 10, 0}; // Cuando se suba de nivel, alguno de estos máximos se decrementará para hacer que el ciclo de avance de el carril correspondiente sea más rápido.
    static int dividers[BOARD_SIZE] = {0, 15, 20, 8, 15, 20, 8, 15, 0, 12, 7, 12, 10, 7, 10, 0}; // Ante un evento de timer, se decrementa el divider de cada carril, logrando así que cada carril tenga su ciclo de timer, cuando el divider llega a 0.
    boolean_t ways[BOARD_SIZE] = {0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0};
    int row = 0;
    srand(time(NULL));
    
    if(pData->levelUp) // Si se tiene que subir de nivel, se efectua un cambio en el máximo de los divisores.
    {
        pData->levelUp = !(pData->levelUp); // Se evita que se suba de nivel nuevamente.
        while((dividersMax[row] <= 1)) 
        {
            row = rand()%16; // Se selecciona al azar uno de los carriles a aumetar su velocidad. No se aumenta la volocidad de los que ya van a la velocidad del clock.
        }
        dividersMax[row]--;
        
        pData->frog.x = 8;
        pData->frog.y = 15; // Se reinicia la posición de la rana.
    }
    else // En cambio, si la rutina fue llamada por evento de timer, se realiza el decremento de los dividers.
    {
        for(row = 0; row < BOARD_SIZE; row++)
        {
            if(!dividersMax[row]) // Si no se está en una fila con divider maximo 0...
            {
                dividers[row]--;
                if(!dividers[row]) // Si se cumplió el ciclo, se mueven los autos.
                {
                    shift_handler(*(pData->pBoard)[row], ways[row]);
                    dividers[row] = dividersMax[row]; // Se resetea el ciclo con el maximo de cada divider.
                }
            }
        }
    }
}

void frog_up(void *pArg)
{
    gameData_t *pData = pArg;
    if(pData->frog.y) // Si no se está en y=0, se sube.
    {
        (pData->frog.y)--;
    }
}

void frog_down(void *pArg)
{
    gameData_t *pData = pArg;
    if(pData->frog.y < 15) // Si no se está en y=15, se baja.
    {
        (pData->frog.y)++;
    }
}

void frog_left(void *pArg)
{
    gameData_t *pData = pArg;
    if(pData->frog.x) // Si no se está en x=0, se va a la izquierda.
    {
        (pData->frog.x)--;
    }
}

void frog_right(void *pArg)
{
    gameData_t *pData = pArg;
    if(pData->frog.x < 15) // Si no se está en x=15, se va a la derecha.
    {
        (pData->frog.x)++;
    }
}

void frog_hit(void *pArg)
{
    gameData_t *pData = pArg;
    pData->lives--;
    pData->frog.x = 8;
    pData->frog.y = 15; // Se reinicia la posición de la rana.
}

void start_game(void *pArg)
{
    gameData_t *pData = pArg;
    pData->lives = LIVES;
    int letter;
    for(letter = 0; letter < 3; letter++)
    {
        pData->player[letter] = 'A';
    }
}

void fst_letter_up(void *pArg)
{
    letter_up(pArg, 0);
}

void scd_letter_up(void *pArg)
{
    letter_up(pArg, 1);
}

void trd_letter_up(void *pArg)
{
    letter_up(pArg, 2);
}

void fst_letter_down(void *pArg)
{
    letter_down(pArg, 0);
}

void scd_letter_down(void *pArg)
{
    letter_down(pArg, 1);
}

void trd_letter_down(void *pArg)
{
    letter_down(pArg, 2);
}

void end_game(void *pArg)
{
    gameData_t *pData = pArg;
    pData->quitGame = 1;
}

void game_over(void *pArg)
{
    printf("GAME OVER."); // CAMBIAR ESTO POR ALGO MAS COPADO.
}





/******************************************* FUNCIONES AUXILIARES **************************************************/
void shift_handler(boolean_t row[BOARD_SIZE], boolean_t way)
{
    if(way) // Si way es 1, se gira a la derecha.
    {
        shift_right_row(row);
    }
    else // Se gira a izquierda.
    {
        shift_left_row(row);
    }
}

void shift_right_row(boolean_t row[BOARD_SIZE])
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

void letter_up(void *pArg, int letter)
{
    gameData_t *pData = pArg;
    if(pData->player[letter] > 'A')
    {
        pData->player[letter]++;
    }
}

void letter_down(void *pArg, int letter)
{
    gameData_t *pData = pArg;
    if(pData->player[letter] < 'Z')
    {
        pData->player[letter]--;
    }
}