#include "../../include/dwelf/elf.h"
#include "../../include/debug.h"
#include <stdlib.h>



static void verify_ehdr(const Elf64_Ehdr* ehdr);
static void print_ehdr(const Elf64_Ehdr* ehdr);





ElfFile* elf_init(char* fname) {
  
  FILE* f = fopen(fname, "rb");
  if (f == NULL)
    PANIC("Could not open file: %s", fname);


  const Elf64_Ehdr* ehdr  = read_ehdr(f);
  const Elf64_Phdr* phdr   = read_phdr(ehdr, f);
  const Elf64_Shdr* shdr   = read_shdr(ehdr, f);

}




const Elf64_Ehdr* read_ehdr(FILE* f) {
  Elf64_Ehdr* ehdr = malloc(sizeof(Elf64_Ehdr));
  if (ehdr == NULL)
    PANIC("Memory allocation for ELF header failed");
  
  if (fread(ehdr, 1, sizeof(Elf64_Ehdr), f) != sizeof(Elf64_Ehdr))
    PANIC("Failed to read complete ELF header");
  
  verify_ehdr(ehdr);

  return ehdr;
}



const Elf64_Phdr* read_phdr(const Elf64_Ehdr* ehdr, FILE* f) {
  fseek(f, ehdr->e_phoff, SEEK_SET);

  Elf64_Phdr* phdr = malloc(ehdr->e_phnum * ehdr->e_phentsize);
  
  if (phdr == NULL)
    PANIC("Memory allocation for ELF program headers failed");

  if (fread(phdr, ehdr->e_phentsize, ehdr->e_phnum, f) != ehdr->e_phnum)
    PANIC("Failed to read all program headers");
  
  return phdr;
}



const Elf64_Shdr* read_shdr(const Elf64_Ehdr* ehdr, FILE* f) {
  fseek(f, ehdr->e_shoff, SEEK_SET);
  
  Elf64_Shdr* shdr = malloc(ehdr->e_shnum * ehdr->e_shentsize);

  if (shdr == NULL)
    PANIC("Memory allocation for ELF section headers failed");

  if (fread(shdr, ehdr->e_shentsize, ehdr->e_shnum, f) != ehdr->e_shnum)
    PANIC("Failed to read all section headers");

  return shdr;
}









static void verify_ehdr(const Elf64_Ehdr* ehdr) {
  

  if (ehdr->e_ident[EI_MAG0] != ELFMAG0 || ehdr->e_ident[EI_MAG1] != ELFMAG1 || 
      ehdr->e_ident[EI_MAG2] != ELFMAG2 || ehdr->e_ident[EI_MAG3] != ELFMAG3)
    PANIC("Invalid ELF file");

  else if (ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    PANIC("Unsupported ELF class (must be 64 bit)");

  else if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB)
    PANIC("Unsupported ELF byte order (must be little endian)");

  else if (ehdr->e_ident[EI_OSABI] != ELFOSABI_SYSV && ehdr->e_ident[EI_OSABI] != ELFOSABI_LINUX)
    PANIC("Unsupported ELF OS ABI (must be SYSV/Linux)");
    
  else if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
    PANIC("Unsupported ELF file type (must be executable)");
  
  else if (ehdr->e_machine != EM_X86_64)
    PANIC("Unsupported ELF architecture (must be EM_X86_64)");

  else if (ehdr->e_version != EV_CURRENT)
    PANIC("Unsupported ELF version (must be 1)");

  else if (ehdr->e_entry == 0)
    PANIC("ELF entry point is NULL");

  else if (ehdr->e_ehsize != sizeof(Elf64_Ehdr))
    PANIC("Invalid ELF ehdr size");

  else if (ehdr->e_phentsize != sizeof(Elf64_Phdr))
    PANIC("Invalid program ehdr entry size");

  else if (ehdr->e_shentsize != sizeof(Elf64_Shdr))
    PANIC("Invalid section header entry size");

}



static void verify_shdr(const Elf64_Shdr* shdr) {
  if (shdr->sh_type == SHN_UNDEF)
    PANIC("No section type found");
  
  else if (shdr->)

}




static void print_ehdr(const Elf64_Ehdr* ehdr) {
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

    ehdr->e_ident,
    ehdr->e_type,
    ehdr->e_machine,
    ehdr->e_version,
    ehdr->e_entry,
    ehdr->e_phoff,
    ehdr->e_shoff,
    ehdr->e_flags,
    ehdr->e_ehsize,
    ehdr->e_phentsize,
    ehdr->e_phnum,
    ehdr->e_shentsize,
    ehdr->e_shnum,
    ehdr->e_shstrndx
  );
}