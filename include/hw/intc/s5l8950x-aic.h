/*
 * Apple A6 (S5L8950X) AIC emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef HW_MISC_S5L8950X_AIC_H
#define HW_MISC_S5L8950X_AIC_H

#include "hw/sysbus.h"
#include "qom/object.h"

#define TYPE_S5L8950X_AIC   "s5l8950x-aic"
OBJECT_DECLARE_SIMPLE_TYPE(S5L8950XAicState, S5L8950X_AIC)

struct S5L8950XAicState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion iomem;
    uint32_t time_lo;
};

#endif /* HW_MISC_S5L8950X_AIC_H */
