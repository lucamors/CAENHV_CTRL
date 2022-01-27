#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>

#include "utils.h"
#include "caen_usb_protocol.h"

int main(int argc, char const *argv[])
{
   // Initialize HV board state
   static caen_hv_state state = { 0 };

   // Opening the serial port
   state.serial_port = open("/dev/ttyACM0", O_RDWR);
   // sleep - questo serve?
   usleep(10000);

   // Check for errors and read in existing settings
   struct termios tty;
   if( tcgetattr(state.serial_port, &tty) != 0)
   {
      printf("Error %i from tcgetattr : %s\n", errno, strerror(errno));
      return 1;
   }

   // configure serial port settings as defined
   // by the CAEN USB protocol
   int delay = 10; // 1 s
   configure_caen_serial_comm(&tty,delay, NON_BLOCKING, state.serial_port);

   // do stuff . . .

   // Closing serial port
   close(state.serial_port);

   return 0;
}
