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

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *btn_connect;
GtkBuilder *builder;

GtkWidget *label_status;
GtkWidget *label_command;
GtkWidget *combo_cmd;
GtkWidget *entry_cmd_val;
GtkWidget *btn_send;

GtkWidget *btn_flag_on;

// Global CAEN HV BOARD STATE
static caen_hv_state state = { 0 };

int main(int argc, char *argv[])
{


   gtk_init(&argc, &argv);

   builder = gtk_builder_new_from_file("glade/gui.glade");
   window  = GTK_WIDGET(gtk_builder_get_object(builder,"window"));

   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

   gtk_builder_connect_signals(builder, NULL);

   fixed         = GTK_WIDGET(gtk_builder_get_object(builder,"fixed"));
   btn_connect   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_connect"));
   label_status  = GTK_WIDGET(gtk_builder_get_object(builder,"label_status"));
   combo_cmd     = GTK_WIDGET(gtk_builder_get_object(builder,"combo_cmd"));
   entry_cmd_val = GTK_WIDGET(gtk_builder_get_object(builder,"entry_cmd_val"));
   btn_send      = GTK_WIDGET(gtk_builder_get_object(builder,"btn_send"));
   btn_flag_on   = GTK_WIDGET(gtk_builder_get_object(builder,"btn_flag_on"));


   gtk_widget_show(window);

   // Setting back locale to english UTF-8
   // how to proper handle this???
   setlocale(LC_ALL, "C");


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

   printf("Serial Port %i\n", state.serial_port);
   gtk_label_set_text(GTK_LABEL(label_status), (const gchar *) "Connected");

   get_board_informations(&state);
   print_board_information(&state);

   // char vset_value[10];
   // sprintf(vset_value,"%4f",state.vset);
   // gtk_label_set_text(GTK_LABEL(label_vset_value), (const gchar *) vset_value);

}

void on_btn_disconnect_clicked(GtkButton *b)
{
   // Closing serial port
   close(state.serial_port);

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

   cmd(&state, val);

   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(btn_flag_on),true);


   return ;
}
