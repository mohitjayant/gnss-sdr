/*!
 * \file rtcm_test.cc
 * \brief  This file implements unit tests for the Rtcm class.
 * \author Carles Fernandez-Prades, 2015. cfernandez(at)cttc.es
 *
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2015  (see AUTHORS file for a list of contributors)
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
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#include <memory>
#include "rtcm.h"

TEST(Rtcm_Test, Hex_to_bin)
{
    auto  rtcm = std::make_shared<Rtcm>();

    std::string test1 = "2A";
    std::string test1_bin = rtcm->hex_to_bin(test1);
    EXPECT_EQ(0, test1_bin.compare("00101010"));

    std::string test2 = "FF";
    std::string test2_bin = rtcm->hex_to_bin(test2);
    EXPECT_EQ(0, test2_bin.compare("11111111"));

    std::string test3 = "ff";
    std::string test3_bin = rtcm->hex_to_bin(test3);
    EXPECT_EQ(0, test3_bin.compare("11111111"));

    std::string test4 = "100";
    std::string test4_bin = rtcm->hex_to_bin(test4);
    EXPECT_EQ(0, test4_bin.compare("000100000000"));

    std::string test5 = "1101";
    std::string test5_bin = rtcm->hex_to_bin(test5);
    EXPECT_EQ(0, test5_bin.compare("0001000100000001"));

    std::string test6 = "3";
    std::string test6_bin = rtcm->hex_to_bin(test6);
    EXPECT_EQ(0, test6_bin.compare("0011"));
}


TEST(Rtcm_Test, Bin_to_hex)
{
    auto  rtcm = std::make_shared<Rtcm>();

    std::string test1 = "00101010";
    std::string test1_hex = rtcm->bin_to_hex(test1);
    EXPECT_EQ(0, test1_hex.compare("2A"));

    std::string test2 = "11111111";
    std::string test2_hex = rtcm->bin_to_hex(test2);
    EXPECT_EQ(0, test2_hex.compare("FF"));

    std::string test4 = "000100000000";
    std::string test4_hex = rtcm->bin_to_hex(test4);
    EXPECT_EQ(0, test4_hex.compare("100"));

    std::string test5 = "0001000100000001";
    std::string test5_hex = rtcm->bin_to_hex(test5);
    EXPECT_EQ(0, test5_hex.compare("1101"));

    std::string test6 = "0011";
    std::string test6_hex = rtcm->bin_to_hex(test6);
    EXPECT_EQ(0, test6_hex.compare("3"));

    std::string test7 = "11";
    std::string test7_hex = rtcm->bin_to_hex(test7);
    EXPECT_EQ(0, test7_hex.compare("3"));

    std::string test8 = "1000100000001";
    std::string test8_hex = rtcm->bin_to_hex(test8);
    EXPECT_EQ(0, test8_hex.compare("1101"));
}



TEST(Rtcm_Test, Hex_to_int)
{
    auto rtcm = std::make_shared<Rtcm>();

    std::string test1 = "2A";
    long int test1_int = rtcm->hex_to_int(test1);
    long int expected1 = 42;
    EXPECT_EQ(expected1, test1_int);
}


TEST(Rtcm_Test, Bin_to_double)
{
    auto rtcm = std::make_shared<Rtcm>();

    std::bitset<4> test1(5);
    long int test1_int = static_cast<long int>(rtcm->bin_to_double(test1.to_string()));
    long int expected1 = 5;
    EXPECT_EQ(expected1, test1_int);

    std::bitset<4> test2(-5);
    EXPECT_DOUBLE_EQ(-5, rtcm->bin_to_double(test2.to_string()));

    std::bitset<65> test3(-5);
    EXPECT_DOUBLE_EQ(0, rtcm->bin_to_double(test3.to_string()));
}

TEST(Rtcm_Test, Test_Read_M1005)
{
    auto rtcm = std::make_shared<Rtcm>();
    std::string reference_msg = rtcm->print_M1005_test();

    unsigned int ref_id;
    double ecef_x;
    double ecef_y;
    double ecef_z;
    bool gps;
    bool glonass;
    bool galileo;

    rtcm->read_M1005(reference_msg, ref_id, ecef_x, ecef_y, ecef_z, gps, glonass, galileo);

    EXPECT_EQ(true, gps);
    EXPECT_EQ(false, glonass);
    EXPECT_EQ(false, galileo);

    EXPECT_EQ(2003, ref_id);
    EXPECT_DOUBLE_EQ(1114104.5999, ecef_x);
    EXPECT_DOUBLE_EQ(-4850729.7108, ecef_y);
    EXPECT_DOUBLE_EQ(3975521.4643, ecef_z);

    rtcm->read_M1005("D300133ED7D30202980EDEEF34B4BD62AC0941986F33360B98", ref_id, ecef_x, ecef_y, ecef_z, gps, glonass, galileo);

    EXPECT_EQ(true, gps);
    EXPECT_EQ(false, glonass);
    EXPECT_EQ(false, galileo);

    EXPECT_EQ(2003, ref_id);
    EXPECT_DOUBLE_EQ(1114104.5999, ecef_x);
    EXPECT_DOUBLE_EQ(-4850729.7108, ecef_y);
    EXPECT_DOUBLE_EQ(3975521.4643, ecef_z);
}

TEST(Rtcm_Test, Check_CRC)
{
    auto rtcm = std::make_shared<Rtcm>();
    EXPECT_EQ(true, rtcm->check_CRC("D300133ED7D30202980EDEEF34B4BD62AC0941986F33360B98"));
    EXPECT_EQ(false, rtcm->check_CRC("D300133ED7D30202980EDEEF34B4BD62AC0941986F33360B99"));

    EXPECT_EQ(true, rtcm->check_CRC(rtcm->print_M1005_test()));
    EXPECT_EQ(true, rtcm->check_CRC(rtcm->print_M1005_test()));
}
