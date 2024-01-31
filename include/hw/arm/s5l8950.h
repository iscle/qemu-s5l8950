/*
 * Apple A6 (S5L8950) System on Chip emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef HW_ARM_S5L8950_H
#define HW_ARM_S5L8950_H

#include "qom/object.h"
#include "target/arm/cpu.h"
#include "hw/intc/arm_gic.h"
#include "hw/misc/s5l8950-pmgr.h"
#include "sysemu/block-backend.h"

/**
 * S5L8950 device list
 *
 * This enumeration can be used refer to a particular device in the
 * S5L8950 SoC. For example, the physical memory base address for
 * each device can be found in the S5L8950State object in the memmap member
 * using the device enum value as index.
 *
 * @see S5L8950State
 */
enum {
    S5L8950_DEV_SRAM,
    S5L8950_DEV_SDIO,
    S5L8950_DEV_SHA1,
    S5L8950_DEV_SHA2,
    S5L8950_DEV_NAND_DLL,
    S5L8950_DEV_FMI0,
    S5L8950_DEV_FMI1,
    S5L8950_DEV_SPI0,
    S5L8950_DEV_SPI1,
    S5L8950_DEV_SPI2,
    S5L8950_DEV_SPI3,
    S5L8950_DEV_SPI4,
    S5L8950_DEV_UART0,
    S5L8950_DEV_UART1,
    S5L8950_DEV_UART2,
    S5L8950_DEV_UART3,
    S5L8950_DEV_UART4,
    S5L8950_DEV_UART5,
    S5L8950_DEV_UART6,
    S5L8950_DEV_PKE,
    S5L8950_DEV_IIC,
    S5L8950_DEV_AUDIO,
    S5L8950_DEV_USBPHY,
    S5L8950_DEV_USBOTG,
    S5L8950_DEV_UPERF_WIDGETS,
    S5L8950_DEV_UPERF_PL301,
    S5L8950_DEV_CDMA,
    S5L8950_DEV_VENC,
    S5L8950_DEV_VDEC,
    S5L8950_DEV_JPEG0,
    S5L8950_DEV_SCALER,
    S5L8950_DEV_JPEG1,
    S5L8950_DEV_NRT_DART,
    S5L8950_DEV_NRT_DART_WIDGETS,
    S5L8950_DEV_NRT_DART_PL301,
    S5L8950_DEV_NRT_TOP_WIDGETS,
    S5L8950_DEV_NRT_TOP_PL301,
    S5L8950_DEV_CLCD,
    S5L8950_DEV_DITHER0,
    S5L8950_DEV_TVOUT,
    S5L8950_DEV_DSIM,
    S5L8950_DEV_RGBOUT,
    S5L8950_DEV_DP,
    S5L8950_DEV_DITHER1,
    S5L8950_DEV_LPDP,
    S5L8950_DEV_ISP,
    S5L8950_DEV_DISP0,
    S5L8950_DEV_DISP1,
    S5L8950_DEV_RT_DART,
    S5L8950_DEV_RT_TOP_WIDGETS,
    S5L8950_DEV_RT_TOP_PL301,
    S5L8950_DEV_ADSP,
    S5L8950_DEV_AMP,
    S5L8950_DEV_SCC,
    S5L8950_DEV_VROM,
    S5L8950_DEV_PMGR,
    S5L8950_DEV_AIC,
    S5L8950_DEV_IOP,
    S5L8950_DEV_ROSC,
    S5L8950_DEV_CHIPID,
    S5L8950_DEV_SWI,
    S5L8950_DEV_DWI,
    S5L8950_DEV_AMC,
    S5L8950_DEV_GPIO,
    S5L8950_DEV_PIO,
    S5L8950_DEV_CIM,
    S5L8950_DEV_CDIO_WIDGETS,
    S5L8950_DEV_CDIO_PL301,
    S5L8950_DEV_SDRAM,
};

/** Total number of CPU cores in the S5L8950 SoC */
#define S5L8950_NUM_CPUS    (2)

/**
 * S5L8950 object model
 * @{
 */

/** Object type for the S5L8950 SoC */
#define TYPE_S5L8950    "s5l8950"

/** Convert input object to S5L8950 state object */
OBJECT_DECLARE_SIMPLE_TYPE(S5L8950State, S5L8950)

/** @} */

/**
 * S5L8950 object
 *
 * This struct contains the state of all the devices
 * which are currently emulated by the S5L8950 SoC code.
 */
struct S5L8950State {
    /*< private >*/
    DeviceState parent_obj;

    /*< public >*/
    ARMCPU cpu[S5L8950_NUM_CPUS];
    const hwaddr *memmap;
    MemoryRegion sram;
    MemoryRegion sram_alias;
    MemoryRegion sram_vrom;
    S5L8950PmgrState pmgr;
};

#endif /* HW_ARM_S5L8950_H */
