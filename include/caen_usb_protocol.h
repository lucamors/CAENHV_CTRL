#pragma once

#include "utils.h"

enum status_bits{
   ON,
   RUP,
   RDW,
   OVC,
   OVV,
   UNV,
   MAXV,
   TRIP,
   OVP,
   RES0,
   DIS,
   KILL,
   ILK,
   NOCAL,
   RES1,
   RES2
};

enum pdmodes{MODE_RAMP,MODE_KILL};

// Helper functions
void read_from_serial(int serial_port, unsigned char * response, int skip_level);

// Monitor Commands
void read_board_name(caen_hv_state* s);
void read_channel_polarity(caen_hv_state* s);
void read_voltage_set(caen_hv_state* s);
void read_current_set(caen_hv_state* s);
float read_voltage(caen_hv_state* s);
float read_current(caen_hv_state* s);
void read_ramp_up(caen_hv_state* s);
void read_ramp_down(caen_hv_state* s);
void read_power_down_mode(caen_hv_state* s);
void read_imrange(caen_hv_state* s);
void read_trip(caen_hv_state* s);
void read_maximum_voltage(caen_hv_state* s);
void read_maximum_current(caen_hv_state* s);
void read_board_status(caen_hv_state* s);

// Set Commands

void set_voltage_level(caen_hv_state* s, float v);
void set_maximum_current(caen_hv_state* s, float imax);
void set_ramp_up_value(caen_hv_state* s, float ramp_up);
void set_ramp_down_value(caen_hv_state* s, float ramp_down);
void set_trip(caen_hv_state* s, float trip);
void set_power_down_mode(caen_hv_state* s, int pdmode);
void set_imrange(caen_hv_state* s, int imode);
void set_channel_on(caen_hv_state* s);
void set_channel_off(caen_hv_state* s);
void board_clear_signal(caen_hv_state* s);
