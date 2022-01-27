#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define NON_BLOCKING 0
#define BLOCKING     1

typedef struct
{
   int serial_port;

   unsigned char board_name[50];
   bool polarity;
   float vset;
   float iset;
   float ramp_up;
   float ramp_down;
   int power_down_mode;
   bool im_range;
   float trip;
   float max_v;
   float max_i;

   bool status[16];

}  caen_hv_state;


enum gtk_set_cmd{SET_VSET,SET_ISET,SET_RAMP_UP,SET_RAMP_DOWN,SET_TRIP};

typedef void (*command)(caen_hv_state*, float);

void test(caen_hv_state* s, float val);

static command commands[10] =
{
   [SET_VSET]      = test,
   [SET_ISET]      = test,
   [SET_RAMP_UP]   = test,
   [SET_RAMP_DOWN] = test,
   [SET_TRIP]      = test,

};

void configure_caen_serial_comm(struct termios * ptr_tty, int vtime, int vmin, int serial_port);


void get_board_informations(caen_hv_state* s);
void print_board_information(caen_hv_state* s);
