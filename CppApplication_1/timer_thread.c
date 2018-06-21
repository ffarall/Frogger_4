#include "timer_thread.h"


void* timer_thread(void* event)
{
    event_t *my_event = event; //asigno puntero para no tener que castear pcada vez que quiero usarlo
    infinite_loop
    {
        if(my_event->timerFlag == false)//OJO esto es si queres que espere a que el main lo vea, si no te importa sacalo, pero cuidado hay que poner semaforos y/o MUTEX (creo que mutex)
        {
            usleep(TIME_MS*1000);
            my_event->timerFlag = true;
        }
    }
}       

dejo esto por aca para que te tire error de compilacion y te obligue a leer los comentarios(perdon)