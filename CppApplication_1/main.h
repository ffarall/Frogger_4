/***************************************************************************//**
  @file     +Nombre del archivo (ej: template.h)+
  @brief    +Descripcion del archivo+
  @author   +Nombre del autor (ej: Salvador Allende)+
 ******************************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "fsm.h"
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <allegro5/events.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define END_TABLE 0
#define KEYDOWN_ASCII 40
#define KEYUP_ASCII 38
#define KEYLEFT_ASCII 37
#define KEYRIGHT_ASCII 39
#define ENTER_ASCII 13
#define ALLEGRO_EVENT_USER 1023
#define LEVEL_UP 1
#define GAME_OVER 1
#define FROG_HIT 1
#define FPS 60.0
#define BOARD_SIZE 16


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
    char player[3];
    uint score;
} scorer_t;
typedef char boolean_t;
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
}gameData_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void init_board(boolean_t *board);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
// +ej: char lcd_goto (int fil, int col);+


/*******************************************************************************
 ******************************************************************************/

#endif // _MAIN_H_