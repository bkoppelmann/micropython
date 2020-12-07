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

typedef struct _pyb_lmx2594_obj_t {
    mp_obj_base_t base;
    pyb_spi_obj_t *spi;
} pyb_lmx2594_obj_t;

void pyb_lmx2594_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    pyb_lmx2594_obj_t *self = self_in;
    mp_printf(print, "LMX(\n  cs pin = %u\n  spi freq = %u\n)", self->spi->cs_arduino, self->spi->freq);
}

STATIC mp_obj_t pyb_lmx2594_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 1, false);
    if (!MP_OBJ_IS_TYPE(args[0], &pyb_spi_type)) {
        mp_raise_TypeError("Requires type 'SPI'");
        return mp_const_none;
    }
    pyb_lmx2594_obj_t *self = m_new_obj(pyb_lmx2594_obj_t);
    self->base.type = &pyb_lmx2594_type;
    self->spi = args[0];

    return MP_OBJ_FROM_PTR(self);
}

uint16_t reg_map[113] =
{
      0x251C, 0x0808, 0x0500, 0x0642, 0x0A43,
      0x00C8, 0xC802, 0x40B2, 0x2000, 0x1604,
      0x10D8, 0x0018, 0x5001, 0x4000, 0x1E70,
      0x064F, 0x0080, 0x012C, 0x0064, 0x27B7,
      0xE048, 0x0401, 0x0001, 0x007C, 0x071A,
      0x0C2B, 0x0DB0, 0x0002, 0x0488, 0x318C,
      0x318C, 0x03EC, 0x0393, 0x1E21, 0x0000,
      0x0004, 0x0032, 0x0304, 0x0000, 0x03E8,
      0x0000, 0x0000, 0x0000, 0x0000, 0x1F23,
      0xC0DF, 0x07FC, 0x0300, 0x0300, 0x4180,
      0x0000, 0x0080, 0x0820, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0020, 0x8001, 0x0001,
      0x0000, 0x00A8, 0x0322, 0x0000, 0x1388,
      0x0000, 0x01F4, 0x0000, 0x03E8, 0x0000,
      0xC350, 0x0081, 0x0001, 0x003F, 0x0000,
      0x0800, 0x000C, 0x0000, 0x0003, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0888, 0x0000, 0x0000,
      0x0000, 0x0011, 0x0000, 0x0000, 0x0000,
      0x0021, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000
};

mp_obj_t pyb_lmx2594_reset(mp_obj_t self_in) {
//    pyb_lmx2594_obj_t *self = self_in;
//    union {
//        uint16_t raw;
//        uint8_t i8[2];
//    } data_conv;
//    for (int i = 112; i>=0 ; i--) {
//        data_conv.raw = reg_map[i];
//        pyb_spi_obj_t *spi = self->spi;
//        spi_write(spi->cs, i, data_conv.i8, 2);
//    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(pyb_lmx2594_reset_obj, pyb_lmx2594_reset);

STATIC const mp_rom_map_elem_t pyb_lmx2594_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_reset), MP_ROM_PTR(&pyb_lmx2594_reset_obj) }
};

STATIC MP_DEFINE_CONST_DICT(pyb_lmx2594_locals_dict, pyb_lmx2594_locals_dict_table);

const mp_obj_type_t pyb_lmx2594_type = {
    { &mp_type_type },
    .name = MP_QSTR_LMX2594,
    .print = pyb_lmx2594_print,
    .make_new = pyb_lmx2594_make_new,
    .locals_dict = (mp_obj_t)&pyb_lmx2594_locals_dict,
};
