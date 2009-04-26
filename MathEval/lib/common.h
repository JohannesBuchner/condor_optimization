/*
 * Copyright (C) 1999, 2002, 2003, 2004  Free Software Foundation, Inc.
 * 
 * This file is part of GNU libmatheval
 * 
 * GNU libmatheval is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later
 * version.
 * 
 * GNU libmatheval is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * program; see the file COPYING. If not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef COMMON_H
#define COMMON_H 1

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

#if STDC_HEADERS
#include <stdlib.h>
#include <string.h>
#elif HAVE_STRINGS_H
#include <strings.h>
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#include "xmalloc.h"

#endif
