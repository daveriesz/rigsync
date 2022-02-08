#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#include "rigsync.h"

void read_args(int argc, char **argv);
void msleep(int ms);
void nsleep(int ns);

struct rs_rig_state { freq_t freq; rmode_t mode; pbwidth_t width; };
void rs_rig_read_state (RIG *rig, struct rs_rig_state *state);
void rs_rig_write_state(RIG *rig, struct rs_rig_state *state);
int rs_rig_state_cmp(struct rs_rig_state *statea, struct rs_rig_state *stateb);

int main(int argc, char **argv)
{
  RIG *riga, *rigb;
  rig_model_t modela, modelb;
  hamlib_port_t porta, portb;
  struct rs_rig_state statea, stateb;
  struct rs_rig_state statep;
  char str[1024];
  int retval;
  int startup = 1;

  setlocale(LC_NUMERIC, "");

  rig_set_debug(RIG_DEBUG_NONE);
  
  read_args(argc, argv);

  modela = RIG_MODEL_NETRIGCTL; /* rigctl server  */
  modelb = RIG_MODEL_FT857;     /* Yaesu FT-857 */

  riga = rig_init(modela);
  rigb = rig_init(modelb);

  strncpy(riga->state.rigport.pathname, "localhost:4532", FILPATHLEN - 1);

  rigb->state.rigport.parm.serial.rate = 38400;
  strncpy(rigb->state.rigport.pathname, "/dev/ttyUSB0", FILPATHLEN - 1);

  retval = rig_open(riga);
  if(retval != RIG_OK) { fprintf(stderr, "*** Could not open NETRIGCTL.\n"); exit(1); }
  retval = rig_open(rigb);
  if(retval != RIG_OK) { fprintf(stderr, "*** Could not oien FT857.\n"); exit(1); }

//  fprintf(stderr, "*** EVERYTHING OKAY 1\n");

  printf("          RIG A           ");
  printf(" ");
  printf("          RIG B           ");
  printf("\n");
  printf("==========================");
  printf(" ");
  printf("==========================");
  printf("\n");

  rs_rig_read_state(riga, &statea);
  rs_rig_read_state(rigb, &stateb);
  if(memcmp(&statea, &stateb, sizeof(struct rs_rig_state)))
  {
    rs_rig_write_state(riga, &stateb);
  }
  rs_rig_state_cmp(&statep, &statea);

  for ( ; ; )
  {
    rs_rig_read_state(riga, &statea);
    rs_rig_read_state(rigb, &stateb);

    {
      if(rs_rig_state_cmp(&statea, &statep))
      {
//        printf("\nrig a changed\n");
        rs_rig_write_state(rigb, &statea);
      }
      else if(rs_rig_state_cmp(&stateb, &statep))
      {
//        printf("\n");
//        printf("rig b changed: b= %'15.1f/%7s/%ld\n", stateb.freq, rig_strrmode(stateb.mode), stateb.width);
//        printf("               p= %'15.1f/%7s/%ld\n", statep.freq, rig_strrmode(statep.mode), statep.width);
        rs_rig_write_state(riga, &stateb);
      }
    }

    printf( "%'15.1f Hz %-7s", statea.freq, rig_strrmode(statea.mode));
    printf(" %'15.1f Hz %-7s", stateb.freq, rig_strrmode(stateb.mode));
    printf("\r");
    fflush(stdout);
    memcpy(&statep, &statea, sizeof(struct rs_rig_state));
    msleep(2000);
  }
  printf("\n");

  rig_close(riga);
  rig_close(rigb);

  rig_cleanup(riga);
  rig_cleanup(rigb);

  rig_sync();

  return 0;
}

void read_args(int argc, char **argv)
{
  int ii;
  for(ii=0 ; ii<argc ; ii++)
  {
#if 0
    if(!(strcmp(argv[ii], "-t")))
    {
      ii++;
      threads = atoi(argv[ii]);
    }
    else if(!(strcmp(argv[ii], "-d")))
    {
      ii++;
      dims = atoi(argv[ii]);
    }    
    printf("argv[%d] = \"%s\"\n", ii, argv[ii]);
#endif
  }
}

static int _finished = 0;     
int finished() { return _finished; }

void nsleep(int ns)
{
  struct timespec ts;
  ts.tv_nsec = ns;
  ts.tv_sec = 0;
  nanosleep(&ts, NULL);
}

void msleep(int ms) { return nsleep(ms * 1000000); }

void rs_rig_read_state(RIG *rig, struct rs_rig_state *state)
{
  rig_get_freq(rig, RIG_VFO_CURR, &(state->freq));
  rig_get_mode(rig, RIG_VFO_CURR, &(state->mode), &(state->width));
}

void rs_rig_write_state(RIG *rig, struct rs_rig_state *state)
{
  rig_set_freq(rig, RIG_VFO_CURR, (state->freq));
  rig_set_mode(rig, RIG_VFO_CURR, (state->mode), (state->width));
}

int rs_rig_state_cmp(struct rs_rig_state *statea, struct rs_rig_state *stateb)
{
  if(statea->freq != stateb->freq) { return 1; }
  if(statea->mode != stateb->mode) { return 2; }
  return 0;
}

