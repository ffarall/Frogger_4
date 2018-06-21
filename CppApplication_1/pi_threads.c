/*#include "input_thread.h"


void* input_thread (void* event)//genera eventos de movimiento del joystick
{
  event_t *my_event = event; //asigno puntero para no tener que castear pcada vez que quiero usarlo
  jcoord_t my_coordinates;
  jswitch_t my_switch=J_NOPRESS;
  bool trigger_lock = false;
  joy_init();
  set_joy_axis(JOY_ROTATE);	// OJO considero que usamos joystick abajo y no a la izq
  set_joy_direction(J_INV_TRUE,J_INV_TRUE);

  infinite_loop
  {
    joystick_update();
    my_switch=joystick_get_switch_value();//recibo valore actuales del joystick y el boton
    my_coordinates = joystick_get_coord();


    if(my_event->flag != true && !trigger_lock) //generador de eventos
    {
      if(my_coordinates.x > JOY_THRESHOLD )
      {
    		my_event->flag = true;
        my_event->type = RIGHT_EVENT;
        trigger_lock = true;
      }
    	else if(my_coordinates.x < -JOY_THRESHOLD )
      {
        my_event->flag = true;
        my_event->type = LEFT_EVENT;
        trigger_lock = true;
      }
    	else if(my_coordinates.y > JOY_THRESHOLD )
      {
        my_event->flag = true;
        my_event->type = UP_EVENT;
        trigger_lock = true;
      }
    	else if(my_coordinates.y < -JOY_THRESHOLD )
      {
        my_event->flag = true;
        my_event->type = DOWN_EVENT;
        trigger_lock = true;
      }
      else if(my_switch == J_PRESS)
    	{
        my_event->flag = true;
        my_event->type = ENTER_EVENT;
        trigger_lock = true;
    	}
    }
    if (trigger_lock)  //bloqueo de lectura para evitar que se envie el evento si se mantiene presionado
    {
      if (my_coordinates.x < JOY_THRESHOLD && my_coordinates.x > -JOY_THRESHOLD\
       && my_coordinates.y < JOY_THRESHOLD && my_coordinates.y > -JOY_THRESHOLD\
       && my_switch == J_NOPRESS)//se puede mejorar este if?
        {
          trigger_lock = false;
        }
    }
  }
}*/