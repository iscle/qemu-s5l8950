/*
 * Apple A6 (S5L8950X) AIC emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/intc/s5l8950x-aic.h"
#include "migration/vmstate.h"
#include "sysemu/runstate.h"

// AIC_VERSION 1 for A6 (S5L8950X)
#define rAIC_TIME_LO    (0x0020)
#define rAIC_TIME_HI    (0x0028)

static uint64_t s5l8950x_aic_read(void *opaque, hwaddr offset,
                                      unsigned size)
{
    S5L8950XAicState *s = (S5L8950XAicState *)opaque;
    uint32_t res = 0;

    switch (offset) {
    case rAIC_TIME_LO:
        res = s->time_lo;
        if (s->time_lo == 0) {
            s->time_lo = 0x8FFFFFFF;
        } else {
            s->time_lo = 0;
        }
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950x_aic_read: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        res = 0;
        break;
    }

    return res;
}

static void s5l8950x_aic_write(void *opaque, hwaddr offset,
                                   uint64_t value, unsigned size)
{
//    S5L8950XAicState *s = (S5L8950XAicState *)opaque;

    switch (offset) {
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950x_aic_write: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        break;
    }
}

static const MemoryRegionOps s5l8950x_aic_ops = {
    .read = s5l8950x_aic_read,
    .write = s5l8950x_aic_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
//    .impl.min_access_size = 4,
//    .impl.max_access_size = 4,
};

//static const VMStateDescription vmstate_s5l8950x_aic = {
//    .name = TYPE_S5L8950X_AIC,
//    .version_id = 1,
//    .minimum_version_id = 1,
//    .fields = (VMStateField[]) {
//        VMSTATE_UINT32(rstc, S5L8950XAicState),
//        VMSTATE_UINT32(rsts, S5L8950XAicState),
//        VMSTATE_UINT32(wdog, S5L8950XAicState),
//        VMSTATE_END_OF_LIST()
//    }
//};

static void s5l8950x_aic_init(Object *obj)
{
    S5L8950XAicState *s = S5L8950X_AIC(obj);

    printf("s5l8950x_aic_init\n");
    memory_region_init_io(&s->iomem, obj, &s5l8950x_aic_ops, s, TYPE_S5L8950X_AIC, 0x100000);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->iomem);
}

static void s5l8950x_aic_reset(DeviceState *dev)
{
//    S5L8950XAicState *s = S5L8950X_AIC(dev);

    printf("s5l8950x_aic_reset\n");

//    s->rstc = 0x00000102;
//    s->rsts = 0x00001000;
//    s->wdog = 0x00000000;
}

static void s5l8950x_aic_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    printf("s5l8950x_aic_class_init\n");

    dc->reset = s5l8950x_aic_reset;
//    dc->vmsd = &vmstate_s5l8950x_aic;
}

static const TypeInfo s5l8950x_aic_info = {
    .name          = TYPE_S5L8950X_AIC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(S5L8950XAicState),
    .class_init    = s5l8950x_aic_class_init,
    .instance_init = s5l8950x_aic_init,
};

static void s5l8950x_aic_register_types(void)
{
    type_register_static(&s5l8950x_aic_info);
}

type_init(s5l8950x_aic_register_types)
