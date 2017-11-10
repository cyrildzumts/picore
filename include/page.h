#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

#define PAGE_SHIFT          12
#define PAGE_SIZE           (1 << PAGE_SHIFT)
#define PAGE_MASK           (~((1 << PAGE_SHIFT) - 1))
#define PAGE_PRESENT        (0x1 << 12)
#define PAGE_PROTECTION     (0x7 << 13)
#define PAGE_MODIFIED       (0x1 << 14)
#define PAGE_REFERENCED     (0x1 << 15)
#define PAGE_CACHE_ENABLE   (0x1 << 16)

struct page{
    uint32_t va; /* Virtual address*/
};
/**
 * @brief __va_to_pa convert a virtual address into
 * a physical address
 * @param va The requested virtual address
 * @return the corresponding physical address
 */
uint32_t __va_to_pa(uint32_t va);

/**
 * @brief __page_index computes the page index containing
 * the virtual address in the page Table
 * @param va
 * @return the page index in the Page Table
 */
uint32_t __page_index(uint32_t va);
#endif // PAGE_H
