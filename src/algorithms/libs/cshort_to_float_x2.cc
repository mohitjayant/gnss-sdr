/*!
 * \file cshort_to_float_x2.cc
 * \brief Adapts a std::complex<short> stream into two float streams
 * \author Carles Fernandez Prades, cfernandez(at)cttc.es
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2019  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <https://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */


#include "cshort_to_float_x2.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>
#include <algorithm>  // for max


cshort_to_float_x2_sptr make_cshort_to_float_x2()
{
    return cshort_to_float_x2_sptr(new cshort_to_float_x2());
}


cshort_to_float_x2::cshort_to_float_x2() : sync_block("cshort_to_float_x2",
                                               gr::io_signature::make(1, 1, sizeof(lv_16sc_t)),  // lv_8sc_t is a Volk's typedef for std::complex<signed char>
                                               gr::io_signature::make(2, 2, sizeof(float)))
{
    const int alignment_multiple = volk_get_alignment() / sizeof(lv_16sc_t);
    set_alignment(std::max(1, alignment_multiple));
}


int cshort_to_float_x2::work(int noutput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items)
{
    const auto *in = reinterpret_cast<const lv_16sc_t *>(input_items[0]);
    auto *out0 = reinterpret_cast<float *>(output_items[0]);
    auto *out1 = reinterpret_cast<float *>(output_items[1]);
    const float scalar = 1;
    volk_16ic_s32f_deinterleave_32f_x2(out0, out1, in, scalar, noutput_items);
    return noutput_items;
}
