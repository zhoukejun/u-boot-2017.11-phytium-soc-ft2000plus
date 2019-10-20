/*                                                                                        
 * (C) Copyright 2019
 * Phytium Technology Co.,Ltd <www.phytium.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <pci.h>
#include <asm/io.h>
#include <asm/gic.h>
#include <linux/arm-smccc.h>
#include <asm/arch/phytium_sip_svc.h> 

#define PEU_SPLIT_MODE_X16			0x0
#define PEU_SPLIT_MODE_X8_X8		0x1

#define PCIE_BASE				0x8002C000000	
#define PCIE_CONFIG_BASE		0x80040000000
#define PCIE_PEU1_HPB_BASE		(PCIE_BASE + 0xB01000)
#define PCIE_PEU0_HPB_BASE		(PCIE_BASE + 0xB00000)
#define PCIE_PEU0_SPLIT_MODE	(PCIE_PEU0_HPB_BASE + 0x800)
#define PCIE_PEU1_SPLIT_MODE	(PCIE_PEU1_HPB_BASE + 0x800)
#define PCIE_PHYS_TO_BUS(x)		(x & 0x7ffffffffff)

static struct pci_controller pci_hose[5];


static unsigned long pcie_config_address(pci_dev_t bdf, int offset)
{
	return PCIE_CONFIG_BASE + ((bdf >> 8) << 12) + offset;
}

static int pcie_config_read_byte(struct pci_controller *hose, pci_dev_t bdf,
				 int offset, unsigned char *val)
{
	unsigned long addr;

	addr = pcie_config_address(bdf, offset);
	*val = in_8(addr);

	return 0;
}

static int pcie_config_read_word(struct pci_controller *hose, pci_dev_t bdf,
				  int offset, unsigned short *val)
{
	unsigned long addr;

	addr = pcie_config_address(bdf, offset);
	*val = in_le16(addr);

	return 0;
}

static int pcie_config_read_dword(struct pci_controller *hose, pci_dev_t bdf,
				  int offset,  unsigned int *val)
{
	unsigned long addr;

	addr = pcie_config_address(bdf, offset);
	*val = in_le32(addr);

	return 0;
}

static int pcie_config_write_byte(struct pci_controller *hose, pci_dev_t bdf,
					int offset, unsigned char val)
{
	unsigned long addr;

	addr = pcie_config_address(bdf, offset);
	out_8(addr, (u8)val);

	return 0;
}

static int pcie_config_write_word(struct pci_controller *hose, pci_dev_t bdf,
					int offset, unsigned short val)
{
	unsigned long addr;

	addr = pcie_config_address(bdf, offset);
	out_le16(addr, (u16)val);

	return 0;
}

static int pcie_config_write_dword(struct pci_controller *hose, pci_dev_t bdf,
					int offset, unsigned int val)
{
	unsigned long addr;

	addr = pcie_config_address(bdf, offset);
	out_le32(addr, (u32)val);

	return 0;
}

int pci_skip_dev(struct pci_controller *hose, pci_dev_t parent, pci_dev_t dev)
{
	unsigned char pos, id;
	unsigned int peu0_split, peu1_split;

	if (PCI_BUS(dev) == 0) {
		peu0_split = readl(PCIE_PEU0_SPLIT_MODE) & 0x3;
		peu1_split = (readl(PCIE_PEU1_SPLIT_MODE) >> 0x2) & 0x3;

		if ( (peu0_split == PEU_SPLIT_MODE_X16) &&
				(peu1_split == PEU_SPLIT_MODE_X16) &&
				(PCI_DEV(dev) > 2) )
			return 1;

		if ( (peu0_split == PEU_SPLIT_MODE_X16) &&
				(peu1_split == PEU_SPLIT_MODE_X8_X8) &&
				(PCI_DEV(dev) > 3) )
			return 1;

		if ( (peu0_split == PEU_SPLIT_MODE_X8_X8) &&
				(peu1_split == PEU_SPLIT_MODE_X16) &&
				(PCI_DEV(dev) > 3) )
			return 1;

		if ( (peu0_split == PEU_SPLIT_MODE_X8_X8) &&
				(peu1_split == PEU_SPLIT_MODE_X8_X8) &&
				(PCI_DEV(dev) > 4) )
			return 1;
	}

	if (parent == 0xffffffff)
		return 0;

	pos = PCI_CAPABILITY_LIST;
	while(1) {
		pcie_config_read_byte(hose, parent, pos, &pos);
		if (pos < 0x40)
			break;
		pos &= ~3;
		pcie_config_read_byte(hose, parent, pos, &id);
		if (id == 0xff)
			break;
		if (id == 0x10) {
			unsigned short capreg;
			unsigned char port_type;

			pcie_config_read_word(hose, parent, pos+2, &capreg);
			port_type = (capreg >> 4) & 0xf;
			if (((port_type == 0x6) || (port_type == 0x4)) && (PCI_DEV(dev) != 0))
				return 1;
			else
				return 0;
		}
		pos += 1;
	}

	return 0;
}

int pci_print_dev(struct pci_controller *hose, pci_dev_t dev)
{
	return 1;
}

void register_pci_host_bridge(
	struct pci_controller *pci_hose,
	unsigned long first_busno,
	unsigned long io_base, unsigned long io_size,
	unsigned long mem32_base, unsigned long mem32_size,
	unsigned long mem64_base, unsigned long mem64_size)
{
	struct pci_controller *hose;

	printf("Register Host controller Start Bus Number = %ld\n", first_busno);
	printf("  IO Space:      0x%lx 0x%lx\n", io_base, io_size);
	printf("  MEM32 Space:   0x%lx 0x%lx\n", mem32_base, mem32_size);
	printf("  MEM64 Space:   0x%lx 0x%lx\n", mem64_base, mem64_size);

	hose = pci_hose;
	hose->first_busno = 0;
	hose->current_busno = first_busno;
	hose->last_busno = 0xff;

	/* PCI IO space */
	pci_set_region(hose->regions + 0,
			(PCIE_PHYS_TO_BUS(io_base) + 0x1000) & (~0x50000000),
			io_base + 0x1000,
			io_size - 0x1000,
			PCI_REGION_IO);

	/* PCI Memory Space */
	pci_set_region(hose->regions + 1,
			(PCIE_PHYS_TO_BUS(mem32_base) + 0x100000) & 0xFFFFFFFF,
			mem32_base + 0x100000,
			mem32_size - 0x100000,
			PCI_REGION_MEM);

	/* PCI Memory Prefetch Space */
	pci_set_region(hose->regions + 2,
			PCIE_PHYS_TO_BUS(mem64_base),
			mem64_base,
			mem64_size,
			PCI_REGION_MEM | PCI_REGION_PREFETCH);

	hose->region_count = 3;

	pci_set_ops(hose,
			pcie_config_read_byte,
			pcie_config_read_word,
			pcie_config_read_dword,
			pcie_config_write_byte,
			pcie_config_write_word,
			pcie_config_write_dword);

	pci_register_hose(hose);

	/* Scan PCIe Root Complex */
#ifdef CONFIG_PCI_SCAN_SHOW
	printf("PCI:   Bus Dev VenId DevId Class Int\n");
#endif
	hose->last_busno = pci_hose_scan(hose);
}

void pci_init_board(void)
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
			register_pci_host_bridge(
			&pci_hose[i],
			hose.hb_blocks[i].bus_start,
			hose.hb_blocks[i].io_base, hose.hb_blocks[i].io_size,
			hose.hb_blocks[i].mem32_base, hose.hb_blocks[i].mem32_size,
			hose.hb_blocks[i].mem64_base, hose.hb_blocks[i].mem64_size);
		}
	}		
}
