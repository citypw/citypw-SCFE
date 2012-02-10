#ifndef __IO_H
#define __IO_H
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

#include "boot_main.h"

// get data operate;
static __inline__ __u8_t port_rb(__u32_t port) __true_inline;
static __inline__ __u16_t port_rw(__u32_t port) __true_inline;
static __inline__ __u32_t port_rl(__u32_t port) __true_inline;

static __inline__ void port_rnb(__u32_t port,
				__gptr_t addr,
				__u32_t cnt) __true_inline;

static __inline__ void port_rnw(__u32_t port,
				__gptr_t addr,
				__u32_t cnt) __true_inline;

static __inline__ void port_rnl(__u32_t port,
				__gptr_t addr,
				__u32_t cnt) __true_inline;


// output data operate;
static __inline__ void port_wb(__u32_t port ,__u8_t data) __true_inline;
static __inline__ void port_ww(__u32_t port ,__u16_t data) __true_inline;
static __inline__ void port_wl(__u32_t port ,__u32_t data) __true_inline;
static __inline__ void port_wnb(__u32_t port,
				__cptr_t addr,
				__u32_t cnt) __true_inline;
static __inline__ void port_wnw(__u32_t port,
				__cptr_t addr,
				__u32_t cnt) __true_inline;
static __inline__ void port_wnl(__u32_t port,
				__cptr_t addr,
				__u32_t cnt) __true_inline;



// you'd better explicitly cast while calling "port_w" directly
#define __port_write(port ,data)			\
  do{ 							\
    __asm__ __volatile__("out%z0 %0 ,%w1"		\
			 :				\
			 :"a" (data) ,"d" (port)	\
			 );				\
  }while(0);

#define __port_nwrite(cmd)					\
  do{								\
    __asm__ __volatile__("cld\n\t"				\
			 "repne\n\t"				\
			 #cmd					\
			 :"=S" (addr) ,"=c" (cnt)		\
			 :"d" (port) ,"0" (addr) ,"1" (cnt)	\
			 :"cc"					\
			 );					\
  }while(0);


#define __port_read(port ,data)		\
  do{					\
  __asm__ __volatile__("in%z0 %w1 ,%0"	\
		       :"=a" (data)	\
		       :"d" (port)	\
		       );		\
  }while(0);

#define __port_nread(cmd)					\
  do{								\
    __asm__ __volatile__("cld\n\t"				\
			 "repne\n\t"				\
			 #cmd					\
			 :"=D" (addr) ,"=c" (cnt)		\
			 :"d" (port) ,"0" (addr) ,"1" (cnt)	\
			 :"memory" ,"cc"			\
			 );					\
  }while(0);

/* Actually we don't need these "cast". I wrote them in case
 * some Muggle who can't "cast" ask why thay are similar. :-)
 */
static __inline__ __u8_t port_rb(__u32_t port)
{
  __u8_t data;
  __port_read(port ,(__u8_t)data);
  
  return data;
}

static __inline__ __u16_t port_rw(__u32_t port)
{
  __u16_t data;
  __port_read(port ,(__u16_t)data);
  
  return data;
}

static __inline__ __u32_t port_rl(__u32_t port)
{
  __u32_t data;
  __port_read(port ,(__u32_t)data);
  
  return data;
}


static __inline__ void port_rnb(__u32_t port ,__gptr_t addr ,__u32_t cnt)
{
  __port_nread(insb);
}

static __inline__ void port_rnw(__u32_t port ,__gptr_t addr ,__u32_t cnt)
{
  __port_nread(insw);
}

static __inline__ void port_rnl(__u32_t port ,__gptr_t addr ,__u32_t cnt)
{
  __port_nread(insl);
}

static __inline__ void port_wb(__u32_t port ,__u8_t data)
{
  __port_write(port ,(__u8_t)data);
}

static __inline__ void port_ww(__u32_t port ,__u16_t data)
{
  __port_write(port ,(__u16_t)data);
}

static __inline__ void port_wl(__u32_t port ,__u32_t data)
{
  __port_write(port ,(__u32_t)data);
}

static __inline__ void port_wnb(__u32_t port,
				__cptr_t addr,
				__u32_t cnt)
{
  __port_nwrite(outsb);
}

static __inline__ void port_wnw(__u32_t port,
				__cptr_t addr,
				__u32_t cnt)
{
  __port_nwrite(outsw);
}

static __inline__ void port_wnl(__u32_t port,
				__cptr_t addr,
				__u32_t cnt)
{
  __port_nwrite(outsl);
}

 



#endif // End of __IO_H;