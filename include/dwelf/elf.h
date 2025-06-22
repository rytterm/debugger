// For simplification in writing this debugger 
// the elf parser will also only work on x86_64



#ifndef __INCLUDE_DWELF_ELF_H
#define __INCLUDE_DWELF_ELF_H


#ifdef __cplusplus
extern "C" {
#endif

#include <elf.h>
#include <stdio.h>


typedef struct {
  Elf64_Ehdr* ehdr;
  Elf64_Phdr* phdr;
  Elf64_Shdr* shdr;
} ElfFile;


ElfFile* elf_init(char*);


// Read the ELF file header and check
// that the header is valid for linux x86_64
const Elf64_Ehdr* read_ehdr(FILE*);

// Read program headers
const Elf64_Phdr* read_phdr(const Elf64_Ehdr*, FILE*);

// Read section headers
const Elf64_Shdr* read_shdr(const Elf64_Ehdr*, FILE*);


#ifdef __cplusplus
}
#endif

#endif