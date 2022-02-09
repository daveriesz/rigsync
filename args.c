/*
 * args.c - (C) David Riesz 2022
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
#include <string.h>
#include <stdlib.h>

#include "rigsync.h"

void usage(int argc, char **argv);

void read_args(int argc, char **argv)
{
  int ii;
  struct rs_rig *rig = NULL;
  for(ii=1 ; ii<argc ; ii++)
  {
    if(!(strcmp(argv[ii], "-m")))
    {
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      rig = add_rig(atoi(argv[ii]));
    }
    else if(!(strcmp(argv[ii], "-s")))
    {
      if(rig == NULL)
      {
        fprintf(stderr, "Rig definition must begin with a \"-m\" argument.\n");
        exit(1);
      }
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      set_rig_speed(rig, atoi(argv[ii]));
    }
    else if(!(strcmp(argv[ii], "-r")))
    {
      if(rig == NULL)
      {
        fprintf(stderr, "Rig definition must begin with a \"-m\" argument.\n");
        exit(1);
      }
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      set_rig_port(rig, argv[ii]);
    }
    else if(!(strcmp(argv[ii], "-n")))
    {
      if((++ii)>=argc) { fprintf(stderr, "Argument error.\n"); exit(1); }
      set_rig_sync_init(atoi(argv[ii])-1);
      if(get_rig_sync_init() >= rig_count())
      {
        fprintf(stderr, "Sync startup argument error.\n");
        exit(1);
      }
    }
    else if(!(strcmp(argv[ii], "-h")))
    {
      usage(argc, argv);
    }
    else { usage(argc, argv); }
  }
  if(rig_count() < 2)
  {
    fprintf(stderr, "At least two rigs must be defined.\n");
    exit(1);
  }
  printf("rig count = %d\n", rig_count());
  test_rigs();
}


void usage(int argc, char **argv)
{
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "    %s <rig definition> <rig definition> ... [-n <rig number>]\n", argv[0]);
  fprintf(stderr, "        Keep defined rigs in sync.\n");
  fprintf(stderr, "        Each <rig definition> entry must start with:\n");
  fprintf(stderr, "             -m <model number>\n");
  fprintf(stderr, "        and be followed optionally by (depending on the rig):\n");
  fprintf(stderr, "             -r <port/connection>\n");
  fprintf(stderr, "             -s <baud rate>\n");
  fprintf(stderr, "        The \"-n <rig number>\" argument defines which rig in the list\n");
  fprintf(stderr, "        the others are synched to on startup.  The default is the first (1).\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "    %s -h\n", argv[0]);
  fprintf(stderr, "        Print this help message.\n");
  fprintf(stderr, "\n");
  exit(1);
}
