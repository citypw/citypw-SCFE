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

#include "elf.h"
#include "boot_main.h"
#include "io.h"

#define BOCHS_DEBUG

void cmain()
{
  
  Elf32_Phdr *prog_head ,*end_prog;
  
  //ptr to const
  const __u32_t *magic = (__u32_t*)(elf_hdr->e_ident);

  read_seg((__u32_t)elf_hdr ,SECTOR*8 ,0);
  
  if( *magic != ELF_MAGIC )
    {
#ifdef BOCHS_DEBUG
      goto bad_elf;
#else
      // TODO: show some error message;
      while(1);
#endif // End ifdef BOCHS_DEBUG;
    }

  prog_head = (Elf32_Phdr *)((__u8_t*)elf_hdr + elf_hdr->e_phoff);
  end_prog = prog_head + elf_hdr->e_phnum;//don't convert

  while( prog_head < end_prog )
    {
      read_seg(prog_head->p_vaddr,
	       prog_head->p_memsz,
	       prog_head->p_offset);
      prog_head++;
    }

  // NOTE: only 24bit addr for now is a trick; 
  ((entry_t)(elf_hdr->e_entry & 0xFFFFFF))();

#ifdef BOCHS_DEBUG
 bad_elf:
      port_ww(0x8A00 ,0x8A00);// enable bochs IO debug;
      port_ww(0x8A00 ,0x8E00);// 
#else

      // TODO: show some error message;

#endif // End ifdef BOCHS_DEBUG;

      while(1);


      // cmain should never return;
}

int read_seg(__u32_t va ,__u32_t count ,__u32_t byte_offset)
{
  __u32_t lba_offset = byte2lba(byte_offset);
  __u32_t addr = down2sect(va & 0xFFFFFF);
  __u32_t sect_cnt = count/SECTOR+1;
 
  read_sect((__u8_t*)addr ,sect_cnt ,lba_offset);
 
  return 0;
}


int read_sect(void *dest ,__u32_t sect_cnt ,__u32_t lba_offset)
{
  
  WAIT_FOR_DISK_READY;

  port_wb(HDC_HEAD ,HEAD(lba_offset) | LBA_PIO_28b);
  port_wb(HDC_SECT_CNT ,sect_cnt);
  port_wb(HDC_SECT_N ,SECT(lba_offset));
  port_wb(HDC_CYL_L ,CY_L(lba_offset));
  port_wb(HDC_CYL_H ,CY_H(lba_offset));
  port_wb(HDC_CMD ,SECT_RR);

  WAIT_FOR_DISK_READY;

  port_rnl(HDC_DATA ,dest ,sect_cnt<<7);

  return 0;
}


