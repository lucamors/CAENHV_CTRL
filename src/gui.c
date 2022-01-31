#include "gui.h"
#include "caen_usb_protocol.h"

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
   sprintf(monitor_string,"VSET: %.2f", s->vset);
   gtk_label_set_text(GTK_LABEL(label_vset), (const gchar *) monitor_string);

   // Update ISET
   sprintf(monitor_string,"ISET: %.2f", s->iset);
   gtk_label_set_text(GTK_LABEL(label_iset), (const gchar *) monitor_string);

   // Update VMON
   sprintf(monitor_string,"Voltage: %.2f", s->voltage_level);
   gtk_label_set_text(GTK_LABEL(label_voltage_level), (const gchar *) monitor_string);

   // Update IMON
   sprintf(monitor_string,"Current: %.2f", s->current_level);
   gtk_label_set_text(GTK_LABEL(label_current_level), (const gchar *) monitor_string);

   // Update RAMPUP
   sprintf(monitor_string,"RAMP UP: %.2f", s->ramp_up);
   gtk_label_set_text(GTK_LABEL(label_ramp_up), (const gchar *) monitor_string);

   // Update RAMPDOWN
   sprintf(monitor_string,"RAMP DOWN: %.2f", s->ramp_down);
   gtk_label_set_text(GTK_LABEL(label_ramp_down), (const gchar *) monitor_string);

   // Update TRIP
   sprintf(monitor_string,"TRIP: %.2f", s->trip);
   gtk_label_set_text(GTK_LABEL(label_trip), (const gchar *) monitor_string);

   // Update switch buttons
   // not supported yet
   return ;
}

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
