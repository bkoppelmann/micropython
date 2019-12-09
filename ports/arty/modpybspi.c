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
#include <stdio.h>
#include "py/runtime.h"
#include "board.h"
#include "modpyb.h"

uint32_t pin_mapping[20] =
{
    16, 17, 18, 19, 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13
};

void pyb_spi_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    pyb_spi_obj_t *self = self_in;
    mp_printf(print, "SPI(\n  cs pin = %u\n  spi freq = %u\n)", self->cs_arduino, self->freq);
}

STATIC mp_obj_t pyb_spi_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 2, 2, false);
    mp_int_t cs_id = mp_obj_get_int(args[0]);
    mp_int_t max_freq = mp_obj_get_int(args[1]);

    if (cs_id > 19) {
        printf("ERROR: Unknown CS pin '%d'. Maximum is 19.\n", cs_id);
        return mp_const_none;
    }

    pyb_spi_obj_t *self = m_new_obj(pyb_spi_obj_t);
    self->base.type = &pyb_spi_type;
    self->freq = max_freq;
    self->cs_arduino = cs_id;
    self->cs = pin_mapping[cs_id];
    spi_begin(self->cs, max_freq);

    return MP_OBJ_FROM_PTR(self);
}

STATIC mp_obj_t pyb_spi_write(uint n_args, const mp_obj_t *args) {
    pyb_spi_obj_t *self = args[0];
    union {
        uint16_t raw;
        uint8_t array[2];
    } data_ptr;

    uint8_t addr = mp_obj_get_int(args[1]);
    data_ptr.raw = mp_obj_get_int(args[2]);

    spi_write(self->cs, addr, data_ptr.array, 2);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pyb_spi_write_obj, 3, 3, pyb_spi_write);

STATIC mp_obj_t pyb_spi_read(uint n_args, const mp_obj_t *args) {
    pyb_spi_obj_t *self = args[0];
    union {
        uint16_t raw;
        uint8_t array[2];
    } data_ptr;

    uint8_t addr = mp_obj_get_int(args[1]);
    spi_read(self->cs, addr, data_ptr.array, 2);

    return MP_OBJ_NEW_SMALL_INT(data_ptr.raw);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(pyb_spi_read_obj, 2, 2, pyb_spi_read);


STATIC const mp_rom_map_elem_t pyb_spi_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&pyb_spi_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&pyb_spi_read_obj) },
};

STATIC MP_DEFINE_CONST_DICT(pyb_spi_locals_dict, pyb_spi_locals_dict_table);

const mp_obj_type_t pyb_spi_type = {
    { &mp_type_type },
    .name = MP_QSTR_SPI,
    .print = pyb_spi_print,
    .make_new = pyb_spi_make_new,
    .locals_dict = (mp_obj_t)&pyb_spi_locals_dict,
};
