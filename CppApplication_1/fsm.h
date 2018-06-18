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


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct STATE
{
    ALLEGRO_EVENT event;
    struct STATE *nextState;
    void (*actionRoutine)(void *);
    int stateID;
} state_t;
typedef struct
{
    ALLEGRO_EVENT_SOURCE lives;
    ALLEGRO_EVENT_SOURCE levelUp;
    ALLEGRO_EVENT_SOURCE frogHit;
    boolean_t quitGame;
    boolean_t (*pBoard)[16][16];
    int frog[2];
    char player[3];
    uint score;
    scorer_t (*pTop10)[10];
    state_t *currentState;
}gameData_t;
typedef char boolean_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void non_act_routine(void *pArg);
void cars_routine(void *pArg);
void shift_right_row(boolean_t row[16]);
void shift_left_row(boolean_t row[16]);
void shift_handler(boolean_t row[BOARD_SIZE], boolean_t way);
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