/*
 * Apple A6 (S5L8950) GPIO emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef HW_MISC_S5L8950_GPIO_H
#define HW_MISC_S5L8950_GPIO_H

#include "hw/sysbus.h"
#include "qom/object.h"

#define TYPE_S5L8950_GPIO   "s5l8950-gpio"
OBJECT_DECLARE_SIMPLE_TYPE(S5L8950GpioState, S5L8950_GPIO)

struct S5L8950GpioState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion iomem;
};

#endif /* HW_MISC_S5L8950_GPIO_H */
