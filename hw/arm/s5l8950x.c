/*
 * Apple A6 (S5L8950X) System on Chip emulation
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
#include "hw/arm/s5l8950x.h"

/* Memory map */
const hwaddr s5l8950x_memmap[] = {
    [S5L8950X_DEV_SRAM]              = 0x10000000, // Length: 0x00080000
    [S5L8950X_DEV_SDIO]              = 0x30000000,
    [S5L8950X_DEV_SHA1]              = 0x30100000,
    [S5L8950X_DEV_SHA2]              = 0x30200000,
    [S5L8950X_DEV_NAND_DLL]          = 0x31000000,
    [S5L8950X_DEV_FMI0]              = 0x31200000,
    [S5L8950X_DEV_FMI1]              = 0x31300000,
    [S5L8950X_DEV_SPI0]              = 0x32000000,
    [S5L8950X_DEV_SPI1]              = 0x32100000,
    [S5L8950X_DEV_SPI2]              = 0x32200000,
    [S5L8950X_DEV_SPI3]              = 0x32300000,
    [S5L8950X_DEV_SPI4]              = 0x32400000,
    [S5L8950X_DEV_UART0]             = 0x32500000,
    [S5L8950X_DEV_UART1]             = 0x32600000,
    [S5L8950X_DEV_UART2]             = 0x32700000,
    [S5L8950X_DEV_UART3]             = 0x32800000,
    [S5L8950X_DEV_UART4]             = 0x32900000,
    [S5L8950X_DEV_UART5]             = 0x32A00000,
    [S5L8950X_DEV_UART6]             = 0x32B00000,
    [S5L8950X_DEV_PKE]               = 0x33100000,
    [S5L8950X_DEV_IIC]               = 0x33200000,
    [S5L8950X_DEV_AUDIO]             = 0x34000000,
    [S5L8950X_DEV_USBPHY]            = 0x36000000,
    [S5L8950X_DEV_USBOTG]            = 0x36100000,
    [S5L8950X_DEV_UPERF_WIDGETS]     = 0x36E00000,
    [S5L8950X_DEV_UPERF_PL301]       = 0x36F00000,
    [S5L8950X_DEV_CDMA]              = 0x37000000,
    [S5L8950X_DEV_VENC]              = 0x38000000,
    [S5L8950X_DEV_VDEC]              = 0x38100000,
    [S5L8950X_DEV_JPEG0]             = 0x38200000,
    [S5L8950X_DEV_SCALER]            = 0x38300000,
    [S5L8950X_DEV_JPEG1]             = 0x38500000,
    [S5L8950X_DEV_NRT_DART]          = 0x38B00000,
    [S5L8950X_DEV_NRT_DART_WIDGETS]  = 0x38C00000,
    [S5L8950X_DEV_NRT_DART_PL301]    = 0x38D00000,
    [S5L8950X_DEV_NRT_TOP_WIDGETS]   = 0x38E00000,
    [S5L8950X_DEV_NRT_TOP_PL301]     = 0x38F00000,
    [S5L8950X_DEV_CLCD]              = 0x39200000,
    [S5L8950X_DEV_DITHER0]           = 0x39300000,
    [S5L8950X_DEV_TVOUT]             = 0x39400000,
    [S5L8950X_DEV_DSIM]              = 0x39500000,
    [S5L8950X_DEV_RGBOUT]            = 0x39600000,
    [S5L8950X_DEV_DP]                = 0x39700000,
    [S5L8950X_DEV_DITHER1]           = 0x39800000,
    [S5L8950X_DEV_LPDP]              = 0x39900000,
    [S5L8950X_DEV_ISP]               = 0x3A000000,
    [S5L8950X_DEV_DISP0]             = 0x3A100000,
    [S5L8950X_DEV_DISP1]             = 0x3A200000,
    [S5L8950X_DEV_RT_DART]           = 0x3A400000,
    [S5L8950X_DEV_RT_TOP_WIDGETS]    = 0x3A500000,
    [S5L8950X_DEV_RT_TOP_PL301]      = 0x3A600000,
    [S5L8950X_DEV_ADSP]              = 0x3B000000,
    [S5L8950X_DEV_AMP]               = 0x3D000000,
    [S5L8950X_DEV_SCC]               = 0x3D200000,
    [S5L8950X_DEV_VROM]              = 0x3F000000, // Length: 0x00010000
    [S5L8950X_DEV_PMGR]              = 0x3F100000,
    [S5L8950X_DEV_AIC]               = 0x3F200000,
    [S5L8950X_DEV_IOP]               = 0x3F300000,
    [S5L8950X_DEV_ROSC]              = 0x3F400000,
    [S5L8950X_DEV_CHIPID]            = 0x3F500000,
    [S5L8950X_DEV_SWI]               = 0x3F600000,
    [S5L8950X_DEV_DWI]               = 0x3F700000,
    [S5L8950X_DEV_AMC]               = 0x3F800000,
    [S5L8950X_DEV_GPIO]              = 0x3FA00000,
    [S5L8950X_DEV_PIO]               = 0x3FB00000,
    [S5L8950X_DEV_CIM]               = 0x3FD00000,
    [S5L8950X_DEV_CDIO_WIDGETS]      = 0x3FE00000,
    [S5L8950X_DEV_CDIO_PL301]        = 0x3FF00000,
    [S5L8950X_DEV_SDRAM]             = 0x80000000, // Length: 0x40000000
};

/* List of unimplemented devices */
static struct {
    const char *device_name;
    hwaddr base;
    hwaddr size;
} unimplemented[] = {
        { "unk_peripheral",   0, 0x80000000 },
        { "sdio", 0x30000000, 0x01000000 },
        { "sha1", 0x30100000, 0x00100000 },
        { "sha2", 0x30200000, 0x00100000 },
        { "nand_dll", 0x31000000, 0x00100000 },
        { "fmi0", 0x31200000, 0x00100000 },
        { "fmi1", 0x31300000, 0x00100000 },
        { "spi0", 0x32000000, 0x00100000 },
        { "spi1", 0x32100000, 0x00100000 },
        { "spi2", 0x32200000, 0x00100000 },
        { "spi3", 0x32300000, 0x00100000 },
        { "spi4", 0x32400000, 0x00100000 },
        { "uart0", 0x32500000, 0x00100000 },
        { "uart1", 0x32600000, 0x00100000 },
        { "uart2", 0x32700000, 0x00100000 },
        { "uart3", 0x32800000, 0x00100000 },
        { "uart4", 0x32900000, 0x00100000 },
        { "uart5", 0x32A00000, 0x00100000 },
        { "uart6", 0x32B00000, 0x00100000 },
        { "pke", 0x33100000, 0x00100000 },
        { "iic", 0x33200000, 0x00100000 },
        { "audio", 0x34000000, 0x00100000 },
        { "usbphy", 0x36000000, 0x00100000 },
        { "usbotg", 0x36100000, 0x00100000 },
        { "uperf_widgets", 0x36E00000, 0x00100000 },
        { "uperf_pl301", 0x36F00000, 0x00100000 },
        { "cdma", 0x37000000, 0x00100000 },
        { "venc", 0x38000000, 0x00100000 },
        { "vdec", 0x38100000, 0x00100000 },
        { "jpeg0", 0x38200000, 0x00100000 },
        { "scaler", 0x38300000, 0x00100000 },
        { "jpeg1", 0x38500000, 0x00100000 },
        { "nrt_dart", 0x38B00000, 0x00100000 },
        { "nrt_dart_widgets", 0x38C00000, 0x00100000 },
        { "nrt_dart_pl301", 0x38D00000, 0x00100000 },
        { "nrt_top_widgets", 0x38E00000, 0x00100000 },
        { "nrt_top_pl301", 0x38F00000, 0x00100000 },
        { "clcd", 0x39200000, 0x00100000 },
        { "dither0", 0x39300000, 0x00100000 },
        { "tvout", 0x39400000, 0x00100000 },
        { "dsim", 0x39500000, 0x00100000 },
        { "rgbout", 0x39600000, 0x00100000 },
        { "dp", 0x39700000, 0x00100000 },
        { "dither1", 0x39800000, 0x00100000 },
        { "lpdp", 0x39900000, 0x00100000 },
        { "isp", 0x3A000000, 0x00100000 },
        { "disp0", 0x3A100000, 0x00100000 },
        { "disp1", 0x3A200000, 0x00100000 },
        { "rt_dart", 0x3A400000, 0x00100000 },
        { "rt_top_widgets", 0x3A500000, 0x00100000 },
        { "rt_top_pl301", 0x3A600000, 0x00100000 },
        { "adsp", 0x3B000000, 0x00100000 },
        { "amp", 0x3D000000, 0x00100000 },
        { "scc", 0x3D200000, 0x00100000 },
        { "vrom", 0x3F000000, 0x00010000 },
        { "pmgr", 0x3F100000, 0x00100000 },
        { "aic", 0x3F200000, 0x00100000 },
        { "iop", 0x3F300000, 0x00100000 },
        { "rosc", 0x3F400000, 0x00100000 },
        { "chipid", 0x3F500000, 0x00100000 },
        { "swi", 0x3F600000, 0x00100000 },
        { "dwi", 0x3F700000, 0x00100000 },
        { "amc", 0x3F800000, 0x00100000 },
        { "gpio", 0x3FA00000, 0x00100000 },
        { "pio", 0x3FB00000, 0x00100000 },
        { "cim", 0x3FD00000, 0x00100000 },
        { "cdio_widgets", 0x3FE00000, 0x00100000 },
        { "cdio_pl301", 0x3FF00000, 0x00100000 },
        { "sdram", 0x80000000, 0x40000000 },
};

static void s5l8950x_init(Object *obj)
{
    S5L8950XState *s = S5L8950X(obj);

    s->memmap = s5l8950x_memmap;

    for (int i = 0; i < S5L8950X_NUM_CPUS; i++) {
        object_initialize_child(obj, "cpu[*]", &s->cpu[i], ARM_CPU_TYPE_NAME("cortex-a15"));
    }

    object_initialize_child(obj, "aic", &s->aic, TYPE_S5L8950X_AIC);
    for (int i = 0; i < S5L8950X_NUM_SPI; i++) {
        object_initialize_child(obj, "spi[*]", &s->spi[i], TYPE_S5L8950X_SPI);
    }
    object_initialize_child(obj, "gpio", &s->gpio, TYPE_S5L8950X_GPIO);
    object_initialize_child(obj, "pmgr", &s->pmgr, TYPE_S5L8950X_PMGR);
    object_initialize_child(obj, "chipid", &s->chipid, TYPE_S5L8950X_CHIPID);
}

static void s5l8950x_realize(DeviceState *dev, Error **errp)
{
    S5L8950XState *s = S5L8950X(dev);
    unsigned i;

    /* CPUs */
    for (i = 0; i < S5L8950X_NUM_CPUS; i++) {

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
    memory_region_add_subregion(get_system_memory(), s->memmap[S5L8950X_DEV_SRAM], &s->sram);

    /* VROM */
    memory_region_init_rom(&s->vrom, OBJECT(dev), "vrom", 0x00010000, &error_abort);
    memory_region_add_subregion(get_system_memory(), s->memmap[S5L8950X_DEV_VROM], &s->vrom);

    memory_region_init_alias(&s->vrom_alias, OBJECT(dev), "vram_alias", &s->vrom, 0, 0x00010000);
    memory_region_add_subregion(get_system_memory(), 0x0, &s->vrom_alias);

    /* AIC */
    sysbus_realize(SYS_BUS_DEVICE(&s->aic), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(&s->aic), 0, s->memmap[S5L8950X_DEV_AIC]);

    /* SPI */
    for (i = 0; i < S5L8950X_NUM_SPI; i++) {
        sysbus_realize(SYS_BUS_DEVICE(&s->spi[i]), &error_fatal);
        sysbus_mmio_map(SYS_BUS_DEVICE(&s->spi[i]), 0, s->memmap[S5L8950X_DEV_SPI0 + i]);
    }

    /* GPIO */
    sysbus_realize(SYS_BUS_DEVICE(&s->gpio), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(&s->gpio), 0, s->memmap[S5L8950X_DEV_GPIO]);

    /* PMGR */
    sysbus_realize(SYS_BUS_DEVICE(&s->pmgr), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(&s->pmgr), 0, s->memmap[S5L8950X_DEV_PMGR]);

    /* Chip ID */
    sysbus_realize(SYS_BUS_DEVICE(&s->chipid), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(&s->chipid), 0, s->memmap[S5L8950X_DEV_CHIPID]);

    /* Unimplemented devices */
    for (i = 0; i < ARRAY_SIZE(unimplemented); i++) {
        create_unimplemented_device(unimplemented[i].device_name, unimplemented[i].base, unimplemented[i].size);
    }
}

static void s5l8950x_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);

    dc->realize = s5l8950x_realize;
}

static const TypeInfo s5l8950x_type_info = {
    .name = TYPE_S5L8950X,
    .parent = TYPE_DEVICE,
    .instance_size = sizeof(S5L8950XState),
    .instance_init = s5l8950x_init,
    .class_init = s5l8950x_class_init,
};

static void s5l8950x_register_types(void)
{
    type_register_static(&s5l8950x_type_info);
}

type_init(s5l8950x_register_types)
