/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Bastian Koppelmann
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/runtime.h"
#include "board.h"
#include "modpyb.h"

typedef struct _pyb_spi_obj_t {
    mp_obj_base_t base;
} pyb_spi_obj_t;

STATIC const pyb_spi_obj_t pyb_spi_obj[] = {
    {{&pyb_spi_type}},
    {{&pyb_spi_type}},
    {{&pyb_spi_type}},
};

#define NUM_LED MP_ARRAY_SIZE(pyb_spi_obj)
#define SPI_ID(obj) ((obj) - &pyb_spi_obj[0] + 1)

void pyb_spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    pyb_spi_obj_t *self = self_in;
    mp_printf(print, "SPI(%u)", SPI_ID(self));
}

STATIC mp_obj_t pyb_spi_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 1, false);
    mp_int_t spi_id = mp_obj_get_int(args[0]);
    if (!(1 <= spi_id && spi_id <= NUM_LED)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "LED %d does not exist", spi_id));
    }
    return (mp_obj_t)&pyb_spi_obj[spi_id - 1];
}

mp_obj_t pyb_spi_write(mp_obj_t self_in) {
    //pyb_spi_obj_t *self = self_in;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pyb_spi_write_obj, pyb_spi_write);

STATIC const mp_rom_map_elem_t pyb_spi_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&pyb_spi_write_obj) },
};

STATIC MP_DEFINE_CONST_DICT(pyb_spi_locals_dict, pyb_spi_locals_dict_table);

const mp_obj_type_t pyb_spi_type = {
    { &mp_type_type },
    .name = MP_QSTR_LED,
    .print = pyb_spi_print,
    .make_new = pyb_spi_make_new,
    .locals_dict = (mp_obj_t)&pyb_spi_locals_dict,
};
