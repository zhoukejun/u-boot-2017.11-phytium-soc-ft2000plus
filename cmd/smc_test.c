#include <common.h>
#include <command.h>
#include <asm/arch/phytium_sip_svc.h>


static int do_interface_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_interface_version();

	return 0;
}

static int do_firmware_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_firmware_version();

	return 0;
}

static int do_cpu_version(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_cpu_version();

	return 0;
}


static int do_cpu_core_maps(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_cpu_core_maps();

	return 0;
}

static int do_cpu_core_conf(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_cpu_core_conf();

	return 0;
}

static int do_mem_region(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_mem_regions();

	return 0;
}

static int do_mcu_dimm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_mcu_dimms();
	
	return 0;
}



static int do_pcie_controller(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_pci_controller();

	return 0;
}

static int do_pcie_host(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	get_pci_host_bridge();

	return 0;
}



static int do_bl33_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	//get_bl33_info();

	return 0;
}

U_BOOT_CMD(
		interface_version,	1,	1,	do_interface_version,
		"get do_interface version",
	""
);

U_BOOT_CMD(
	firmware_version,	1,	1,	do_firmware_version,
	"get firmware version",
	""
);


U_BOOT_CMD(
	cpu_version,	1,	1,	do_cpu_version,
	"get cpu version",
	""
);

U_BOOT_CMD(
	cpu_core_maps,	1,	1,	do_cpu_core_maps,
	"get core maps",
	""
);

U_BOOT_CMD(
	cpu_core_conf,	1,	1,	do_cpu_core_conf,
	"get cpu config information",
	""
);

U_BOOT_CMD(
	mem_region,	1,	1,	do_mem_region,
	"get memory regions",
	""
);

U_BOOT_CMD(
	mcu_dimm,	1,	1,	do_mcu_dimm,
	"get dimm info",
	""
);

U_BOOT_CMD(
	pcie_controller,	1,	1,	do_pcie_controller,
	"get pcie controller info",
	""
);

U_BOOT_CMD(
	pcie_host,	1,	1,	do_pcie_host,
	"get pcie host info",
	""
);



U_BOOT_CMD(
	bl33_info, 1, 1, do_bl33_info,
	"get bl33 info",
	""
);

