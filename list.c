/*
 * list.c - (C) David Riesz 2022
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

#include "rigsync.h"

struct rs_rig *rig_list = NULL;
static int rig_count_value = 0;
static int rig_sync_init_value = 0;

int rig_count() { return (rig_count_value); }
struct rs_rig *get_rig(int val) { return &(rig_list[val]); }

struct rs_rig *add_rig(rig_model_t model)
{
  rig_list = (struct rs_rig *)realloc(rig_list, sizeof(struct rs_rig)*(rig_count_value+1));
  memset(&(rig_list[rig_count_value]), 0, sizeof(struct rs_rig));
  rig_list[rig_count_value].model = model;
  rig_list[rig_count_value].rig = rig_init(model);
  rig_list[rig_count_value].number = rig_count_value;
  rig_count_value++;
  return &(rig_list[rig_count_value-1]);
}

void set_rig_port(struct rs_rig *rig, const char *portstr)
{
  if(rig->portstr) { free(rig->portstr); }
  rig->portstr = (char *)malloc(strlen(portstr) + 1);
  strcpy(rig->portstr, portstr);
  strncpy(rig->rig->state.rigport.pathname, portstr, HAMLIB_FILPATHLEN - 1);
}

void set_rig_speed(struct rs_rig *rig, int speed)
{
  rig->speed = speed;
  rig->rig->state.rigport.parm.serial.rate = speed;
}

void set_rig_sync_init(int val)
{
  rig_sync_init_value = val;
}

int get_rig_sync_init() { return rig_sync_init_value; }

void test_rigs()
{
  int ii, retval;

  for(ii=0 ; ii<rig_count() ; ii++)
  {
    retval = rig_open(rig_list[ii].rig);

    if(retval != RIG_OK)
    {
      fprintf(stderr, "*** Could not open rig %d: %s\n", ii+1, rigerror(retval));
      exit(1);
    }
    
    gprintf(1, "opened rig: model %d, port %s, speed %d\n", rig_list[ii].model, rig_list[ii].portstr, rig_list[ii].speed);
  }
}
