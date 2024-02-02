/*
 * Apple A6 (S5L8950) AIC emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef HW_MISC_S5L8950_AIC_H
#define HW_MISC_S5L8950_AIC_H

#include "hw/sysbus.h"
#include "qom/object.h"

#define TYPE_S5L8950_AIC   "s5l8950-aic"
OBJECT_DECLARE_SIMPLE_TYPE(S5L8950AicState, S5L8950_AIC)

struct S5L8950AicState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion iomem;
    uint32_t time_lo;
};

#endif /* HW_MISC_S5L8950_AIC_H */
