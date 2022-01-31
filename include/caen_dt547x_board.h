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

// Global CAEN HV BOARsD STATE

typedef struct
{
   int serial_port;

   unsigned char board_name[50];
   bool polarity;

   float voltage_level;
   float current_level;

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

void configure_caen_serial_comm(struct termios * ptr_tty, int vtime, int vmin, int serial_port);


void get_board_informations(caen_hv_state* s);
void print_board_information(caen_hv_state* s);
