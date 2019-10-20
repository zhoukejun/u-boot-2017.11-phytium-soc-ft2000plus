/*                                                                                        
 * (C) Copyright 2019
 * Phytium Technology Co.,Ltd <www.phytium.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <netdev.h>
#include <asm/io.h>
#include <linux/compiler.h>
#include <asm/armv8/mmu.h>

DECLARE_GLOBAL_DATA_PTR;

static struct mm_region ft2000plus_mem_map[] = {
	{
		.virt = (u64)PHYS_SDRAM_1,
		.phys = (u64)PHYS_SDRAM_1,
		.size = (u64)PHYS_SDRAM_1_SIZE,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0x80000000000UL,
		.phys = 0x80000000000UL,
		.size = 0x4000000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = ft2000plus_mem_map;

int board_init(void)
{
	return 0;
}

/*
 * Board specific reset that is system reset.
 */
void reset_cpu(ulong addr)
{
	unsigned int val;

    /* D3 output 0 */
    val = readl(GPIO_BASE + 0x24);
    val = val & (~(0x1 << 3));
    writel(val, GPIO_BASE + 0x24);

    /* delay 100ms */
    udelay(100000);

    /* D4 output 1 */
    writel(val | (0x1 << 3), GPIO_BASE + 0x24);

}

/*
 * Board specific shutdown that is system shutdown.
 */
void poweroff_cpu(ulong addr)
{
    unsigned int val;

    /* D4 output 0 */
    val = readl(GPIO_BASE + 0x24);
    val = val & (~(0x1 << 4));
    writel(val, GPIO_BASE + 0x24);

    /* delay 100ms */
    udelay(100000);

    /* D4 output 1 */
    writel(val | (0x1 << 4), GPIO_BASE + 0x24);
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;

	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}
