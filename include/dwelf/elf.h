// For simplification in writing this debugger 
// the elf parser will also only work on x86_64




#ifndef _INCLUDE_DWELF_ELF_H
#define _INCLUDE_DWELF_ELF_H


#ifdef __cplusplus
extern "C" {
#endif

// Read the ELF file header and check
// that the header is valid for linux x86_64
void read_header(char* fname);


#ifdef __cplusplus
}
#endif






#endif