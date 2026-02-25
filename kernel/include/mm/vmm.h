#ifndef VMM_H
#define VMM_H

#include <stdint.h>
#include <mm/mmu.h>

void vmm_init(void);
page_directory_t* vmm_create_user_page_directory(void);
void vmm_destroy_user_page_directory(page_directory_t* page);
void* vmm_va2pa(page_directory_t* pgdir, uint8_t* va);
void vmm_switch_kernel_page_directory(void);
void vmm_switch_user_page_directory(page_directory_t* pgdir);

#endif
