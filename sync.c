/*
 * sync.c - (C) David Riesz 2022
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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "rigsync.h"

void rig_read_state(struct rs_rig *rig);
void rig_write_state(struct rs_rig *rig, struct rs_rig_state *state);
void rig_sync_state(struct rs_rig *rig_dest, struct rs_rig *rig_src);

void rig_sync()
{
  int ii, nn, update_rig_no;
  struct rs_rig *dirty_rig, *cur_rig;

  nn = rig_count();

  // read initial states and clear dirty flags
  for(ii=0 ; ii<nn ; ii++)
  {
    cur_rig = get_rig(ii);
    gprintf(1, "rig %d: reading initial state\n", ii);
    rig_read_state(cur_rig);
    gprintf(2, "        clearing dirty flag\n");
    cur_rig->dirty = 0;
  }

  gprintf(1, "reading states again\n");
  for(ii=0 ; ii<nn ; ii++) { rig_read_state(get_rig(ii)); }

  // set dirty flag on startup sync rig
  update_rig_no = get_rig_sync_init();
  cur_rig = get_rig(update_rig_no);
  rig_read_state(cur_rig);
  cur_rig->dirty = 1;

  // When this loop starts, it will find that
  // only the startup rig has the dirty flag set.
  // The other rigs will be synced to it in the
  // first cycle.
  for( ; ; )
  {
    // first, find the first updated (dirty) rig
    for(dirty_rig=NULL, ii=0 ; ii<nn ; ii++)
    {
      cur_rig = get_rig(ii);
      if(cur_rig->dirty)
      {
        dirty_rig = cur_rig;
        gprintf(1, "dirty rig: %d\n", dirty_rig?dirty_rig->number:-1);
        break;
      }
    }

    // second, sync other rigs to dirty rig
    for(ii=0 ; dirty_rig!=NULL && ii<nn ; ii++)
    {
      cur_rig = get_rig(ii);
      if(cur_rig != dirty_rig)
      {
        rig_sync_state(cur_rig, dirty_rig);
      }
    }
    if(dirty_rig != NULL) { dirty_rig->dirty = 0; }

    // third, find new dirty rigs
    for(ii=0 ; ii<nn ; ii++) { rig_read_state(get_rig(ii)); }
    usleep(10000); /* 10 milliseconds */
//    return;
  }
}

void rig_read_state(struct rs_rig *rig)
{
  freq_t ff;
  rmode_t mm;
  pbwidth_t ww;

  rig_get_freq(rig->rig, RIG_VFO_CURR, &ff);
  rig_get_mode(rig->rig, RIG_VFO_CURR, &mm, &ww);
  
  if((ff != rig->state.freq) || (mm != rig->state.mode))
  {
    rig->dirty = 1;
    gprintf(2, "rig %d is dirty: old=%'15.2fHz/%3s new=%'15.2fHz/%3s\n",
      rig->number,
      rig->state.freq, rig_strrmode(rig->state.mode), ff, rig_strrmode(mm));
  }
  rig->state.freq = ff;
  rig->state.mode = mm; 
  rig->state.width = ww;
}

void rig_write_state(struct rs_rig *rig, struct rs_rig_state *new_state)
{
  struct rs_rig_state *cur_state = &(rig->state);

  rig_set_freq(rig->rig, RIG_VFO_CURR, (new_state->freq));
  rig_set_mode(rig->rig, RIG_VFO_CURR, (new_state->mode), (cur_state->width));

  rig_read_state(rig);
  cur_state = &(rig->state);

  gprintf(1, "rig_write_state() req=%'15.2fHz/%3s act=%'15.2fHz/%3s\n",
    new_state->freq, rig_strrmode(new_state->mode),
    cur_state->freq, rig_strrmode(cur_state->mode));

  rig->dirty = 0;
}

void rig_sync_state(struct rs_rig *rig_dest, struct rs_rig *rig_src)
{
  rig_write_state(rig_dest, &(rig_src->state));
}


