/*
 * Apple A6 (S5L8950) System on Chip emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu/error-report.h"
#include "qemu/module.h"
#include "qemu/units.h"
#include "hw/qdev-core.h"
#include "hw/sysbus.h"
#include "hw/char/serial.h"
#include "hw/misc/unimp.h"
#include "hw/usb/hcd-ehci.h"
#include "hw/loader.h"
#include "sysemu/sysemu.h"
#include "hw/arm/s5l8950.h"

/* Memory map */
const hwaddr s5l8950_memmap[] = {
    [S5L8950_DEV_SRAM]              = 0x10000000, // Length: 0x00080000
    [S5L8950_DEV_SDIO]              = 0x30000000,
    [S5L8950_DEV_SHA1]              = 0x30100000,
    [S5L8950_DEV_SHA2]              = 0x30200000,
    [S5L8950_DEV_NAND_DLL]          = 0x31000000,
    [S5L8950_DEV_FMI0]              = 0x31200000,
    [S5L8950_DEV_FMI1]              = 0x31300000,
    [S5L8950_DEV_SPI0]              = 0x32000000,
    [S5L8950_DEV_SPI1]              = 0x32100000,
    [S5L8950_DEV_SPI2]              = 0x32200000,
    [S5L8950_DEV_SPI3]              = 0x32300000,
    [S5L8950_DEV_SPI4]              = 0x32400000,
    [S5L8950_DEV_UART0]             = 0x32500000,
    [S5L8950_DEV_UART1]             = 0x32600000,
    [S5L8950_DEV_UART2]             = 0x32700000,
    [S5L8950_DEV_UART3]             = 0x32800000,
    [S5L8950_DEV_UART4]             = 0x32900000,
    [S5L8950_DEV_UART5]             = 0x32A00000,
    [S5L8950_DEV_UART6]             = 0x32B00000,
    [S5L8950_DEV_PKE]               = 0x33100000,
    [S5L8950_DEV_IIC]               = 0x33200000,
    [S5L8950_DEV_AUDIO]             = 0x34000000,
    [S5L8950_DEV_USBPHY]            = 0x36000000,
    [S5L8950_DEV_USBOTG]            = 0x36100000,
    [S5L8950_DEV_UPERF_WIDGETS]     = 0x36E00000,
    [S5L8950_DEV_UPERF_PL301]       = 0x36F00000,
    [S5L8950_DEV_CDMA]              = 0x37000000,
    [S5L8950_DEV_VENC]              = 0x38000000,
    [S5L8950_DEV_VDEC]              = 0x38100000,
    [S5L8950_DEV_JPEG0]             = 0x38200000,
    [S5L8950_DEV_SCALER]            = 0x38300000,
    [S5L8950_DEV_JPEG1]             = 0x38500000,
    [S5L8950_DEV_NRT_DART]          = 0x38B00000,
    [S5L8950_DEV_NRT_DART_WIDGETS]  = 0x38C00000,
    [S5L8950_DEV_NRT_DART_PL301]    = 0x38D00000,
    [S5L8950_DEV_NRT_TOP_WIDGETS]   = 0x38E00000,
    [S5L8950_DEV_NRT_TOP_PL301]     = 0x38F00000,
    [S5L8950_DEV_CLCD]              = 0x39200000,
    [S5L8950_DEV_DITHER0]           = 0x39300000,
    [S5L8950_DEV_TVOUT]             = 0x39400000,
    [S5L8950_DEV_DSIM]              = 0x39500000,
    [S5L8950_DEV_RGBOUT]            = 0x39600000,
    [S5L8950_DEV_DP]                = 0x39700000,
    [S5L8950_DEV_DITHER1]           = 0x39800000,
    [S5L8950_DEV_LPDP]              = 0x39900000,
    [S5L8950_DEV_ISP]               = 0x3A000000,
    [S5L8950_DEV_DISP0]             = 0x3A100000,
    [S5L8950_DEV_DISP1]             = 0x3A200000,
    [S5L8950_DEV_RT_DART]           = 0x3A400000,
    [S5L8950_DEV_RT_TOP_WIDGETS]    = 0x3A500000,
    [S5L8950_DEV_RT_TOP_PL301]      = 0x3A600000,
    [S5L8950_DEV_ADSP]              = 0x3B000000,
    [S5L8950_DEV_AMP]               = 0x3D000000,
    [S5L8950_DEV_SCC]               = 0x3D200000,
    [S5L8950_DEV_VROM]              = 0x3F000000, // Length: 0x00010000
    [S5L8950_DEV_PMGR]              = 0x3F100000,
    [S5L8950_DEV_AIC]               = 0x3F200000,
    [S5L8950_DEV_IOP]               = 0x3F300000,
    [S5L8950_DEV_ROSC]              = 0x3F400000,
    [S5L8950_DEV_CHIPID]            = 0x3F500000,
    [S5L8950_DEV_SWI]               = 0x3F600000,
    [S5L8950_DEV_DWI]               = 0x3F700000,
    [S5L8950_DEV_AMC]               = 0x3F800000,
    [S5L8950_DEV_GPIO]              = 0x3FA00000,
    [S5L8950_DEV_PIO]               = 0x3FB00000,
    [S5L8950_DEV_CIM]               = 0x3FD00000,
    [S5L8950_DEV_CDIO_WIDGETS]      = 0x3FE00000,
    [S5L8950_DEV_CDIO_PL301]        = 0x3FF00000,
    [S5L8950_DEV_SDRAM]             = 0x80000000, // Length: 0x40000000
};

/* List of unimplemented devices */
static struct {
    const char *device_name;
    hwaddr base;
    hwaddr size;
} unimplemented[] = {
    { "unk_peripheral",   0, 0x80000000 },
};

static void s5l8950_init(Object *obj)
{
    S5L8950State *s = S5L8950(obj);

    s->memmap = s5l8950_memmap;

    for (int i = 0; i < S5L8950_NUM_CPUS; i++) {
        object_initialize_child(obj, "cpu[*]", &s->cpu[i], ARM_CPU_TYPE_NAME("cortex-a15"));
    }

    object_initialize_child(obj, "pmgr", &s->pmgr, TYPE_S5L8950_PMGR);
}

static void s5l8950_realize(DeviceState *dev, Error **errp)
{
    S5L8950State *s = S5L8950(dev);
    unsigned i;

    /* CPUs */
    for (i = 0; i < S5L8950_NUM_CPUS; i++) {

        /*
         * Disable secondary CPUs. Guest EL3 firmware will start
         * them via CPU reset control registers.
         */
        qdev_prop_set_bit(DEVICE(&s->cpu[i]), "start-powered-off", i > 0);

        /* All exception levels required */
        qdev_prop_set_bit(DEVICE(&s->cpu[i]), "has_el3", true);
        qdev_prop_set_bit(DEVICE(&s->cpu[i]), "has_el2", true);

        /* Mark realized */
        qdev_realize(DEVICE(&s->cpu[i]), NULL, &error_fatal);
    }

    /* SRAM */
    memory_region_init_ram(&s->sram, OBJECT(dev), "sram", 0x00080000, &error_abort);
    memory_region_add_subregion(get_system_memory(), s->memmap[S5L8950_DEV_SRAM], &s->sram);

    memory_region_init_alias(&s->sram_alias, OBJECT(dev), "sram_alias", &s->sram, 0, 0x00080000);
    memory_region_add_subregion(get_system_memory(), 0x0, &s->sram_alias);

    /* VROM */
    memory_region_init_rom(&s->sram_vrom, OBJECT(dev), "sram_vrom", 0x00010000, &error_abort);
    memory_region_add_subregion(get_system_memory(), s->memmap[S5L8950_DEV_VROM], &s->sram_vrom);

    /* PMGR */
    sysbus_realize(SYS_BUS_DEVICE(&s->pmgr), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(&s->pmgr), 0, s->memmap[S5L8950_DEV_PMGR]);

    /* Unimplemented devices */
    for (i = 0; i < ARRAY_SIZE(unimplemented); i++) {
        create_unimplemented_device(unimplemented[i].device_name, unimplemented[i].base, unimplemented[i].size);
    }
}

static void s5l8950_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);

    dc->realize = s5l8950_realize;
}

static const TypeInfo s5l8950_type_info = {
    .name = TYPE_S5L8950,
    .parent = TYPE_DEVICE,
    .instance_size = sizeof(S5L8950State),
    .instance_init = s5l8950_init,
    .class_init = s5l8950_class_init,
};

static void s5l8950_register_types(void)
{
    type_register_static(&s5l8950_type_info);
}

type_init(s5l8950_register_types)
