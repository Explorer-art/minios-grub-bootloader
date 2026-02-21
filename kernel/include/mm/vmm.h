#ifndef VMM_H
#define VMM_H

#include <stdint.h>
#include <mm/mmu.h>

void vmm_init(void);
page_directory_t* vmm_create_user_page_directory(void);
void vmm_destroy_user_page_directory(page_directory_t* page);

#endif
