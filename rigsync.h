/*
 * rigsync.h - (C) David Riesz 2022
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

#ifndef __RIGSYNC_H__
#define __RIGSYNC_H__

// note Ubuntu 20.04 uses hamlib 3.3
#include <hamlib/rig.h>

// Hamlib 4.x renames FILEPATHLEN to HAMLIB_FILEPATHLEN
#ifndef HAMLIB_FILPATHLEN
#define HAMLIB_FILPATHLEN FILPATHLEN
#endif

struct rs_rig_state
{
  freq_t freq;
  rmode_t mode;
  pbwidth_t width;
};

struct rs_rig
{
  RIG *rig;
  rig_model_t model;
  char *portstr;
  int speed;
  struct rs_rig_state state;
  int dirty;
  int number;
};

/* args.c */
void read_args(int argc, char **argv);

/* list.c */
int rig_count();
struct rs_rig *get_rig(int val);
struct rs_rig *add_rig(rig_model_t model);
void set_rig_port(struct rs_rig *rig, const char *portstr);
void set_rig_speed(struct rs_rig *rig, int speed);
void test_rigs();
void set_rig_sync_init(int val);
int get_rig_sync_init();

/* sync.c */
void rig_sync();

/* main.c */
int finished();

/* debug.c */
#if DEBUG
void set_debug_level(int level);
int get_debug_level();
int gprintf(int level, const char *fmt, ...);
#else
#define gprintf() 0
#endif


#endif /* __RIGSYNC_H__ */
