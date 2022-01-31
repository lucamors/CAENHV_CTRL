/*
*  CAEN H.V. CTRL
*  -----------------------------------------------------------------------------
*  DT547x CAEN Desktop Power Supply control software
*
*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <gtk/gtk.h>

#include "gui.h"
#include "caen_dt547x_board.h"
#include "caen_usb_protocol.h"


// static gui_context   gui = { 0 };

int main(int argc, char *argv[])
{
   // Init GTK
   gtk_init(&argc, &argv);

   // Initialize GUI Interface with GLADE XML builder
   init_gui();

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
