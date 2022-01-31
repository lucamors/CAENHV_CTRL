#pragma once
#include <gtk/gtk.h>

#include "utils.h"
#include "caen_usb_protocol.h"
#include "gui.h"

// Main Windows
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *fixed;

// Board Connection dashboard
GtkWidget *label_status;
GtkWidget *label_command;
GtkWidget *btn_connect;
GtkWidget *btn_disconnect;
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

// Helper Functions
void refresh_gui_monitor(caen_hv_state* s);
void init_gui();
void clear_flags();

// GTK Signals
void on_btn_connect_clicked(GtkButton *b);
void on_btn_disconnect_clicked(GtkButton *b);
void on_btn_send_clicked(GtkButton* b);
void on_btn_alarm_reset_clicked(GtkButton *b);
void on_btn_ch1_enable_state_set(GtkSwitch* b);
void on_btn_monitor_clicked(GtkButton * b);
void on_btn_imrange_state_set(GtkSwitch * s);
void on_btn_power_down_mode_state_set(GtkSwitch * s);


// Define functions dispatcher
typedef void (*command)(caen_hv_state*, float);

enum gtk_set_cmd{SET_VSET,SET_ISET,SET_RAMP_UP,SET_RAMP_DOWN,SET_TRIP};

static command commands[5] =
{
   [SET_VSET]      = set_voltage_level,
   [SET_ISET]      = set_maximum_current,
   [SET_RAMP_UP]   = set_ramp_up_value,
   [SET_RAMP_DOWN] = set_ramp_down_value,
   [SET_TRIP]      = set_trip
};
