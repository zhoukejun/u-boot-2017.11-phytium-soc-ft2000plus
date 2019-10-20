/*                                                                                        
 * (C) Copyright 2019
 * Phytium Technology Co.,Ltd <www.phytium.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __FT2000PLUS_H
#define __FT2000PLUS_H

#ifndef CONFIG_SPL_BUILD
#include <config_distro_defaults.h>
#endif

#define CONFIG_SYS_LDSCRIPT 			"arch/arm/cpu/armv8/u-boot.lds"
#define CONFIG_SYS_MEMTEST_START		PHYS_SDRAM_1

/* Physical Memory Map */

/* CONFIG_SYS_TEXT_BASE needs to align with where ATF loads bl33.bin */
#define CONFIG_SYS_TEXT_BASE        0x1400000

#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1				0x00000000		 /* SDRAM Bank  start address */

#define PHYS_SDRAM_1_SIZE			0xC0000000ul	 /* 3GB */

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1

#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_SDRAM_BASE + 0x7fff0)

#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x80000)

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY			50 * 1000 *1000	  /* 50MHz */

/*Size of malloc() pool*/
#define CONFIG_SYS_MALLOC_LEN		(16 * 1024 *1024 + CONFIG_ENV_SIZE)

/* SMP Spin Table Address */
#define CPU_RELEASE_ADDR				(CONFIG_SYS_SDRAM_BASE + 0x7fff0)

/* GPIO */
#define GPIO_BASE						0x80028006000
/* 16550a Serial Configuration */
#define CONFIG_CONS_INDEX				1
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_COM1			0x80028001000	
#define CONFIG_SYS_NS16550_CLK			COUNTER_FREQUENCY
#define CONFIG_SYS_NS16550_REG_SIZE		-4

/* Initial CONFIG_BOOTARGS variables */
#define UART_BASE_ADDR_STR  "0x80028001000"


/*boot*/
#define CONFIG_USE_BOOTARGS


#ifdef CONFIG_NAND_BOOT
#define CONFIG_EXTRA_ENV_SETTINGS			\
	"kernel_addr=0x80000300000\0"			\
	"initrd_addr=0x80001400000\0"			\
	"initrd_size=0x10F5398\0"				\
	"fdt_addr=0x80000200000\0"	   
	   	 				 
#define CONFIG_BOOTARGS    "console=ttyS0,115200 earlycon=uart8250,mmio32,0x80028001000 root=/dev/ram0 rw"
#define CONFIG_BOOTCOMMAND	"bootm $kernel_addr $initrd_addr:$initrd_size $fdt_addr"

#elif CONFIG_SATA_BOOT
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"kernel_addr=0x90100000\0"  						\
	"initrd_addr=0x95000000\0"  						\
	"initrd_size=0x1000000\0"							\
	"fdt_addr=0x90000000\0"								\
	"scsi_load_kern=ext4load scsi 0:1 $kernel_addr uImage-ft2000plus\0"    \
	"scsi_load_fdt=ext4load scsi 0:1 $fdt_addr    dtb/ft2000plus.dtb\0"   \
	"scsi_load_initrd=ext4load scsi 0:1 $initrd_addr initramfs.img\0"        \
	"scsi_boot=run scsi_load_kern;run scsi_load_fdt;run scsi_load_initrd;run next_boot\0"   \
	"next_boot=bootm $kernel_addr $initrd_addr:0x2000000 $fdt_addr\0"

#define CONFIG_BOOTCOMMAND "run scsi_boot"
#define CONFIG_BOOTARGS	  "console=ttyS0,115200 earlycon=uart8250,mmio32,0x80028001000 root=/dev/sda2 rw"
#endif


/* Miscellaneous configurable options */
#define CONFIG_ENV_SIZE			0x1000   /*4K bytes*/
#define CONFIG_SYS_MONITOR_BASE (CONFIG_SYS_TEXT_BASE)
#define HAVE_BLOCK_DEVICE

/* PCIe Related Configurations */
#ifdef CONFIG_PCI
#define CONFIG_SYS_PCI_64BIT
#define CONFIG_PCI_SCAN_SHOW
#endif

/* sata/scsi configutations */
#define CONFIG_SYS_SATA_MAX_DEVICE      4
#define CONFIG_LIBATA
#define CONFIG_SCSI_AHCI
#define CONFIG_SYS_SCSI_MAX_SCSI_ID    4
#define CONFIG_SYS_SCSI_MAX_LUN        1
#define CONFIG_SYS_SCSI_MAX_DEVICE     (CONFIG_SYS_SCSI_MAX_SCSI_ID * CONFIG_SYS_SCSI_MAX_LUN)
#define CONFIG_SCSI_DEV_LIST {0x1b4b, 0x9215} 

#define CONFIG_SYS_BOOTM_LEN	0x2400000

#endif	/* __FT2000PLUS_H */


