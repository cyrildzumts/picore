#include <cpu.h>
unsigned int read_mpidr()
{
    unsigned int mpidr = 0;
    __asm("MRC P15, 0,  %0, C0, C0, 5\n\t"
        : "=r"(mpidr));
    return mpidr;
}

void cpu_fetch_info(struct cpu_info *cpu)
{
    cpu->mpidr = read_mpidr();
}

unsigned int cpu_cpuid()
{

}

unsigned int read_frequency()
{
    unsigned int freq = 0;
    __asm("MRC P15, 0, %0, C14, c0, 0\n\t"
        : "=r"(freq));
    return freq;
}

unsigned int read_scr()
{
    unsigned int scr = 0;
    __asm("MRC P15, 0, %0, C1, C1, 0\n\t"
        : "=r"(scr));
    return scr;
}


unsigned int read_hcr()
{
    unsigned int hcr = 0;
    __asm("MRC P15, 4, %0, C1, C1, 0\n\t"
        : "=r"(hcr));
    return hcr;
}
void printHCR()
{
    unsigned int hcr = read_hcr();
    printf("HCR REG : %X\n", hcr);
    printf("HCR_TSC : %d\n", (hcr & HCR_TSC) >> 19);
}
void printSCR()
{
    unsigned int scr = read_scr();
    printf("SCR REG : %X\n", scr);
}
