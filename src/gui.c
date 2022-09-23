#include <gtk/gtk.h>
#include "gui.h"
#include "caen_usb_protocol.h"

////////////////////////////////////////////////////////////////////////////////

void refresh_gui_monitor(caen_hv_state* s)
{
   for (int i = 0; i < 16; i++)
   {
      // Continue if reserved bit
      if( i == RES0 || i == RES1 || i == RES2) continue;

      gboolean is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(flag_buttons[i]));
      if(s->status[i] && !is_active)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(flag_buttons[i]),true);
      }

      if(!s->status[i] && is_active)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(flag_buttons[i]),false);
      }

#ifdef DEBUG
      is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(flag_buttons[i]));
      printf("[dbg] > Bit %i is %i\n", i, is_active);
      printf("[dbg] > %i %i\n", i, s->status[i]);
#endif

   }

   char monitor_string[50];

   // Update VSET
   sprintf(monitor_string,"VSET [V]: %.2f", s->vset);
   gtk_label_set_text(GTK_LABEL(label_vset), (const gchar *) monitor_string);

   // Update ISET
   sprintf(monitor_string,"ISET [V]: %.2f", s->iset);
   gtk_label_set_text(GTK_LABEL(label_iset), (const gchar *) monitor_string);

   // Update VMON
   sprintf(monitor_string,"Voltage [V]: %.2f", s->voltage_level);
   gtk_label_set_text(GTK_LABEL(label_voltage_level), (const gchar *) monitor_string);

   // Update IMON
   sprintf(monitor_string,"Current [uA]: %.2f", s->current_level);
   gtk_label_set_text(GTK_LABEL(label_current_level), (const gchar *) monitor_string);

   // Update RAMPUP
   sprintf(monitor_string,"RAMP UP [V]: %.2f", s->ramp_up);
   gtk_label_set_text(GTK_LABEL(label_ramp_up), (const gchar *) monitor_string);

   // Update RAMPDOWN
   sprintf(monitor_string,"RAMP DOWN [V]: %.2f", s->ramp_down);
   gtk_label_set_text(GTK_LABEL(label_ramp_down), (const gchar *) monitor_string);

   // Update TRIP
   sprintf(monitor_string,"TRIP [V]: %.2f", s->trip);
   gtk_label_set_text(GTK_LABEL(label_trip), (const gchar *) monitor_string);

   // Update switch buttons
   // not supported yet

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void init_gui()
{
   // Build GUI from GLADE XML File
  builder = gtk_builder_new_from_file("glade/gui.glade");
   // Main Window and Fixed Container
   window  = GTK_WIDGET(gtk_builder_get_object(builder,"window"));
   fixed   = GTK_WIDGET(gtk_builder_get_object(builder,"fixed"));

   // Connect signal to window
   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
   gtk_builder_connect_signals(builder, NULL);


   // Connection Dashboard
   btn_connect     = GTK_WIDGET(gtk_builder_get_object(builder,"btn_connect"));
   btn_disconnect  = GTK_WIDGET(gtk_builder_get_object(builder,"btn_disconnect"));
   label_status    = GTK_WIDGET(gtk_builder_get_object(builder,"label_status"));
   btn_ch1_enable  = GTK_WIDGET(gtk_builder_get_object(builder,"btn_ch1_enable"));
   btn_alarm_reset = GTK_WIDGET(gtk_builder_get_object(builder,"btn_alarm_reset"));

   // Commands Dashboard
   combo_cmd     = GTK_WIDGET(gtk_builder_get_object(builder,"combo_cmd"));
   entry_cmd_val = GTK_WIDGET(gtk_builder_get_object(builder,"entry_cmd_val"));
   btn_send      = GTK_WIDGET(gtk_builder_get_object(builder,"btn_send"));
   btn_refresh   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_refresh"));

   // FLAGS Dashboard
   flag_buttons[ON]    = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_on"));
   flag_buttons[RUP]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_ramp_up"));
   flag_buttons[RDW]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_ramp_down"));
   flag_buttons[OVC]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_over_current"));
   flag_buttons[OVV]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_over_voltage"));
   flag_buttons[UNV]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_under_voltage"));
   flag_buttons[MAXV]  = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_maxv"));
   flag_buttons[TRIP]  = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_current_gen"));
   flag_buttons[OVP]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_over_temperature"));
   flag_buttons[DIS]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_chdis"));
   flag_buttons[KILL]  = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_kill"));
   flag_buttons[ILK]   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_interlock"));
   flag_buttons[NOCAL] = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_calibration_error"));
   flag_buttons[RES0]  = NULL;
   flag_buttons[RES1]  = NULL;
   flag_buttons[RES2]  = NULL;

   // Switches
   btn_imrange = GTK_WIDGET(gtk_builder_get_object(builder,"btn_imrange"));
   btn_power_down_mode = GTK_WIDGET(gtk_builder_get_object(builder,"btn_power_down_mode"));

   // Monitor Dashboard
   label_voltage_level = GTK_WIDGET(gtk_builder_get_object(builder,"label_voltage_level"));
   label_current_level = GTK_WIDGET(gtk_builder_get_object(builder,"label_current_level"));
   label_vset          = GTK_WIDGET(gtk_builder_get_object(builder,"label_vset"));
   label_iset          = GTK_WIDGET(gtk_builder_get_object(builder,"label_iset"));
   label_ramp_up       = GTK_WIDGET(gtk_builder_get_object(builder,"label_ramp_up"));
   label_ramp_down     = GTK_WIDGET(gtk_builder_get_object(builder,"label_ramp_down"));
   label_trip          = GTK_WIDGET(gtk_builder_get_object(builder,"label_trip"));

   return ;
}

////////////////////////////////////////////////////////////////////////////////
// This function do not function properly !!!
void clear_flags()
{
   for (int i = 0; i < 16; i++)
   {
      if( i == RES0 || i == RES1 || i == RES2) continue;

      gboolean is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(flag_buttons[i]));

      if(is_active)
      {
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(flag_buttons[i]),false);
      }
   }

   return ;
}

////////////////////////////////////////////////////////////////////////////////
// GUI Signals functions
////////////////////////////////////////////////////////////////////////////////

extern caen_hv_state state;

void on_btn_connect_clicked(GtkButton *b)
{
   // Opening the serial port
   state.serial_port = open("/dev/ttyACM0", O_RDWR);
   // sleep - questo serve?
   usleep(10000);

   // Check for errors and read in existing settings
   struct termios tty;
   if( tcgetattr(state.serial_port, &tty) != 0)
   {
      printf("Error %i from tcgetattr : %s\n", errno, strerror(errno));
      exit(-1);
   }

   // configure serial port settings as defined
   // by the CAEN USB protocol
   int delay = 10; // 1 s
   configure_caen_serial_comm(&tty,delay, NON_BLOCKING, state.serial_port);


#ifdef DEBUG
   printf("Connected Board on serial port %i\n", state.serial_port);
   print_board_information(&state);
#endif

   // Retrieve Board Information
   get_board_informations(&state);

   if(state.status[ON] == true)
   {
      gtk_switch_set_active(GTK_SWITCH(btn_ch1_enable),true);
   }

   if(state.status[KILL] == true)
   {
      gtk_switch_set_active(GTK_SWITCH(btn_power_down_mode),true);
   }

   char status_msg[100];
   sprintf(status_msg,"Connected : %s\n", state.board_name);

   gtk_label_set_text(GTK_LABEL(label_status), (const gchar *) status_msg);

   // Refreshing GTK GUI Monitor
   refresh_gui_monitor(&state);

   return ;
}

////////////////////////////////////////////////////////////////////////////////
// This function needs refactoring
void on_btn_disconnect_clicked(GtkButton *b)
{
   set_channel_off(&state);
   board_clear_signal(&state);
   clear_flags();
   refresh_gui_monitor(&state);

   // Closing serial port
   close(state.serial_port);

   // Updating connection status label
   gtk_label_set_text(GTK_LABEL(label_status), (const gchar *) "Disconnected");
}

void on_btn_send_clicked(GtkButton* b)
{
   // Retrieve combo box active text
   int cmd_id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_cmd));

   // get selected function
   command cmd = commands[cmd_id];

   // Retrieve entry text value
   const gchar * str_val = gtk_entry_get_text(GTK_ENTRY(entry_cmd_val));
   float val = atof(str_val);

   // Update status
   cmd(&state, val);

   // Update flag buttons
   refresh_gui_monitor(&state);

   return ;
}

void on_btn_alarm_reset_clicked(GtkButton *b)
{
   if(state.serial_port <= 0)
   {
#ifdef DEBUG
      printf("[dbg] > No Board Currently Connected.\n");
#endif
      return ;
   }

   // Send alarm reset signal
   board_clear_signal(&state);

   //Update flag buttons
   refresh_gui_monitor(&state);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void on_btn_ch1_enable_state_set(GtkSwitch* b)
{
   gboolean is_active = gtk_switch_get_active(GTK_SWITCH(btn_ch1_enable));


   if(is_active)
   {
      set_channel_on(&state);
   }
   else
   {
      set_channel_off(&state);
   }

   //Update flag buttons
   refresh_gui_monitor(&state);

   return ;
}

void on_btn_monitor_clicked(GtkButton * b)
{
   // Refresh Monitor
   read_voltage(&state);
   read_current(&state);

   refresh_gui_monitor(&state);
   return ;
}

////////////////////////////////////////////////////////////////////////////////

void on_btn_imrange_state_set(GtkSwitch * s)
{
   gboolean is_active = gtk_switch_get_active(GTK_SWITCH(btn_imrange));

   if(is_active)
   {
      set_imrange(&state,1);
   }
   else
   {
      set_imrange(&state,0);
   }
}

////////////////////////////////////////////////////////////////////////////////

void on_btn_power_down_mode_state_set(GtkSwitch * s)
{
   gboolean is_active = gtk_switch_get_active(GTK_SWITCH(btn_imrange));

   if(is_active)
   {
      set_power_down_mode(&state,1);
   }
   else
   {
      set_power_down_mode(&state,0);
   }
}

////////////////////////////////////////////////////////////////////////////////
