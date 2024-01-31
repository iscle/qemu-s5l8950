/*
 * iPhone 5 (N42AP) emulation
 *
 * Copyright (C) 2024 Iscle <albertiscle9@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/cutils.h"
#include "qapi/error.h"
#include "hw/arm/boot.h"
#include "hw/arm/s5l8950.h"
#include "hw/registerfields.h"
#include "qemu/error-report.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "hw/arm/boot.h"
#include "qom/object.h"

struct IphoneMachineState {
    /*< private >*/
    MachineState parent_obj;

    /*< public >*/
    S5L8950State soc;
    struct arm_boot_info binfo;
};
typedef struct IphoneMachineState IphoneMachineState;

struct IphoneMachineClass {
    /*< private >*/
    MachineClass parent_obj;

    /*< public >*/
};
typedef struct IphoneMachineClass IphoneMachineClass;

#define TYPE_IPHONE_MACHINE MACHINE_TYPE_NAME("iphone-common")
DECLARE_OBJ_CHECKERS(IphoneMachineState, IphoneMachineClass, IPHONE_MACHINE, TYPE_IPHONE_MACHINE)

static void setup_boot(MachineState *machine, size_t ram_size)
{
    IphoneMachineState *s = IPHONE_MACHINE(machine);
    int r;

    s->binfo.ram_size = ram_size;

    /* SDRAM */
    memory_region_add_subregion(get_system_memory(), s->soc.memmap[S5L8950_DEV_SDRAM], machine->ram);

    /* If the user specified a "firmware" image (e.g. BootROM), we bypass
     * the normal Linux boot process
     */
    if (machine->firmware) {
        printf("Loading firmware... File: %s\n", machine->firmware);
        hwaddr firmware_addr = s->soc.memmap[S5L8950_DEV_SRAM];
        /* load the firmware image */
        r = load_image_targphys(machine->firmware, firmware_addr, 64 * KiB);
        if (r < 0) {
            error_report("Failed to load firmware from %s", machine->firmware);
            exit(1);
        }

        s->binfo.entry = firmware_addr;
        s->binfo.firmware_loaded = true;
    } else {
        printf("Firmware is required!\n");
        exit(1);
    }

    arm_load_kernel(&s->soc.cpu[0], machine, &s->binfo);
}

static void iphone_machine_init(MachineState *machine)
{
//    IphoneMachineClass *mc = IPHONE_MACHINE_GET_CLASS(machine);
    IphoneMachineState *s = IPHONE_MACHINE(machine);

    if (machine->ram_size != 1 * GiB) {
        char *size_str = size_to_str(1 * GiB);
        error_report("Invalid RAM size, should be %s", size_str);
        g_free(size_str);
        exit(1);
    }

    /* SOC */
    object_initialize_child(OBJECT(machine), "soc", &s->soc, TYPE_S5L8950);
    qdev_realize(DEVICE(&s->soc), NULL, &error_fatal);

    setup_boot(machine, machine->ram_size);
}

static void iphone_machine_class_common_init(MachineClass *mc)
{
    mc->desc = g_strdup("Apple iPhone 5 (N42AP)");
    mc->init = iphone_machine_init;
    mc->default_cpus = mc->min_cpus = mc->max_cpus = 2;
    mc->default_ram_size = 1 * GiB;
    mc->default_ram_id = "ram";
};

static void n42ap_machine_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
//    IphoneMachineClass *imc = IPHONE_MACHINE_CLASS(oc);

    iphone_machine_class_common_init(mc);
}

static const TypeInfo iphone_machine_types[] = {
    {
        .name           = MACHINE_TYPE_NAME("iphone-n42ap"),
        .parent         = TYPE_IPHONE_MACHINE,
        .class_init     = n42ap_machine_class_init,
    }, {
        .name           = TYPE_IPHONE_MACHINE,
        .parent         = TYPE_MACHINE,
        .instance_size  = sizeof(IphoneMachineState),
        .class_size     = sizeof(IphoneMachineClass),
        .abstract       = true,
    }
};

DEFINE_TYPES(iphone_machine_types)
