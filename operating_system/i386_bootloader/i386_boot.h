#ifndef __I386_BOOT_H
#define __I386_BOOT_H
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



/* These gdt index is one-touch vars, they can be used only in the 
 * real-mode. So they are useless after the bootup;
 * And if you gonnar change them, don't forget they are aligned to 
 * 8-Bytes;
 */
#define PM_CS	0x8	/* code segment selector
			 * NOTE: can't be 0! for 0 is NULL descriptor!
			 */
#define PM_DS 	0x10	// data segment selector


// A20
#define KB_CON_STAT	0x64
#define KB_IS_BUSY	0x02
#define KB_WRITE	0xd1
#define KB_CON_INBUF	0x64
#define A20_EN		0xdf
#define KB_CON_DPORT	0x6
#define CR0_PM	0x01

#endif // End of __I386_BOOT_H;
