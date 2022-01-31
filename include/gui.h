#pragma once
#include <gtk/gtk.h>

#include "utils.h"
#include "caen_usb_protocol.h"

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *btn_connect;
GtkBuilder *builder;

// Board Connection dashboard
GtkWidget *label_status;
GtkWidget *label_command;
GtkWidget *btn_ch1_enable;
GtkWidget *btn_alarm_reset;
GtkWidget *btn_refresh;

// Command dashboard
GtkWidget *combo_cmd;
GtkWidget *entry_cmd_val;
GtkWidget *btn_send;


// Flags labels
GtkWidget *flag_buttons[16];


// Monitor Labels
GtkWidget * label_voltage_level;
GtkWidget * label_current_level;
GtkWidget * label_vset;
GtkWidget * label_iset;
GtkWidget * label_ramp_up;
GtkWidget * label_ramp_down;
GtkWidget * label_trip;

// Mode switches
GtkWidget * btn_imrange;
GtkWidget * btn_power_down_mode;

void refresh_gui_monitor(caen_hv_state* s);
void clear_flags();


typedef void (*command)(caen_hv_state*, float);


void test(caen_hv_state* s, float val);

enum gtk_set_cmd{SET_VSET,SET_ISET,SET_RAMP_UP,SET_RAMP_DOWN,SET_TRIP};

static command commands[10] =
{
   [SET_VSET]      = set_voltage_level,
   [SET_ISET]      = set_maximum_current,
   [SET_RAMP_UP]   = set_ramp_up_value,
   [SET_RAMP_DOWN] = set_ramp_down_value,
   [SET_TRIP]      = set_trip
};
