#ifndef __SEGMENT_H__
#define __SEGMENT_H__
/*	
 *  Copyright (C) 2012
 *	"Mu Lei" known as "NalaGinrut" <NalaGinrut@gmail.com>
 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// universal segment type
#define USEG_X		0x8	    // Executable segment
#define USEG_E		0x4	    // Expand down (non-executable segments)
#define USEG_C		0x4	    // Conforming code segment (executable only)
#define USEG_W		0x2	    // Writeable (non-executable segments)
#define USEG_R		0x2	    // Readable (executable segments)
#define USEG_A		0x1	    // Accessed

#ifdef __ASSEMBLER__
/*
 * Macros to build GDT entries in assembly.
 */
#define SEG_NULL                                                \
        .word 0, 0;                                             \
        .byte 0, 0, 0, 0

// SET_INIT is just used for P_MODE during BOOT time;
#define SEG_INIT(type ,base ,lim)                               \
        .word (((lim) >> 12) & 0xffff), ((base) & 0xffff);      \
        .byte (((base) >> 16) & 0xff), (0x90 | (type)),         \
	       (0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)
#endif // __ASSEMBLER__

#endif // End of __SEGMENT_H__;
