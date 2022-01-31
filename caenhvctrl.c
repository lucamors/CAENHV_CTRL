#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <gtk/gtk.h>

#include "utils.h"
#include "caen_usb_protocol.h"
#include "gui.h"

static caen_hv_state state = { 0 };

int main(int argc, char *argv[])
{
   gtk_init(&argc, &argv);

   builder = gtk_builder_new_from_file("glade/gui.glade");
   window  = GTK_WIDGET(gtk_builder_get_object(builder,"window"));

   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

   gtk_builder_connect_signals(builder, NULL);

   fixed                     = GTK_WIDGET(gtk_builder_get_object(builder,"fixed"));
   btn_connect               = GTK_WIDGET(gtk_builder_get_object(builder,"btn_connect"));
   label_status              = GTK_WIDGET(gtk_builder_get_object(builder,"label_status"));
   combo_cmd                 = GTK_WIDGET(gtk_builder_get_object(builder,"combo_cmd"));
   entry_cmd_val             = GTK_WIDGET(gtk_builder_get_object(builder,"entry_cmd_val"));
   btn_send                  = GTK_WIDGET(gtk_builder_get_object(builder,"btn_send"));
   btn_ch1_enable            = GTK_WIDGET(gtk_builder_get_object(builder,"btn_ch1_enable"));
   btn_alarm_reset           = GTK_WIDGET(gtk_builder_get_object(builder,"btn_alarm_reset"));

   btn_refresh               = GTK_WIDGET(gtk_builder_get_object(builder,"btn_refresh"));

   // Linking widget to flags radio buttons
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

   flag_buttons[RES0] = NULL;
   flag_buttons[RES1] = NULL;
   flag_buttons[RES2] = NULL;

   btn_imrange = GTK_WIDGET(gtk_builder_get_object(builder,"btn_imrange"));
   btn_power_down_mode = GTK_WIDGET(gtk_builder_get_object(builder,"btn_power_down_mode"));

   // Linking widget to monitor labels
   label_voltage_level = GTK_WIDGET(gtk_builder_get_object(builder,"label_voltage_level"));
   label_current_level = GTK_WIDGET(gtk_builder_get_object(builder,"label_current_level"));
   label_vset          = GTK_WIDGET(gtk_builder_get_object(builder,"label_vset"));
   label_iset          = GTK_WIDGET(gtk_builder_get_object(builder,"label_iset"));
   label_ramp_up       = GTK_WIDGET(gtk_builder_get_object(builder,"label_ramp_up"));
   label_ramp_down     = GTK_WIDGET(gtk_builder_get_object(builder,"label_ramp_down"));
   label_trip          = GTK_WIDGET(gtk_builder_get_object(builder,"label_trip"));

   // Showing GUI
   gtk_widget_show(window);

   // Setting back locale to english UTF-8
   // how to proper handle this??? and why it is activated another local?
   // is gtk faults?
   setlocale(LC_ALL, "C");

   // Running gtk main loop
   gtk_main();

   return 0;
}

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
