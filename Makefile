# Makefile - (C) David Riesz 2022
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published
#   by the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with this program; if not, write to the Free Software Foundation,
#   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

PROGRAM = rigsync

CSOURCES = main.c args.c list.c sync.c debug.c
COBJECTS = $(CSOURCES:.c=.o)

CFLAGS  = $(COPT) $(CDEF) $(CINC)
COPT    = -O2 -g
CDEF    = -DDEBUG
CINC    =

LDFLAGS = $(LDOPT) $(LDDIR) $(LDLIB)
LDOPT   =
LDDIR   =
LDLIB   = -lhamlib

all: $(PROGRAM)

$(PROGRAM): $(COBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c rigsync.h
	$(CC) -c -o $@ $(CFLAGS) $<

clean: objclean execlean
objclean:
	$(RM) $(COBJECTS)
execlean:
	$(RM) $(PROGRAM)

run1: $(PROGRAM)
	./$< \
		-m 2 -r localhost:4532 \
		-m 210 -r /dev/ttyF1 -s 57600

run2: $(PROGRAM)
	./$< \
		-m 2 -r localhost:4532 \
		-m 210 -r /dev/ttyF1 -s 57600 \
		-m 122 -r /dev/ttyF0 -s 38400 \
		-n 2

run3: $(PROGRAM)
	./$< \
		-m 2 -r localhost:4532 \
		-m 122 -r /dev/ttyF0 -s 38400 \
		-n 2
