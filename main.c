/*
 * main.c - (C) David Riesz 2022
 *
 * This program test/control a radio using Hamlib.
 * It takes commands in interactive mode as well as
 * from command line options.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this program; if not, write to the Free Software Foundation,
 *   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

#include "rigsync.h"

#if 0
struct rs_rig_state { freq_t freq; rmode_t mode; pbwidth_t width; };
void   rs_rig_read_state (RIG *rig, struct rs_rig_state *state);
void   rs_rig_write_state(RIG *rig, struct rs_rig_state *state);
int    rs_rig_state_cmp(struct rs_rig_state *statea, struct rs_rig_state *stateb);
void   rs_rig_state_cpy(struct rs_rig_state *state_dest, struct rs_rig_state *state_orig);

//#define RIG_A_MODEL RIG_MODEL_FT857
#define RIG_A_MODEL RIG_MODEL_TS870S
#define RIG_A_SPEED 38400
#define RIG_A_PORT  "/dev/ttyF1"

#define RIG_B_MODEL RIG_MODEL_NETRIGCTL /* rigctl server */
#define RIG_B_PORT "localhost:4532"
#endif /* 0 */

int main(int argc, char **argv)
{
  setlocale(LC_NUMERIC, ""); // why isn't this working?
  printf("abc: %'15.1f\n", 12345067890.0);
  rig_set_debug(RIG_DEBUG_NONE);
  read_args(argc, argv);
  rig_sync();
  return 0;
}


#if 0
int xmain(int argc, char **argv)
{
  RIG *riga, *rigb;
  rig_model_t modela, modelb;
  hamlib_port_t porta, portb;
  struct rs_rig_state statea, stateb;
  struct rs_rig_state stateap, statebp;
  char str[1024];
  int retval;
  int startup = 1;

  setlocale(LC_NUMERIC, "");

  rig_set_debug(RIG_DEBUG_NONE);
  
  read_args(argc, argv);
  exit(0);

  modela = RIG_A_MODEL;
  modelb = RIG_B_MODEL;

  riga = rig_init(modela);
  rigb = rig_init(modelb);

  riga->state.rigport.parm.serial.rate = RIG_A_SPEED;
  strncpy(riga->state.rigport.pathname, RIG_A_PORT, HAMLIB_FILPATHLEN - 1);
  strncpy(rigb->state.rigport.pathname, RIG_B_PORT, HAMLIB_FILPATHLEN - 1);

  retval = rig_open(riga);
  if(retval != RIG_OK) { fprintf(stderr, "*** Could not open FT857.\n"); exit(1); }
  retval = rig_open(rigb);
  if(retval != RIG_OK) { fprintf(stderr, "*** Could not oien NETRIGCTL.\n"); exit(1); }

//  fprintf(stderr, "*** EVERYTHING OKAY 1\n");

  printf("          RIG A           ");
  printf(" ");
  printf("          RIG B           ");
  printf("\n");
  printf("==========================");
  printf(" ");
  printf("==========================");
  printf("\n");

  memset(&statea, 0, sizeof(struct rs_rig_state));
  memset(&stateb, 0, sizeof(struct rs_rig_state));
  memset(&stateap, 0, sizeof(struct rs_rig_state));
  memset(&statebp, 0, sizeof(struct rs_rig_state));

  for ( ; ; )
  {
    rs_rig_read_state(riga, &statea);
    rs_rig_read_state(rigb, &stateb);
    
    if(rs_rig_state_cmp(&statea, &stateap))
    {
      rs_rig_write_state(rigb, &statea);
    }
    else if(rs_rig_state_cmp(&stateb, &statebp))
    {
      rs_rig_write_state(riga, &stateb);
    }
    rs_rig_read_state(riga, &stateap);
    rs_rig_read_state(rigb, &statebp);
    printf( "%'15.1f Hz %-7s", statea.freq, rig_strrmode(statea.mode));
    printf(" %'15.1f Hz %-7s", stateb.freq, rig_strrmode(stateb.mode));
    printf("\r");
    fflush(stdout);
    usleep(10000);
  }

  rig_close(riga);
  rig_close(rigb);

  rig_cleanup(riga);
  rig_cleanup(rigb);

  rig_sync();

  return 0;
}

static int _finished = 0;     
int finished() { return _finished; }

void rs_rig_read_state(RIG *rig, struct rs_rig_state *state)
{
  rig_get_freq(rig, RIG_VFO_CURR, &(state->freq));
  rig_get_mode(rig, RIG_VFO_CURR, &(state->mode), &(state->width));
}

void rs_rig_write_state(RIG *rig, struct rs_rig_state *state)
{
  struct rs_rig_state curstate;
  rs_rig_read_state(rig, &curstate);
  rig_set_freq(rig, RIG_VFO_CURR, (state->freq));
  rig_set_mode(rig, RIG_VFO_CURR, (state->mode), (curstate.width));
}

int rs_rig_state_cmp(struct rs_rig_state *statea, struct rs_rig_state *stateb)
{
  if(statea->freq != stateb->freq) { return 1; }
  if(statea->mode != stateb->mode) { return 2; }
  return 0;
}

void rs_rig_state_cpy(struct rs_rig_state *state_dest, struct rs_rig_state *state_orig)
{
  state_dest->freq = state_orig->freq;
  state_dest->mode = state_orig->mode;
}
#endif /* 0 */
