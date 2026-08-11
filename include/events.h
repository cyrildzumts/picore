#ifndef EVENT_H
#define EVENT_H

#include <core.h>
#define ARM_INT_BASE               (PERI_BASE + 0xB000)
#define IRQ_BASIC_BASE             (ARM_INT_BASE + 0x200)
#define ARM_IRQ_ENABLE_REG_1       (IRQ_BASIC_BASE + 0x10)
#define ARM_IRQ_ENABLE_REG_2       (IRQ_BASIC_BASE + 0x14)
#define AUX_INT_IDX                29
#define I2C_SPI_SLV_INT_IDX        43
#define PWM0_IDX                   45 
#define PWM1_IDX                   46
#define SMI_IDX                    48
#define GPIO_INT_1_IDX             49
#define GPIO_INT_2_IDX             50
#define GPIO_INT_3_IDX             51
#define GPIO_INT_4_IDX             52
#define I2C_INT_1_IDX              53
#define SPI_INT_1_IDX              54
#define PCM_INT_1_IDX              55
#define UART_INT_1_IDX             57
#define ARM_TIMER_PENDING          (0x1 << 0)
#define EVENT_ARM_TIMER_IRQ         (0x1 << 0)
#define PENDING_IRQ_2               (0x1 << 9)
#define PENDING_IRQ_1               (0x1 << 8)

#define PUSH_BTN_BOUNCING_DELAY    60


typedef struct ARM_IRQ_REGISTERS {
    volatile uint32_t IRQ_BASIC_PENDING;
    volatile uint32_t IRQ_PENDING1;
    volatile uint32_t IRQ_PENDING2;
    volatile uint32_t FIQ_CONTROL;
    volatile uint32_t ENABLE_IRQ1;
    volatile uint32_t ENABLE_IRQ2;
    volatile uint32_t ENABLE_BASIC_IRQ;
    volatile uint32_t DISABLE_IRQ1;
    volatile uint32_t DISABLE_IRQ2;
    volatile uint32_t DISABLE_BASIC_IRQ;
}ARM_IRQ_REGISTERS;


ARM_IRQ_REGISTERS* event_get_arm_irq_registers();

uint32_t irq_is_arm_timer_pending();
uint32_t irq_is_arm_basic_pending();
uint32_t irq_is_arm_basic_pending1();
uint32_t irq_is_arm_basic_pending2();
void irqARMTimerEnable();
void irqARMTimerDisable();
void irq_interrupt_enable(int source);
void irq_interrupt_disable(int source);
void irq_base_interrupt_enable(int source);
void irq_base_interrupt_disable(int source);

#endif //EVENT_H