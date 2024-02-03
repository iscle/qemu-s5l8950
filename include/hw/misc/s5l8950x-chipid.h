/*
 * Apple A6 (S5L8950X) chip ID emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef HW_MISC_S5L8950X_CHIPID_H
#define HW_MISC_S5L8950X_CHIPID_H

#include "hw/sysbus.h"
#include "qom/object.h"

#define TYPE_S5L8950X_CHIPID   "s5l8950x-chipid"
OBJECT_DECLARE_SIMPLE_TYPE(S5L8950XChipIdState, S5L8950X_CHIPID)

struct S5L8950XChipIdState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion iomem;
};

#endif /* HW_MISC_S5L8950X_CHIPID_H */
