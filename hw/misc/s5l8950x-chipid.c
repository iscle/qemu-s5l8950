/*
 * Apple A6 (S5L8950X) chip ID emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/misc/s5l8950x-chipid.h"
#include "migration/vmstate.h"
#include "sysemu/runstate.h"

#define	rCFG_FUSE0                                      (0x00)
#define CFG_FUSE0_PRODUCTION_MODE                       (1 << 0)
#define CFG_FUSE0_SECURE_MODE                           (1 << 1)
#define CFG_FUSE0_SECURITY_DOMAIN                       (1 << 2)
#define CFG_FUSE0_SECURITY_DOMAIN_MASK                  (0x3 << 2)
#define CFG_FUSE0_BOARD_ID                              (1 << 4)
#define CFG_FUSE0_BOARD_ID_MASK                         (0x3 << 4)
#define CFG_FUSE0_ECID_IMAGE_PERSONALIZATION_REQUIRED   (1 << 7)
#define CFG_FUSE0_MINIMUM_EPOCH                         (1 << 9)
#define CFG_FUSE0_MINIMUM_EPOCH_MASK                    (0x7F << 9)

#define	rCFG_FUSE1      (0x04)
#define	rCFG_FUSE2      (0x08)
#define	rCFG_FUSE3      (0x0C)
#define	rCFG_FUSE4      (0x10)
#define	rCFG_FUSE5      (0x14)

#define	rECIDLO         (0x20)
#define	rECIDHI         (0x24)

#define rDVFM_FUSE(n)   (0x40 + (n) * 4)

#define rSCC_FUSE(n)    (0x80 + (n) * 4)

static uint64_t s5l8950x_chipid_read(void *opaque, hwaddr offset,
                                      unsigned size)
{
//    S5L8950XChipIdState *s = (S5L8950XChipIdState *)opaque;
    uint32_t res = 0;

    switch (offset) {
    case rCFG_FUSE0:
        printf("s5l8950x_chipid_read: rCFG_FUSE0\n");
        res = 0x00000000;
        break;
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950x_chipid_read: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        res = 0;
        break;
    }

    return res;
}

static void s5l8950x_chipid_write(void *opaque, hwaddr offset,
                                   uint64_t value, unsigned size)
{
//    S5L8950XChipIdState *s = (S5L8950XChipIdState *)opaque;

    switch (offset) {
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950x_chipid_write: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        break;
    }
}

static const MemoryRegionOps s5l8950x_chipid_ops = {
    .read = s5l8950x_chipid_read,
    .write = s5l8950x_chipid_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
//    .impl.min_access_size = 4,
//    .impl.max_access_size = 4,
};

//static const VMStateDescription vmstate_s5l8950x_chipid = {
//    .name = TYPE_S5L8950X_CHIPID,
//    .version_id = 1,
//    .minimum_version_id = 1,
//    .fields = (VMStateField[]) {
//        VMSTATE_UINT32(rstc, S5L8950XChipIdState),
//        VMSTATE_UINT32(rsts, S5L8950XChipIdState),
//        VMSTATE_UINT32(wdog, S5L8950XChipIdState),
//        VMSTATE_END_OF_LIST()
//    }
//};

static void s5l8950x_chipid_init(Object *obj)
{
    S5L8950XChipIdState *s = S5L8950X_CHIPID(obj);

    printf("s5l8950x_chipid_init\n");
    memory_region_init_io(&s->iomem, obj, &s5l8950x_chipid_ops, s, TYPE_S5L8950X_CHIPID, 0x100000);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->iomem);
}

static void s5l8950x_chipid_reset(DeviceState *dev)
{
//    S5L8950XChipIdState *s = S5L8950X_CHIPID(dev);

    printf("s5l8950x_chipid_reset\n");

//    s->rstc = 0x00000102;
//    s->rsts = 0x00001000;
//    s->wdog = 0x00000000;
}

static void s5l8950x_chipid_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    printf("s5l8950x_chipid_class_init\n");

    dc->reset = s5l8950x_chipid_reset;
//    dc->vmsd = &vmstate_s5l8950x_chipid;
}

static const TypeInfo s5l8950x_chipid_info = {
    .name          = TYPE_S5L8950X_CHIPID,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(S5L8950XChipIdState),
    .class_init    = s5l8950x_chipid_class_init,
    .instance_init = s5l8950x_chipid_init,
};

static void s5l8950x_chipid_register_types(void)
{
    type_register_static(&s5l8950x_chipid_info);
}

type_init(s5l8950x_chipid_register_types)
