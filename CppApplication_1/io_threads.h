#ifndef IO_THREADS_H
#define IO_THREADS_H

#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "termlib.h"
#include "joydrv.h"
#include "disdrv.h"
#include "fsm.h"

#define infinite_loop while(1)
#define JOY_THRESHOLD 70     // 10-100 Joystick threshold (sensitivity)
#define DISSIZE 16
#define FROGTIME 1000

void* input_thread (void* event);
void* output_thread(void* p2gameData);


#endif /* IO_THREADS_H*/

