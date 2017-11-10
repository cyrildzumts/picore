#ifndef _CPU_H
#define _CPU_H

// SCR : SECURE CONFIGURATION REGISTER
// SCR is only accessible from Secure PL1 Modes
#define SCR_SIF                 (1 << 9)
#define SCR_HCE                 (1 << 8)
#define SCR_SCD                 (1 << 7)
#define SCR_nET                 (1 << 6)
#define SCR_AW                  (1 << 5)
#define SCR_FW                  (1 << 4)
#define SCR_EA                  (1 << 3)
#define SCR_FIQ                 (1 << 2)
#define SCR_IRQ                 (1 << 1)
#define SCR_NS                  (1 << 0)

// HCR: HYP CONFIGURATION REGISTER.
// Only accessible from Hyp Mode. This register
// resets to zeros.
#define HCR_TGE                 (1 << 27)
#define HCR_TVM                 (1 << 26)
#define HCR_TTLB                (1 << 25)
#define HCR_TPU                 (1 << 24)
#define HCR_TSC                 (1 << 19)
#define HCR_MASK_OVERRIDE       (0x7 << 3)
#define HCR_PTW                 (1 << 2)
#define HCR_VM                  (1 << 0)

// MPIDR : Multiprocessor Affinity Register
#define MPIRD_CPUID             (0x3 << 0)
#define MPIRD_CLUSTERID         (0xF << 8)
#define MPIRD_MT                (1 << 24)
#define MPIRD_U                 (1 << 30)
#define MPIRD_REG_FORMAT        (1 << 31)
struct cpu_info
{
    unsigned int mpidr;
    unsigned int revision;
    unsigned int frequency;
};
void cpu_fetch_info(struct cpu_info *cpu);
unsigned int read_mpidr();
unsigned int cpu_cpuid();
unsigned int read_frequency();

unsigned int read_scr();
unsigned int read_hcr();
void printSCR();
void printHCR();
#endif //_CPU_H
