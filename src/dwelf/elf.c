#include "../../include/dwelf/elf.h"
#include "../../lib/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>


static void verify_header(const Elf64_Ehdr* header);
static void print_header(const Elf64_Ehdr* header);


void read_header(char* fname) {
  FILE* f = fopen(fname, "rb");

  if (f == NULL)
    PANIC("Could not open file: %s", fname);

  Elf64_Ehdr header;
  
  fread(&header, 1, sizeof(Elf64_Ehdr), f);
  
  fclose(f);
  
  verify_header(&header);
}


static void verify_header(const Elf64_Ehdr* header) {
  
  //print_header(header);

  if (header->e_ident[EI_MAG0] != ELFMAG0 || header->e_ident[EI_MAG1] != ELFMAG1 || 
      header->e_ident[EI_MAG2] != ELFMAG2 || header->e_ident[EI_MAG3] != ELFMAG3)
    PANIC("Invalid ELF file");

    else if (header->e_ident[EI_CLASS] != ELFCLASS64)
    PANIC("Unsupported ELF class (must be 64 bit)");

  else if (header->e_ident[EI_DATA] != ELFDATA2LSB)
    PANIC("Unsupported ELF byte order (must be little endian)");

  else if (header->e_ident[EI_OSABI] != ELFOSABI_SYSV && header->e_ident[EI_OSABI] != ELFOSABI_LINUX)
    PANIC("Unsupported ELF OS ABI (must be SYSV/Linux)");
    
  else if (header->e_type != ET_EXEC && header->e_type != ET_DYN)
    PANIC("Unsupported ELF file type (must be executable)");
  
  else if (header->e_machine != EM_X86_64)
    PANIC("Unsupported ELF architecture (must be EM_X86_64)");

  else if (header->e_version != EV_CURRENT)
    PANIC("Unsupported ELF version (must be 1)");

  else if (header->e_entry == 0)
    PANIC("ELF entry point is NULL");

  else if (header->e_ehsize != sizeof(Elf64_Ehdr))
    PANIC("Invalid ELF header size");

  else if (header->e_phentsize != sizeof(Elf64_Phdr))
    PANIC("Invalid program header entry size");

  else if (header->e_shentsize != sizeof(Elf64_Shdr))
    PANIC("Invalid section header entry size");

}



static void print_header(const Elf64_Ehdr* header) {
  printf(
    "e_ident: %.4s\n"
    "e_type: %u\n"
    "e_machine: %u\n"
    "e_version: %u\n"
    "e_entry: 0x%lx\n"
    "e_phoff: %lu\n"
    "e_shoff: %lu\n"
    "e_flags: %u\n"
    "e_ehsize: %u\n"
    "e_phentsize: %u\n"
    "e_phnum: %u\n"
    "e_shentsize: %u\n"
    "e_shnum: %u\n"
    "e_shstrndx: %u\n",

    header->e_ident,
    header->e_type,
    header->e_machine,
    header->e_version,
    header->e_entry,
    header->e_phoff,
    header->e_shoff,
    header->e_flags,
    header->e_ehsize,
    header->e_phentsize,
    header->e_phnum,
    header->e_shentsize,
    header->e_shnum,
    header->e_shstrndx
  );
}