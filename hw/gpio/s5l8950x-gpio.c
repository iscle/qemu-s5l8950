/*
 * Apple A6 (S5L8950X) GPIO emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/gpio/s5l8950x-gpio.h"
#include "migration/vmstate.h"
#include "sysemu/runstate.h"

// S5L8950X has Apple GPIO_VERSION 2

// gpion = pad * GPIOPADPINS + pin

#define GPIOPADPINS     (8)

#define rGPIOCFG(_n)    (0x000 + (_n) * 4)
#define rGPIOINT(_n)    (0x800 + (_n) * 4)

#define GPIO(pad, pin)  ((pad) * GPIOPADPINS + (pin))

static uint64_t s5l8950x_gpio_read(void *opaque, hwaddr offset,
                                      unsigned size)
{
//    S5L8950XGpioState *s = (S5L8950XGpioState *)opaque;
    uint32_t res = 0;

    switch (offset) {
        case rGPIOCFG(GPIO(0, 0)):
            printf("s5l8950x_gpio_read: rGPIOCFG(GPIO_REQUEST_DFU2)\n");
            res = 0x00000001; // Return 1, button pull up
            break;
        case rGPIOCFG(GPIO(0, 1)):
            printf("s5l8950x_gpio_read: rGPIOCFG(GPIO_REQUEST_DFU1)\n");
            res = 0x00000001; // Return 1, button pull up
            break;
        case rGPIOCFG(GPIO(25, 6)):
            printf("s5l8950x_gpio_read: rGPIOCFG(GPIO_FORCE_DFU)\n");
            res = 0x00000000; // Return 0
            break;
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950x_gpio_read: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        res = 0;
        break;
    }

    return res;
}

static void s5l8950x_gpio_write(void *opaque, hwaddr offset,
                                   uint64_t value, unsigned size)
{
//    S5L8950XGpioState *s = (S5L8950XGpioState *)opaque;

    switch (offset) {
    default:
        qemu_log_mask(LOG_UNIMP, "s5l8950x_gpio_write: Unknown offset 0x%08"HWADDR_PRIx"\n", offset);
        break;
    }
}

static const MemoryRegionOps s5l8950x_gpio_ops = {
    .read = s5l8950x_gpio_read,
    .write = s5l8950x_gpio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
//    .impl.min_access_size = 4,
//    .impl.max_access_size = 4,
};

//static const VMStateDescription vmstate_s5l8950x_gpio = {
//    .name = TYPE_S5L8950X_GPIO,
//    .version_id = 1,
//    .minimum_version_id = 1,
//    .fields = (VMStateField[]) {
//        VMSTATE_UINT32(rstc, S5L8950XGpioState),
//        VMSTATE_UINT32(rsts, S5L8950XGpioState),
//        VMSTATE_UINT32(wdog, S5L8950XGpioState),
//        VMSTATE_END_OF_LIST()
//    }
//};

static void s5l8950x_gpio_init(Object *obj)
{
    S5L8950XGpioState *s = S5L8950X_GPIO(obj);

    printf("s5l8950x_gpio_init\n");
    memory_region_init_io(&s->iomem, obj, &s5l8950x_gpio_ops, s, TYPE_S5L8950X_GPIO, 0x100000);
    sysbus_init_mmio(SYS_BUS_DEVICE(s), &s->iomem);
}

static void s5l8950x_gpio_reset(DeviceState *dev)
{
//    S5L8950XGpioState *s = S5L8950X_GPIO(dev);

    printf("s5l8950x_gpio_reset\n");

//    s->rstc = 0x00000102;
//    s->rsts = 0x00001000;
//    s->wdog = 0x00000000;
}

static void s5l8950x_gpio_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    printf("s5l8950x_gpio_class_init\n");

    dc->reset = s5l8950x_gpio_reset;
//    dc->vmsd = &vmstate_s5l8950x_gpio;
}

static const TypeInfo s5l8950x_gpio_info = {
    .name          = TYPE_S5L8950X_GPIO,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(S5L8950XGpioState),
    .class_init    = s5l8950x_gpio_class_init,
    .instance_init = s5l8950x_gpio_init,
};

static void s5l8950x_gpio_register_types(void)
{
    type_register_static(&s5l8950x_gpio_info);
}

type_init(s5l8950x_gpio_register_types)
