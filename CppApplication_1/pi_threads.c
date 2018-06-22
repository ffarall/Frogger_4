
/*#include "io_threads.h"


void* input_thread (void* event)//genera eventos de movimiento del joystick
{
  event_t *my_event = event; //asigno puntero para no tener que castear pcada vez que quiero usarlo
  jcoord_t my_coordinates;
  jswitch_t my_switch = J_NOPRESS;
  bool trigger_lock_x = false;
  bool trigger_lock_y = false;
  bool switch_lock = false;

  joy_init();
  set_joy_axis(JOY_ROTATE);	// OJO considero que usamos joystick abajo y no a la izq
  set_joy_direction(J_INV_TRUE,J_INV_TRUE);

  infinite_loop
  {
    joystick_update();
    my_switch=joystick_get_switch_value();//recibe valores actuales del joystick y el boton
    my_coordinates = joystick_get_coord();

    if(my_event->flag != true ) //generador de eventos
    {
       if(!trigger_lock_x)
       {
         if(my_coordinates.x > JOY_THRESHOLD )
         {
          my_event->flag = true;
          my_event->type = RIGHT_EVENT;
          trigger_lock_x = true;
          printf("RIGHT EVENT\n");
         }
         else if(my_coordinates.x < -JOY_THRESHOLD )
         {
          my_event->flag = true;
          my_event->type = LEFT_EVENT;
          trigger_lock_x = true;
          printf("LEFT EVENT\n");
         }
       }
       if(!trigger_lock_y)
       {
         if(my_coordinates.y > JOY_THRESHOLD )
         {
           my_event->flag = true;
           my_event->type = UP_EVENT;
           trigger_lock_y = true;
           printf("UP EVENT\n");
         }
         else if(my_coordinates.y < -JOY_THRESHOLD )
         {
           my_event->flag = true;
           my_event->type = DOWN_EVENT;
           trigger_lock_y = true;
           printf("DOWN EVENT\n");
         }
       }
       if(my_switch == J_PRESS && !switch_lock)
    	{
        my_event->flag = true;
        my_event->type = ENTER_EVENT;
        switch_lock = true;
        printf("ENTER EVENT\n");
    	}
    }
    if (trigger_lock_x && my_coordinates.x < JOY_THRESHOLD && my_coordinates.x > -JOY_THRESHOLD)  //bloqueo de lectura para evitar que se envie el evento si se mantiene presionado
    {
      trigger_lock_x = false;
      //printf("TRIGGERLOCKX OFF\n");
    }
    if (trigger_lock_y && my_coordinates.y < JOY_THRESHOLD && my_coordinates.y > -JOY_THRESHOLD)  //bloqueo de lectura para evitar que se envie el evento si se mantiene presionado
    {
      trigger_lock_y = false;
      //printf("TRIGGERLOCKY OFF\n");
    }
    if (switch_lock && my_switch == J_NOPRESS)  //bloqueo de lectura para evitar que se envie el evento si se mantiene presionado
    {
      switch_lock = false;
      //printf("SWITCHLOCK OFF\n");
    }
  }
} 


/* output_thread (de Raspberry Pi)
Recibe un puntero a la estructura gameData que contiene la informacion del juego
Se encarga de:
-Mover los autos y los troncos
-Imprimir todo en el display de 16x16
-Informar al kernel cuando hubo un choque
-Hacer parpadear la rana para diferenciarla de los autos (para eso usa el thread frogTimeThread)
Nota: no se encarga de mover la rana en el funcionamiento interno del juego, solo imprime su movimiento */

/*void* output_thread(void* pointer)
{
    bool board[DISSIZE][DISSIZE];
    int i,j;
    for( i = 0 ; i < DISSIZE ; i++ )
    {
      for( j = 0 ; j < DISSIZE ; j++ )
      {
        board[i][j] = false; //inicializo tablero apagado
      }
    }
    gameData_t *pGameData = pointer;
    display_init(); // inicializacion del display
    set_display_axis(NORMAL);
    display_clear();
    bool frogTimer = false;
    static pthread_t frogTid;
    pthread_create(&frogTid,NULL,frogTimeThread,&frogTimer);    //creacion de timer para parpadeo de la rana
    bool toggle = false; //variable para el parpadeo de la rana
    infinite_loop
    {
      

      printBoard(pGameData->pBoard[0]);  //VER ACA COMO ES LA ESTRUCTURA GAME DATA Y OJO CON EL NIVEL DEL PUNTERO//Escribe en el display el estado actual de autos y troncos

      if(frogTimer)
      {
          toggle = !toggle;
          display_write(pGameData->frog.x,pGameData->frog.y,toggle);  //prende/apaga la posicion de la rana
        //  display_update();   //SE PUEDE SACAR SIEMPRE QUE ESTO ESTE CERCA DEL FINAL DEL LOOP
      	  frogTimer = false;
      }

      display_update();
    }
}*/

/*printBoard:
Recibe un puntero al primer elemento de un arreglo de bools
Escribe en un display de 16x16 el contenido de ese arreglo
Nota: debe estar inicializado el display con las herramientas de <disdrv.h> */
/*void printBoard(bool p2board[][DISSIZE])
{
  int i,j;
  for( i=0 ; i < DISSIZE ; i++ )
  {
    for( j=0 ; j < DISSIZE ; j++)
    {
      display_write(i,j,p2board[i][j]);
    }
  }
}

/*frogTimeThread
Timer para el parpadeo de la rana */
void* frogTimeThread (void* p2timer)
{
  infinite_loop
  {
   if(*(bool*) p2timer == false)  //solo arranca nuevamente cuando hayan leido el evento
   {
     usleep(FROGTIME);
     *(bool*)p2timer = true;
   }
  }
  return NULL;
}       */
