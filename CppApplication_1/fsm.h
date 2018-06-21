/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.h)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

#ifndef _FSM_H_
#define _FSM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/events.h>
#include <time.h>
#include <stdlib.h>



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BOARD_SIZE 16
#define LIVES 3
#define END_TABLE 0


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum events {
    RIGHT_EVENT = 1024,
    UP_EVENT,
    DOWN_EVENT,
    LEFT_EVENT,
    GAME_OVER_EVENT,
    LEVEL_UP_EVENT,
    FROG_HIT_EVENT,
    ENTER_EVENT,
    TIMER_EVENT,
    PAUSE_EVENT,
};

typedef char boolean_t;
typedef struct
{
    boolean_t flag;
    int type;
    boolean_t timerFlag;
}event_t;
typedef struct
{
    char player[3];
    uint score;
} scorer_t;
typedef struct STATE
{
    event_t event;
    struct STATE *nextState;
    void (*actionRoutine)(void *);
    int stateID;
} state_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
}frog_t;

typedef struct
{
    int lives;
    boolean_t levelUp;
    boolean_t frogHit;
    boolean_t quitGame;
    boolean_t (*pBoard)[16][16];
    frog_t frog;
    char player[3];
    uint score;
    scorer_t (*pTop10)[10];
    state_t *currentState;
    event_t event;
}gameData_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void non_act_routine(void *pArg);
void cars_routine(void *pArg);
void shift_right_row(boolean_t row[BOARD_SIZE][BOARD_SIZE], int row_num);
void shift_left_row(boolean_t row[BOARD_SIZE][BOARD_SIZE], int row_num);
void shift_handler(boolean_t row[BOARD_SIZE][BOARD_SIZE], boolean_t way, int row_num);
void frog_up(void *pArg);
void frog_down(void *pArg);
void frog_left(void *pArg);
void frog_right(void *pArg);
void frog_hit(void *pArg);
void start_game(void *pArg);
void letter_up(void *pArg, int letter);
void letter_down(void *pArg, int letter);
void fst_letter_up(void *pArg);
void scd_letter_up(void *pArg);
void trd_letter_up(void *pArg);
void fst_letter_down(void *pArg);
void scd_letter_down(void *pArg);
void trd_letter_down(void *pArg);
void end_game(void *pArg);
void game_over(void *pArg);


/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
// +ej: char lcd_goto (int fil, int col);+


/*******************************************************************************
 ******************************************************************************/

#endif // _FSM_H_