#
# Makefile for builtin GNU/Guile interpreter
#
#
#   Copyright (C) 2012 Mu Lei known as NalaGinrut <NalaGinrut@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Include some boilerplate Gnu makefile definitions.
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
infodir = ${datarootdir}/info
includedir = ${prefix}/include

datarootdir = ${prefix}/share

topdir = ../../..
BUILD_DIR = /home/nalaginrut/Project/bash-4.1/bash-4.1
srcdir = .
VPATH = .


CC = gcc
RM = rm -f

SHELL = /bin/sh

GUILE = guile
GUILE_CFLAGS = `guile-config compile`
GUILE_LDFLAGS = `guile-config link`

CFLAGS = -g -O2 $(GUILE_CFLAGS)


#
# These values are generated for configure by ${topdir}/support/shobj-conf.
# If your system is not supported by that script, but includes facilities for
# dynamic loading of shared objects, please update the script and send the
# changes to bash-maintainers@gnu.org.
#
SHOBJ_CC = gcc
SHOBJ_CFLAGS = -fPIC
SHOBJ_LD = ${CC}
SHOBJ_LDFLAGS = -shared -Wl,-soname,$@
SHOBJ_XLDFLAGS = 
SHOBJ_LIBS = 
SHOBJ_STATUS = supported

SRC = guile.c
OBJ = guile.o

BUILTIN = big

INC = -I. -I.. -I$(topdir) -I$(topdir)/lib -I$(topdir)/builtins \
      -I$(topdir)/include -I$(BUILD_DIR) -I$(BUILD_DIR)/lib \
      -I$(BUILD_DIR)/builtins


${BUILTIN}:	${OBJ}
	${RM} $@
	${SHOBJ_LD} ${SHOBJ_LDFLAGS} ${SHOBJ_XLDFLAGS} -o $@ ${OBJ} ${GUILE_LDFLAGS} ${SHOBJ_LIBS}

guile.o:	guile.c
	${RM} $@
	$(SHOBJ_CC) $(SHOBJ_CFLAGS) $(CFLAGS) $(INC) -c -o $@ ${srcdir}/guile.c

clean mostlyclean:
	${RM} ${OBJ}
	${RM} ${BUILTIN}

