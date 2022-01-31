#include "caen_usb_protocol.h"
#include "caen_dt547x_board.h"

#include <string.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// MONITOR COMMANDS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

void read_board_status(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:STAT\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response, 1);

   uint16_t status = atoll(response);

   for (int i = 0; i < 16; i++)
   {
      s->status[i] = (status & (1 << i) ) >> (i);
   }

   return ;
}

////////////////////////////////////////////////////////////////////////////////


void read_from_serial(int serial_port, unsigned char * response, int skip_level)
{
   // Response bufferx
   unsigned char read_buf[50];
   memset(&read_buf, '\0', sizeof(read_buf));

   int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

   if (num_bytes < 0)
   {
        printf("Error reading: %s", strerror(errno));
        exit(-1);
   }

   // Retrieving board name
   int semicolon_counts = 0;

   int split_index = 0;
   for(int i = 0; i < strlen(read_buf); i++)
   {
      if(read_buf[i] == ':'){ semicolon_counts++; }
      split_index++;

      if(semicolon_counts > skip_level)
      {
         break;
      }
   }

   // Filter out response
   strncpy(response, &read_buf[split_index], strlen(read_buf)-split_index);
   // Stripping Carriage Return <CR> and Line Feed <LF>
   response[strcspn(response, "\r\n")] = 0;

   return ;//
}

////////////////////////////////////////////////////////////////////////////////

void read_board_name(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:BDNAME\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);//

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   // Copy name string
   strncpy(s->board_name, response, strlen(response));

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_channel_polarity(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:POLARITY\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->polarity = response[0] == '-' ? 0 : 1;

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_voltage_set(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:VSET\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->vset = atof(response);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_current_set(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:ISET\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->iset = atof(response);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

float read_voltage(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:VMON\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(5000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->voltage_level =  atof(response);

   #ifdef DEBUG
      printf(">>>vl: %.2f\n",s->voltage_level);
   #endif
}

////////////////////////////////////////////////////////////////////////////////

float read_current(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:IMON\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->current_level =  atof(response);
}

////////////////////////////////////////////////////////////////////////////////

void read_ramp_up(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:RUP\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->ramp_up =  atof(response);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_ramp_down(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:RDW\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->ramp_down =  atof(response);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_power_down_mode(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:PDWN\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->power_down_mode = strcmp(response,"RAMP") ? MODE_KILL : MODE_RAMP;

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_imrange(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:IMRANGE\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->im_range = !strcmp(response,"HIGH") ? 1 : 0;

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_trip(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:TRIP\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->trip =  atof(response);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_maximum_voltage(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:MAXV\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->max_v =  atof(response);

   return ;
}

////////////////////////////////////////////////////////////////////////////////

void read_maximum_current(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:MON,PAR:IMAX\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,1);

   s->max_i =  atof(response);

   return ;
}



////////////////////////////////////////////////////////////////////////////////
// SET COMMANDS
////////////////////////////////////////////////////////////////////////////////

void set_voltage_level(caen_hv_state *s, float v)
{
   // Writing to serial
   unsigned char cmd[50];
   sprintf(cmd,"$CMD:SET,PAR:VSET,VAL:%.3f\r\n",v);
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET VOLTAGE COMMAND\n");
      exit(-40);
   }

   s->vset = v;

   read_board_status(s);
   return ;
}

////////////////////////////////////////////////////////////////////////////////

void set_maximum_current(caen_hv_state *s, float imax)
{
   // Writing to serial

   unsigned char cmd[50];
   sprintf(cmd,"$CMD:SET,PAR:ISET,VAL:%3f\r\n",imax);
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET MAXIMUM CURRENT COMMAND\n");
      exit(-41);
   }

   s->iset = imax;

   read_board_status(s);
   return ;

}

////////////////////////////////////////////////////////////////////////////////

void set_ramp_up_value(caen_hv_state *s, float ramp_up)
{
   // Writing to serial

   unsigned char cmd[50];
   sprintf(cmd,"$CMD:SET,PAR:RUP,VAL:%3f\r\n",ramp_up);
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET RAMP UP COMMAND\n");
      exit(-42);
   }

   s->ramp_up = ramp_up;

   read_board_status(s);
   return ;
}

////////////////////////////////////////////////////////////////////////////////

void set_ramp_down_value(caen_hv_state *s, float ramp_down)
{
   // Writing to serial

   unsigned char cmd[50];
   sprintf(cmd,"$CMD:SET,PAR:RDW,VAL:%3f\r\n",ramp_down);
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET RAMP DOWN COMMAND\n");
      exit(-42);
   }

   s->ramp_down = ramp_down;

   read_board_status(s);
   return ;
}

////////////////////////////////////////////////////////////////////////////////

void set_trip(caen_hv_state *s, float trip)
{
   // Writing to serial
   unsigned char cmd[50];
   sprintf(cmd,"$CMD:SET,PAR:TRIP,VAL:%3f\r\n",trip);
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET TRIP COMMAND\n");
      exit(-42);
   }

   s->trip = trip;


   read_board_status(s);
   return ;
}

////////////////////////////////////////////////////////////////////////////////

void set_power_down_mode(caen_hv_state *s, int pdmode)
{
   // Writing to serial
   unsigned char cmd[50];
   char* modes[2] = {"RAMP","KILL"};

   sprintf(cmd,"$CMD:SET,PAR:PDWN,VAL:%s\r\n", modes[pdmode]);
   printf("%s\n", cmd);

   printf(">>p %i\n", s->status[KILL]);
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);


   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   read_board_status(s);
   printf(">>d %i\n", s->status[KILL]);

   printf("%s\n", response);
   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET POWER DOWN MODE COMMAND\n");
      exit(-42);
   }

   s->power_down_mode = (pdmode == MODE_KILL) ? MODE_KILL: MODE_RAMP;


   return ;
}

////////////////////////////////////////////////////////////////////////////////

void set_imrange(caen_hv_state *s, int imode)
{
   // Writing to serial
   unsigned char cmd[50];
   char* modes[2] = {"LOW","HIGH"};

   sprintf(cmd,"$CMD:SET,PAR:PDWN,VAL:%s\r\n", modes[imode]);
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET IMRANGE COMMAND\n");
      exit(-42);
   }

   s->im_range = imode ? 1 : 0 ;

   read_board_status(s);
   return ;
}

////////////////////////////////////////////////////////////////////////////////

void set_channel_on(caen_hv_state *s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:SET,PAR:ON\r\n";
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET CHANNEL ON COMMAND\n");
      exit(-41);
   }


   read_board_status(s);

   printf("[dbg] > Channel 1 : %i\n", s->status[ON]);
   return ;

}

////////////////////////////////////////////////////////////////////////////////

void set_channel_off(caen_hv_state *s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:SET,PAR:OFF\r\n";
   write(s->serial_port, cmd, sizeof(cmd));
   usleep(2000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response,0);

   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET CHANNEL ON COMMAND\n");
      exit(-41);
   }


   read_board_status(s);

   return ;
}


////////////////////////////////////////////////////////////////////////////////

void board_clear_signal(caen_hv_state* s)
{
   // Writing to serial
   unsigned char cmd[] = "$CMD:SET,PAR:BDCLR\r\n";
   write(s->serial_port, cmd, sizeof(cmd));

   // Waiting for transmission time
   // seems to be longer for clearing board signal state
   usleep(10000);

   unsigned char response[50];
   read_from_serial(s->serial_port, response, 0);

   // Error Handling
   // Check for errros
   if(strcmp(response,"OK"))
   {
      printf("Error on SET MAXIMUM CURRENT COMMAND\n");
      exit(-41);
   }

   read_board_status(s);
   return ;
}
