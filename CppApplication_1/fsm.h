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



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct STATE
{
    ALLEGRO_EVENT event;
    struct STATE *nextState;
    void (*actionRoutine)(void *);
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
}gameData_t;



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void nonActRoutine(void *);

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