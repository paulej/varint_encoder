/*
 *  test_varint_encoder.cpp
 *
 *  Copyright (C) 2023
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *  Description:
 *      This test module will test the variable integer encoding and
 *      decoding functions.
 *
 *  Portability Issues:
 *      None.
 */

#include <cstdint>
#include <array>
#include <limits>
#include <cstddef>
#include <varint_encoder.h>
#include <stf/stf.h>

using namespace VarIntEncoder;

STF_TEST(VariableEncoder, EncodeUnsignedInteger)
{
    std::uint64_t value;
    std::uint64_t value2;
    std::array<std::uint8_t, 128> buffer;

    // Initialize the buffer
    for (std::size_t i = 0; i < buffer.size(); i++) buffer[i] = 0x22;

    // Single octet tests
    value = 0x00;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x00, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x01;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x01, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x40;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x40, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Two octet test
    value = 0x80;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x00, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x100;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x82, buffer[0]);
    STF_ASSERT_EQ(0x00, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x1000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xa0, buffer[0]);
    STF_ASSERT_EQ(0x00, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x2000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xc0, buffer[0]);
    STF_ASSERT_EQ(0x00, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Three octet test
    value = 0x4000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x00, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x4001;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x01, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x10'0000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xc0, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x00, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Four octet test
    value = 0x20'0000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x00, buffer[3]);
    STF_ASSERT_EQ(0x22, buffer[4]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Nine octet test
    value = 0x4000000000000000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xc0, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x80, buffer[3]);
    STF_ASSERT_EQ(0x80, buffer[4]);
    STF_ASSERT_EQ(0x80, buffer[5]);
    STF_ASSERT_EQ(0x80, buffer[6]);
    STF_ASSERT_EQ(0x80, buffer[7]);
    STF_ASSERT_EQ(0x00, buffer[8]);
    STF_ASSERT_EQ(0x22, buffer[9]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Ten octet test
    value = 0x8000000000000000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x80, buffer[3]);
    STF_ASSERT_EQ(0x80, buffer[4]);
    STF_ASSERT_EQ(0x80, buffer[5]);
    STF_ASSERT_EQ(0x80, buffer[6]);
    STF_ASSERT_EQ(0x80, buffer[7]);
    STF_ASSERT_EQ(0x80, buffer[8]);
    STF_ASSERT_EQ(0x00, buffer[9]);
    STF_ASSERT_EQ(0x22, buffer[10]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Largest value
    value = 0xffff'ffff'ffff'ffff;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0xff, buffer[1]);
    STF_ASSERT_EQ(0xff, buffer[2]);
    STF_ASSERT_EQ(0xff, buffer[3]);
    STF_ASSERT_EQ(0xff, buffer[4]);
    STF_ASSERT_EQ(0xff, buffer[5]);
    STF_ASSERT_EQ(0xff, buffer[6]);
    STF_ASSERT_EQ(0xff, buffer[7]);
    STF_ASSERT_EQ(0xff, buffer[8]);
    STF_ASSERT_EQ(0x7f, buffer[9]);
    STF_ASSERT_EQ(0x22, buffer[10]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);
}

STF_TEST(VariableEncoder, EncodePositiveInteger)
{
    std::int64_t value;
    std::int64_t value2;
    std::array<std::uint8_t, 128> buffer;

    // Initialize the buffer
    for (std::size_t i = 0; i < buffer.size(); i++) buffer[i] = 0x22;

    // Single octet tests
    value = 0x00;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x00, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x01;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x01, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x20;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x20, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Two octet test
    value = 0x40;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x80, buffer[0]);
    STF_ASSERT_EQ(0x40, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x80;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x00, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x100;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x82, buffer[0]);
    STF_ASSERT_EQ(0x00, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x1000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xA0, buffer[0]);
    STF_ASSERT_EQ(0x00, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Three octet test
    value = 0x2000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x80, buffer[0]);
    STF_ASSERT_EQ(0xc0, buffer[1]);
    STF_ASSERT_EQ(0x00, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x4000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x00, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x8'0000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xa0, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x00, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Four octet test
    value = 0x10'0000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x80, buffer[0]);
    STF_ASSERT_EQ(0xc0, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x00, buffer[3]);
    STF_ASSERT_EQ(0x22, buffer[4]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x20'0000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x81, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x00, buffer[3]);
    STF_ASSERT_EQ(0x22, buffer[4]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = 0x40'0000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x82, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x00, buffer[3]);
    STF_ASSERT_EQ(0x22, buffer[4]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Nine octet test
    value = 0x2000000000000000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xa0, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x80, buffer[3]);
    STF_ASSERT_EQ(0x80, buffer[4]);
    STF_ASSERT_EQ(0x80, buffer[5]);
    STF_ASSERT_EQ(0x80, buffer[6]);
    STF_ASSERT_EQ(0x80, buffer[7]);
    STF_ASSERT_EQ(0x00, buffer[8]);
    STF_ASSERT_EQ(0x22, buffer[9]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Ten octet test
    value = 0x4000000000000000;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x80, buffer[0]);
    STF_ASSERT_EQ(0xc0, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x80, buffer[3]);
    STF_ASSERT_EQ(0x80, buffer[4]);
    STF_ASSERT_EQ(0x80, buffer[5]);
    STF_ASSERT_EQ(0x80, buffer[6]);
    STF_ASSERT_EQ(0x80, buffer[7]);
    STF_ASSERT_EQ(0x80, buffer[8]);
    STF_ASSERT_EQ(0x00, buffer[9]);
    STF_ASSERT_EQ(0x22, buffer[10]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Test largest signed integer
    value = std::numeric_limits<std::int64_t>::max();
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x80, buffer[0]);
    STF_ASSERT_EQ(0xff, buffer[1]);
    STF_ASSERT_EQ(0xff, buffer[2]);
    STF_ASSERT_EQ(0xff, buffer[3]);
    STF_ASSERT_EQ(0xff, buffer[4]);
    STF_ASSERT_EQ(0xff, buffer[5]);
    STF_ASSERT_EQ(0xff, buffer[6]);
    STF_ASSERT_EQ(0xff, buffer[7]);
    STF_ASSERT_EQ(0xff, buffer[8]);
    STF_ASSERT_EQ(0x7f, buffer[9]);
    STF_ASSERT_EQ(0x22, buffer[10]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);
}

STF_TEST(VariableEncoder, EncodeNegativeInteger)
{
    std::int64_t value;
    std::int64_t value2;
    std::array<std::uint8_t, 128> buffer;

    // Initialize the buffer
    for (std::size_t i = 0; i < buffer.size(); i++) buffer[i] = 0x22;

    // Single octet tests
    value = -1;
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x7f, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = -33; // 0xFFFFFFFFFFFFFFDF
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0x5f, buffer[0]);
    STF_ASSERT_EQ(0x22, buffer[1]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Two octet tests
    value = -65; // 0xFFFFFFFFFFFFFFBF
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xff, buffer[0]);
    STF_ASSERT_EQ(0x3f, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = -129; // 0xFFFFFFFFFFFFFFBF
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xfe, buffer[0]);
    STF_ASSERT_EQ(0x7f, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = -4097; // 0xFFFFFFFFFFFFEFFF
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xdf, buffer[0]);
    STF_ASSERT_EQ(0x7f, buffer[1]);
    STF_ASSERT_EQ(0x22, buffer[2]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Three octet tests
    value = -8193; // 0xFFFFFFFFFFFFDFFF
     STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xff, buffer[0]);
    STF_ASSERT_EQ(0xbf, buffer[1]);
    STF_ASSERT_EQ(0x7f, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = -16385; // 0xFFFFFFFFFFFFBFFF
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xfe, buffer[0]);
    STF_ASSERT_EQ(0xff, buffer[1]);
    STF_ASSERT_EQ(0x7f, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    value = -32769; // 0xFFFFFFFFFFFF7FFF
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xfd, buffer[0]);
    STF_ASSERT_EQ(0xff, buffer[1]);
    STF_ASSERT_EQ(0x7f, buffer[2]);
    STF_ASSERT_EQ(0x22, buffer[3]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Nine octet test
    value = 0xDFFFFFFFFFFFFFFF; // Bit 61 (0..63)
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xdf, buffer[0]);
    STF_ASSERT_EQ(0xff, buffer[1]);
    STF_ASSERT_EQ(0xff, buffer[2]);
    STF_ASSERT_EQ(0xff, buffer[3]);
    STF_ASSERT_EQ(0xff, buffer[4]);
    STF_ASSERT_EQ(0xff, buffer[5]);
    STF_ASSERT_EQ(0xff, buffer[6]);
    STF_ASSERT_EQ(0xff, buffer[7]);
    STF_ASSERT_EQ(0x7f, buffer[8]);
    STF_ASSERT_EQ(0x22, buffer[9]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Ten octet test
    value = 0xBFFFFFFFFFFFFFFF; // Bit 62 (0..63)
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xff, buffer[0]);
    STF_ASSERT_EQ(0xbf, buffer[1]);
    STF_ASSERT_EQ(0xff, buffer[2]);
    STF_ASSERT_EQ(0xff, buffer[3]);
    STF_ASSERT_EQ(0xff, buffer[4]);
    STF_ASSERT_EQ(0xff, buffer[5]);
    STF_ASSERT_EQ(0xff, buffer[6]);
    STF_ASSERT_EQ(0xff, buffer[7]);
    STF_ASSERT_EQ(0xff, buffer[8]);
    STF_ASSERT_EQ(0x7f, buffer[9]);
    STF_ASSERT_EQ(0x22, buffer[10]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);

    // Test smallest signed integer
    value = std::numeric_limits<std::int64_t>::min();
    STF_ASSERT_NE(0, Serialize(buffer, value));
    STF_ASSERT_EQ(0xff, buffer[0]);
    STF_ASSERT_EQ(0x80, buffer[1]);
    STF_ASSERT_EQ(0x80, buffer[2]);
    STF_ASSERT_EQ(0x80, buffer[3]);
    STF_ASSERT_EQ(0x80, buffer[4]);
    STF_ASSERT_EQ(0x80, buffer[5]);
    STF_ASSERT_EQ(0x80, buffer[6]);
    STF_ASSERT_EQ(0x80, buffer[7]);
    STF_ASSERT_EQ(0x80, buffer[8]);
    STF_ASSERT_EQ(0x00, buffer[9]);
    STF_ASSERT_EQ(0x22, buffer[10]); // Should have no data
    STF_ASSERT_NE(0, Deserialize(buffer, value2));
    STF_ASSERT_EQ(value, value2);
}

STF_TEST(VariableEncoder, TestTooBig)
{
    std::uint64_t value;

    // This would be one octet too large
    std::array<std::uint8_t, 128> buffer =
        {0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};

    // Zero should be returned indicating failure
    STF_ASSERT_EQ(0, Deserialize(buffer, value));
}
