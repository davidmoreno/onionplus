/**
		onion+ - web small modular server.
		Copyright (C) 2007-2010 David Moreno Montero

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU Affero General Public License as
		published by the Free Software Foundation, either version 3 of the
		License, or (at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU Affero General Public License for more details.

		You should have received a copy of the GNU Affero General Public License
		along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <libgen.h>

/// Set to true to enable all debugging.
extern char ONION_DEBUG;

#define DEBUG(...) { if (ONION_DEBUG){ fprintf(stderr,"\033[01;34mDEBUG %s:%d ",basename((char*)__FILE__),__LINE__); fprintf(stderr,__VA_ARGS__); fprintf(stderr,"\033[0m\n"); } }
#define ERROR(...) { fprintf(stderr,"\033[01;31mERROR %s:%d ",basename((char*)__FILE__),__LINE__); fprintf(stderr,__VA_ARGS__); fprintf(stderr,"\033[0m\n"); }
#define LOG(...) { fprintf(stderr,"LOG %s:%d \033[02;32m",basename((char*)__FILE__),__LINE__); fprintf(stderr,__VA_ARGS__); fprintf(stderr,"\033[0m\n"); }
#define WARNING(...) { fprintf(stderr,"\033[01;33mWARNING %s:%d ",basename((char*)__FILE__),__LINE__); fprintf(stderr,__VA_ARGS__); fprintf(stderr,"\033[0m\n"); }

#endif
