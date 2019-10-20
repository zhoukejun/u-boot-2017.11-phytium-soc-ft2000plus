/*                                                                                        
 * (C) Copyright 2019
 * Phytium Technology Co.,Ltd <www.phytium.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef _PHYTIUM_SIP_H
#define _PHYTIUM_SIP_H

#include <common.h>
#include <linux/arm-smccc.h>

/*******************************************************************************
 * SiP Service Function IDs
 ******************************************************************************/

#define PHYTIUM_SIP_SVC_VERSION		0x8200ff03
#define PBF_VERSION					0x82000001
#define CPU_VERSION					0xC2000002
#define CPU_CORE_MAPS				0xC2000003
#define CPU_CORE_CONF				0xC2000004
#define MEM_REGIONS					0xC2000005
#define MCU_DIMMS					0xC2000006
#define PCI_CONTROLLER				0xC2000007
#define PCI_HOST_BRIDGE				0xC2000008

/*******************************************************************************
 * SiP Service error codes
 ******************************************************************************/
#define SIP_E_SUCCESS		0
#define SIP_E_NOT_SUPPORTED	-1
#define SIP_E_INVALID_PARAMS	-2
#define SIP_E_DENIED		-3
#define SIP_E_ALREADY_ON	-4
#define SIP_E_ON_PENDING	-5
#define SIP_E_INTERN_FAIL	-6
#define SIP_E_NOT_PRESENT	-7
#define SIP_E_DISABLED		-8
#define SIP_E_INVALID_ADDRESS	-9

typedef struct cpu_core_maps {
	uint64_t cpu_core_map_count;
	uint64_t cpu_core_map;
} __attribute((aligned(64))) cpu_core_maps_t;

typedef struct cpu_core_conf {
	uint64_t cpu_freq;
	uint64_t cpu_l3_cache;
	uint64_t l3_cache_line;
}__attribute((aligned(64))) cpu_core_conf_t;

typedef struct mem_block {
	uint64_t start;	 
	uint64_t size;  		  
	uint64_t node_id;    
} mem_block_t;

typedef struct mem_region {
	uint64_t mb_count;
	mem_block_t mb_blocks[17];
} __attribute((aligned(64)))mem_region_t;

typedef struct  dimm_block {
	uint64_t dimm_capacity;				//内存容量 In MB
	uint64_t dram_manufacturer_id;		//颗粒厂商ID
	uint64_t module_manufacturer_id;  	//模组厂商ID
	uint64_t module_serial_number;		//模组序列号
	uint64_t slot_number; 				//为1,该 slot有内存条，0则无
} dimm_block_t;

typedef struct mcu_dimm {
	uint64_t mem_freq;
	uint64_t db_count;
	dimm_block_t dimm_blocks[8];
} __attribute((aligned(64)))mcu_dimm_t;

/* PCI-E Related */
typedef struct pci_controller_info {
	uint64_t count;
	uint64_t blocks[5];
	
} __attribute((aligned(64)))pci_controller_info_t;

typedef struct hb_block {	
	uint8_t bus_start;
	uint8_t bus_end;
	uint8_t resv[6];
	uint64_t cfg_base;
	uint64_t io_base;
	uint64_t io_size;
	uint64_t mem32_base;
	uint64_t mem32_size;
	uint64_t mem64_base;
	uint64_t mem64_size;
	uint16_t intA;
	uint16_t intB;
	uint16_t intC;
	uint16_t intD;
} hb_block_t;

typedef struct host_bridge {	
	uint64_t hb_count;
	hb_block_t hb_blocks[5];
}__attribute((aligned(64)))  host_bridge_t;



void get_interface_version(void);
void get_firmware_version(void);
void get_cpu_version(void);
void get_cpu_core_maps(void);
void get_cpu_core_conf(void);
void get_mem_regions(void);
void get_mcu_dimms(void);
void get_pci_controller(void);
void get_pci_host_bridge(void);





#if 0
void get_cpu_info(void);
void get_memory_info(void);
void print_cpu_info(void);
void print_pcie_info(void);
void print_mem_info(void);
void get_bl33_info(void);
#endif
#endif

