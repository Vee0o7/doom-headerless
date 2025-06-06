//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Fixed point implementation.
//



#include "include.h"

#include "doomtype.h"
#include "i_system.h"

#include "m_fixed.h"




// Fixme. __USE_C_FIXED__ or something.

// FRACBITS are 16

__attribute__((noinline))
fixed_t
FixedMul
( fixed_t	a,
  fixed_t	b )
{
	uint32_t ua_l = a & 0xffff;
	uint32_t ua_h = a >> 16;
	uint32_t ub_l = b & 0xffff;
	uint32_t ub_h = b >> 16;

	return ub_h * a + (ub_l * ua_l >> 16) + ub_l * ua_h;
    //return ((uint64_t) a * (uint64_t) b) >> FRACBITS;
}



//
// FixedDiv, C version.
//

__attribute__((noinline))
fixed_t FixedDiv(fixed_t a, fixed_t b)
{
    if ((abs(a) >> 14) >= abs(b))
    {
	return (a^b) < 0 ? INT_MIN : INT_MAX;
    }
    else
    {
	/*int64_t result;

	result = ((int64_t) a << 16) / b;
	return (fixed_t) result;*/

	uint32_t ua = abs(a);
	uint32_t ub = abs(b);
	int s = (a < 0) != (b < 0);

	uint32_t r_h = ua / ub;
	ua -= r_h * ub;

	// there is probably some better algorithm, but this is good enough
	uint32_t ur = (r_h << 16);
	if (ub < 0x10000) {
		ur += (ua << 16) / ub;
	} else if (ub < 0x1000000) {
		ua <<= 8;
		uint32_t r_m = ua / ub;
		ur += r_m << 8;
		ua -= r_m * ub;
		ur += (ua << 8) / ub;
	} else if (ub < 0x10000000) {
		uint32_t r_m = 0;
		for (uint32_t i = 4; i-- > 0;) {
			ua <<= 4;
			r_m = ua / ub;
			ur += r_m << (i << 2);
			ua -= r_m * ub;
		}
	} else if (ub < 0x40000000) {
		uint32_t r_m = 0;
		for (uint32_t i = 8; i-- > 0;) {
			ua <<= 2;
			r_m = ua / ub;
			ur += r_m << (i << 1);
			ua -= r_m * ub;
		}
	} else {
		uint32_t r_m = 0;
		for (uint32_t i = 16; i-- > 0;) {
			ua <<= 1;
			r_m = ua / ub;
			ur += r_m << i;
			ua -= r_m * ub;
		}
	}
	if (s) return -ur;
	return ur;

    }
}

