/*
 *	$Id$
 *	int module interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2007-10, IRIT UPS.
 *
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_INT_H_
#define ELM_INT_H_

#include <stdint.h>

namespace elm { namespace t {

// base types
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

// other types
#ifndef __LP64__
	typedef uint32 size;
	typedef uint32 offset;
	typedef uint32 uint;
	typedef uint32 intptr;
#else
	typedef uint64 size;
	typedef uint64 offset;
	typedef uint64 uint;
	typedef uint64 intptr;
#endif
}	// t

// prototypes
int msb32(t::uint32 i);
inline t::uint32 roundup(t::uint32 v, t::uint32 m) { return ((v - 1) / m + 1) * m; }
inline t::uint32 rounddown(t::uint32 v, t::uint32 m) { return v / m * m; }

} // elm

#endif /* ELM_INT_H_ */
