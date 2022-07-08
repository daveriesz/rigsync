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

#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>

#include "rigsync.h"

static int _debug_level = 0;

void set_debug_level(int level)
{
  _debug_level = level;
  gprintf(1, "debug level set: %d\n", _debug_level);
}

int get_debug_level() { return _debug_level; }

/* THIS IS NOT THREAD-SAFE */
#define GBUF_SIZE 131072
static char gbuf[GBUF_SIZE];
int gprintf(int level, const char *fmt, ...)
{
  if(_debug_level == 0) { return 0; }
  if(level > _debug_level) { return 0; }
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(gbuf, GBUF_SIZE, fmt, ap);
  va_end(ap);
  return fprintf(stderr, "DEBUG: %s", gbuf);
}

#endif /* DEBUG */
