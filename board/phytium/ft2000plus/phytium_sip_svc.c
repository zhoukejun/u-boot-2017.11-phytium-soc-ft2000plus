/*                                                                                        
 * (C) Copyright 2019
 * Phytium Technology Co.,Ltd <www.phytium.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/arch/phytium_sip_svc.h>

void get_interface_version(void)
{
	u64 interface_version_major,interface_version_minor;
	struct arm_smccc_res res;
	
	arm_smccc_smc(PHYTIUM_SIP_SVC_VERSION, 0, 0, 0, 0, 0, 0, 0, &res);
	interface_version_major = res.a0 >> 16;
	interface_version_minor = res.a0 & 0xffff;
	
	printf("Phytium Sip Service Interface version is %llx.%llx\n",interface_version_major,interface_version_minor);
	printf("\n");
}

void get_firmware_version(void)
{
	u64 firmware_version_major,firmware_version_minor;
	struct arm_smccc_res res;
	
	arm_smccc_smc(PBF_VERSION, 0, 0, 0, 0, 0, 0, 0, &res);	
	firmware_version_major = res.a0 >> 16;
	firmware_version_minor = res.a0 & 0xffff;	
	
	printf("Phytium Base Firmware version is %llx.%llx\n", firmware_version_major, firmware_version_minor);
	printf("\n");
}
void get_cpu_version(void)
{
	struct arm_smccc_res res;
	uint64_t ret = 0, cpu_version;
	
	arm_smccc_smc(CPU_VERSION, (unsigned long)&cpu_version, (unsigned long)sizeof(uint64_t), 0, 0, 0, 0, 0, &res);
	
	ret = res.a0; 
	dsb();
	isb();
	
	if(SIP_E_NOT_SUPPORTED == ret) {
		printf("Currently Not Support. \n");
	} 
}

void get_cpu_core_maps(void)
{
	struct arm_smccc_res res;
	cpu_core_maps_t cpu_core_maps;
	uint64_t ret = 0, size = 0;

	invalidate_dcache_range( (unsigned long)&cpu_core_maps, (unsigned long)(&cpu_core_maps + sizeof(cpu_core_maps_t)));
	arm_smccc_smc(CPU_CORE_MAPS, (unsigned long)&cpu_core_maps, (unsigned long)sizeof(cpu_core_maps_t), 0, 0, 0, 0, 0, &res);

	ret = res.a0;
	size = res.a1;
	dsb();
	isb();


	if(SIP_E_INVALID_PARAMS == ret) {
		printf("buff size is small, should be %llx \n", size);		
	} else {
		
		printf("cpu_core_map_count = %lld \n", cpu_core_maps.cpu_core_map_count);
		printf("cpu_core_map = 0x%llx \n", cpu_core_maps.cpu_core_map);
	}
	
}

void get_cpu_core_conf(void)
{
	struct arm_smccc_res res;
    cpu_core_conf_t cpu_core_conf;
	uint64_t ret = 0, size = 0;

	invalidate_dcache_range((ulong)&cpu_core_conf, (ulong)(&cpu_core_conf + sizeof(cpu_core_conf_t)));
	arm_smccc_smc(CPU_CORE_CONF, (unsigned long)&cpu_core_conf, (unsigned long)sizeof(cpu_core_conf_t), 0, 0, 0, 0, 0, &res);

	ret = res.a0;
	size = res.a1;
	dsb();
	isb();
	
	if(SIP_E_INVALID_PARAMS == ret) {
		printf("Buff Size Is Small, Should Be 0x%llx \n", size);		
	} else {
		printf("cpu_freq = %lld \n", cpu_core_conf.cpu_freq);
		printf("l3_cache  = 0x%llx \n", cpu_core_conf.cpu_l3_cache);
		printf("cache line = %lld \n", cpu_core_conf.l3_cache_line);
	}
	
}

void get_mem_regions(void)
{
	struct arm_smccc_res res;
	mem_region_t mem_region;
	uint64_t ret = 0, size = 0;
	int i;

	invalidate_dcache_range((ulong)&mem_region, (ulong)(&mem_region + sizeof(mem_region_t)));
	arm_smccc_smc(MEM_REGIONS, (unsigned long)&mem_region, (unsigned long)sizeof(mem_region_t), 0, 0, 0, 0, 0, &res);

	ret = res.a0;
	size = res.a1;
	dsb();
	isb();
	
	if(SIP_E_INVALID_PARAMS == ret) {
		printf("Buff Size Is Small, Should Be 0x%llx \n ", size);		
	} else {
		printf("mb_count : %lld \n", mem_region.mb_count);
		for(i = 0; i < mem_region.mb_count; i++) {
			printf("node_id : %lld \n", mem_region.mb_blocks[i].node_id);
			printf("start : 0x%llx \n", mem_region.mb_blocks[i].start);
			printf("size : 0x%llx \n", mem_region.mb_blocks[i].size);
			printf("\n");
		}
	}
	
}

void get_mcu_dimms(void)
{
	struct arm_smccc_res res;
	mcu_dimm_t mcu_dimms;
	uint64_t ret = 0, size = 0;
	int i;

	invalidate_dcache_range((ulong)&mcu_dimms, (ulong)(&mcu_dimms + sizeof(mcu_dimm_t)));
	arm_smccc_smc(MCU_DIMMS, (unsigned long)&mcu_dimms, (unsigned long)sizeof(mcu_dimm_t), 0, 0, 0, 0, 0, &res);

	ret = res.a0;
	size = res.a1;
	dsb();
	isb();
	
	if(SIP_E_INVALID_PARAMS == ret) {
		printf("Buff Size Is Small, Should Be 0x%llx \n", size);		
	} else {
		for(i = 0; i < mcu_dimms.db_count; i++) {
			printf("dimm_capacity :%lld \n", mcu_dimms.dimm_blocks[i].dimm_capacity);
			printf("dram_manufacturer_id :0x%llx \n", mcu_dimms.dimm_blocks[i].dram_manufacturer_id);
			printf("module_manufacturer_id :0x%llx \n", mcu_dimms.dimm_blocks[i].module_manufacturer_id);
			printf("module_serial_number :0x%llx \n", mcu_dimms.dimm_blocks[i].module_serial_number);
			printf("slot_number :0x%llx \n", mcu_dimms.dimm_blocks[i].slot_number);
			printf("\n");
		}
	}
	
}

void get_pci_controller()
{
	struct arm_smccc_res res;
	pci_controller_info_t pci_controller;
	uint64_t ret = 0, size = 0;
	int i;

	invalidate_dcache_range((ulong)&pci_controller, (ulong)(&pci_controller + sizeof(pci_controller_info_t)));
	arm_smccc_smc(PCI_CONTROLLER, (unsigned long)&pci_controller, (unsigned long)sizeof(pci_controller_info_t), 0, 0, 0, 0, 0, &res);

	ret = res.a0;
	size = res.a1;
	dsb();
	isb();

	if(SIP_E_INVALID_PARAMS == ret) {
		printf("Buff Size Is Small, Should Be 0x%llx \n", size);		
	} else {
		for(i = 0; i < pci_controller.count; i++) {
			printf("pci_controller.blocks[%d] = %llx \n", i, pci_controller.blocks[i]);
			printf("[%d].lane : 0x%llx \n", i, (pci_controller.blocks[i] >> 56) & 0xff);
			printf("[%d].speed : 0x%llx \n", i, (pci_controller.blocks[i] >> 48) & 0xff);
			printf("\n");
		}
	}		
}

void get_pci_host_bridge()
{
	struct arm_smccc_res res;
	host_bridge_t hose;
	uint64_t ret = 0, size = 0;
	int i;

	invalidate_dcache_range((ulong)&hose, (ulong)(&hose + sizeof(host_bridge_t)));
	arm_smccc_smc(PCI_HOST_BRIDGE, (unsigned long)&hose, (unsigned long)sizeof(host_bridge_t), 0, 0, 0, 0, 0, &res);

	ret = res.a0;
	size = res.a1;
	dsb();
	isb();

	if(SIP_E_INVALID_PARAMS == ret) {
		printf("Buff Size Is Small, Should Be 0x%llx \n", size);		
	} else {
		for(i = 0; i < hose.hb_count; i++) {
			printf("[%d].bus_start = %d\n", i, hose.hb_blocks[i].bus_start);	
			printf("[%d].bus_end = %d\n", i, hose.hb_blocks[i].bus_end);
			printf("[%d].cfg_base = %llx\n", i, hose.hb_blocks[i].cfg_base);
			printf("[%d].io_base = %llx\n", i, hose.hb_blocks[i].io_base);
			printf("[%d].io_size = %llx\n", i, hose.hb_blocks[i].io_size);
			printf("[%d].mem32_base = %llx\n", i, hose.hb_blocks[i].mem32_base);
			printf("[%d].mem32_size = %llx\n", i, hose.hb_blocks[i].mem32_size);
			printf("[%d].mem64_base = %llx\n", i, hose.hb_blocks[i].mem64_base);
			printf("[%d].mem64_size = %llx\n", i, hose.hb_blocks[i].mem64_size);	
			printf("[%d].intA = %d\n", i, hose.hb_blocks[i].intA);
			printf("[%d].intB = %d\n", i, hose.hb_blocks[i].intB);
			printf("[%d].intC = %d\n", i, hose.hb_blocks[i].intC);
			printf("[%d].intD = %d\n", i, hose.hb_blocks[i].intD);
			printf("\n");
		}
	}		
	
}




