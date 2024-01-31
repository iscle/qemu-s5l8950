/*
 * Apple A6 (S5L8950) Power Management emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/misc/s5l8950-pmgr.h"
#include "migration/vmstate.h"
#include "sysemu/runstate.h"

static uint64_t s5l8950_pmgr_read(void *opaque, hwaddr offset,
                                      unsigned size)
{
//    S5L8950PmgrState *s = (S5L8950PmgrState *)opaque;
    uint32_t res = 0;

    switch (offset) {
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950_pmgr_read: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        res = 0;
        break;
    }

    return res;
}

static void s5l8950_pmgr_write(void *opaque, hwaddr offset,
                                   uint64_t value, unsigned size)
{
//    S5L8950PmgrState *s = (S5L8950PmgrState *)opaque;

    switch (offset) {
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950_pmgr_write: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        break;
    }
}

static const MemoryRegionOps s5l8950_pmgr_ops = {
    .read = s5l8950_pmgr_read,
    .write = s5l8950_pmgr_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
//    .impl.min_access_size = 4,
//    .impl.max_access_size = 4,
};

//static const VMStateDescription vmstate_s5l8950_pmgr = {
//    .name = TYPE_S5L8950_PMGR,
//    .version_id = 1,
//    .minimum_version_id = 1,
//    .fields = (VMStateField[]) {
//        VMSTATE_UINT32(rstc, S5L8950PmgrState),
//        VMSTATE_UINT32(rsts, S5L8950PmgrState),
//        VMSTATE_UINT32(wdog, S5L8950PmgrState),
//        VMSTATE_END_OF_LIST()
//    }
//};

static void s5l8950_pmgr_init(Object *obj)
{
    S5L8950PmgrState *s = S5L8950_PMGR(obj);

    printf("s5l8950_pmgr_init\n");
    memory_region_init_io(&s->iomem, obj, &s5l8950_pmgr_ops, s, TYPE_S5L8950_PMGR, 0x100000);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->iomem);
}

static void s5l8950_pmgr_reset(DeviceState *dev)
{
//    S5L8950PmgrState *s = S5L8950_PMGR(dev);

    printf("s5l8950_pmgr_reset\n");

//    s->rstc = 0x00000102;
//    s->rsts = 0x00001000;
//    s->wdog = 0x00000000;
}

static void s5l8950_pmgr_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    printf("s5l8950_pmgr_class_init\n");

    dc->reset = s5l8950_pmgr_reset;
//    dc->vmsd = &vmstate_s5l8950_pmgr;
}

static const TypeInfo s5l8950_pmgr_info = {
    .name          = TYPE_S5L8950_PMGR,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(S5L8950PmgrState),
    .class_init    = s5l8950_pmgr_class_init,
    .instance_init = s5l8950_pmgr_init,
};

static void s5l8950_pmgr_register_types(void)
{
    type_register_static(&s5l8950_pmgr_info);
}

type_init(s5l8950_pmgr_register_types)
