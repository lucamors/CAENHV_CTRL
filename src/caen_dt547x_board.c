#include "caen_dt547x_board.h"
#include "gui.h"
#include "caen_usb_protocol.h"

////////////////////////////////////////////////////////////////////////////////

void configure_caen_serial_comm(struct termios * ptr_tty, int vtime, int vmin, int serial_port)
{
   // Parity bit set to none
   ptr_tty->c_cflag &= ~PARENB;
   // Only 1 stop bit required
   ptr_tty->c_cflag &= ~CSTOPB;
   // Reset size bit
   ptr_tty->c_cflag &= ~CSIZE;
   // Set size to 8 bit at a time
   ptr_tty->c_cflag |= CS8;
   // Enable hardware flow control
   ptr_tty->c_cflag |= CRTSCTS;

   ptr_tty->c_lflag &= ~ICANON;
   ptr_tty->c_lflag &= ~ECHO;
   ptr_tty->c_lflag &= ~ECHOE;
   ptr_tty->c_lflag &= ~ECHONL;
   ptr_tty->c_lflag &= ~ISIG;

   ptr_tty->c_iflag &= ~(IXON | IXOFF | IXANY);
   ptr_tty->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

   ptr_tty->c_oflag &= ~OPOST;
   ptr_tty->c_oflag &= ~ONLCR;

   ptr_tty->c_cc[VTIME] = vtime;
   ptr_tty->c_cc[VMIN]  = vmin;

   // Setting input/output baudrate
   cfsetispeed(ptr_tty, B9600);
   cfsetospeed(ptr_tty, B9600);

   // Saving tty settings and errors checking
   if (tcsetattr(serial_port, TCSANOW, ptr_tty) != 0)
   {
       printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
       exit(-1);
   }

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void get_board_informations(caen_hv_state* s)
{
   read_board_name(s);
   read_channel_polarity(s);
   read_voltage_set(s);
   read_current_set(s);
   read_ramp_up(s);
   read_ramp_down(s);
   read_power_down_mode(s);
   read_imrange(s);
   read_trip(s);
   read_maximum_voltage(s);
   read_maximum_current(s);
   read_board_status(s);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void print_board_information(caen_hv_state* s)
{
   printf("Board Name      : %s\n", s->board_name);
   printf("Polarity        : %s\n", s->polarity ? "Positive" : "Negative");
   printf("Voltage Set     : %4f V\n", s->vset );
   printf("Current Set     : %4f uA\n", s->iset );
   printf("Ramp Up         : %4f V\n", s->ramp_up );
   printf("Ramp Down       : %4f uA\n", s->ramp_down );
   printf("Power Down Mode : %s\n", s->power_down_mode ? "KILL" : "RAMP" );
   printf("IMRANGE         : %s\n", s->im_range ? "HIGH" : "LOW" );
   printf("Trip value      : %4f\n", s->trip );
   printf("Maximum Voltage : %4f V\n", s->max_v);
   printf("Maximum Current : %4f uA\n", s->max_i);

   return ;
}
