/*
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
#include <platform.h>
#include <platform/clock.h>
#include <uart_dm.h>
#include <arch/arm.h>
#include <arch/arm/mmu.h>
#include <dev/interrupt/arm_gic.h>
#include <qtimer.h>
#include <kernel/vm.h>
#include <lk/reg.h>
#include <platform/irqs.h>
#include <dev/uart.h>
#include <arch/ops.h>

struct mmu_initial_mapping mmu_initial_mappings[] = {
    {
        .phys = MEMBASE,
        .virt = MEMBASE,
        .size = MEMSIZE,
        .flags = 0,
        .name = "memory"
    },
    {
        .phys = MSM_IOMAP_BASE,
        .virt = MSM_IOMAP_BASE,
        .size = MSM_IOMAP_END - MSM_IOMAP_BASE,
        .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
    },
    {
        .phys = A53_SS_BASE,
        .virt = A53_SS_BASE,
        .size = A53_SS_END - A53_SS_BASE,
        .flags = MMU_INITIAL_MAPPING_FLAG_DEVICE,
    },
    /* null entry to terminate the list */
    { 0 }
};

static pmm_arena_t arena = {
    .name = "sdram",
    .base = MEMBASE,
    .size = MEMSIZE,
    .flags = PMM_ARENA_FLAG_KMAP,
};

void platform_early_init(void) {
    msm8916_clock_init();
    uart_dm_init(2, BLSP1_UART1_BASE);
    arm_gic_init();
    qtimer_init();
    pmm_add_arena(&arena);
}

int platform_dgetc(char *c, bool wait) {
    int n = uart_getc(0, 0);
    if (n < 0) {
        return -1;
    } else {
        *c = n;
        return 0;
    }
}

void platform_dputc(char c) {
    uart_putc(0, c);
}

int qtmr_irq(void) {
    return INT_QTMR_FRM_0_PHYSICAL_TIMER_EXP_8x16;
}

void platform_halt(platform_halt_action suggested_action,
                          platform_halt_reason reason) {
  uint32_t *base = 0;
  vmm_alloc_physical(vmm_get_kernel_aspace(), "pmic", PAGE_SIZE, &base,
                     8, 0x004ab000, 0, ARCH_MMU_FLAG_UNCACHED_DEVICE);
  *base = 0;

  spin(1000);

  dprintf(ALWAYS, "HALT: spinning forever... (reason = %d)\n", reason);
  arch_disable_ints();
  for (;;)
    arch_idle();
}
