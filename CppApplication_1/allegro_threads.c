void allegrokb(void* event_str)
{
  /***********INICIALIZACIÓN DE ELEMENTOS DE ALLEGRO*****/
  ALLEGRO_EVENT_QUEUE *ev_queue = NULL;

  if(!al_install_keyboard())
  {
    fprintf(stderr,"Failed to install keyboard!\n");
    exit(-1);                                                       // interrumpo la ejecución de todo el programa
  }

  ev_queue = al_create_event_queue();

  if(!ev_queue)
  {
    fprintf(stderr,"Failed to create even queue!\n");
    exit(-1);                                                     // interrumpo la ejecución de todo el programa
  }

  al_register_event_source(ev_queue,al_get_keyboard_event_source());

  /**************ESPERA DE EVENTOS******************/

  while(1)
  {
    ALLEGRO_EVENT event;  // creo estructura de eventos
    int key_pressed;
    if(al_get_next_event(ev_queue,&event))
    {
      if((event.type == ALLEGRO_KEY_DOWN) && ((event_t*)(event_str))->flag == 0)   // chequeo si el evento anterior fue tomado por el núcleo
      {
        switch(event.keyboard.keycode)
        {
          case ALLEGRO_KEY_UP:
            key_pressed = UP_EVENT;
            break;

          case ALLEGRO_KEY_DOWN:
            key_pressed = DOWN_EVENT;
            break;

          case ALLEGRO_KEY_LEFT:
            key_pressed = LEFT_EVENT;
            break;

          case ALLEGRO_KEY_RIGHT:
            key_pressed = RIGHT_EVENT;
            break;

          case ALLEGRO_KEY_ENTER:
            key_pressed = ENTER_EVENT;
            break;

          case ALLEGRO_KEY_P:
            key_pressed = PAUSE_EVENT;
            break;

      /* para que se utilice el mismo thread para el input de datos (como caracteres), se deben agregar cases con las respectivas teclas */

        }
/*usar mutex para bloquear el acceso a la estructura !!!!!!!!! */

        ((event_t*)(event_str))->flag == 1;             // set del flag de EVENTOS
        ((event_t*)(event_str))->type == key_pressed;   // descripción del tipo de evento

      }
    }
  }

}