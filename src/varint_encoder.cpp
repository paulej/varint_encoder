/*
 *  varint_encoder.cpp
 *
 *  Copyright (C) 2023
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *  Description:
 *  Description:
 *      This module defines functions that will perform variable-length
 *      integer encoding into a buffer.  Integers are encoding using a scheme
 *      where the MSb of an octet indicates whether this is the final octet
 *      or whether an additional octet follows.  The balance of the 7 bits
 *      in the octet are actual bits of the serialized integer.
 *
 *      THe following is an example integer serialized:
 *
 *      10000011 11111111 01111111
 *      ^        ^        ^--- 0 == final octet
 *
 *      The '^' character marks the position of the bit indicating whether the
 *      next octet contains additional bits of the integer or not.
 *
 *  Portability Issues:
 *      None.
 */

#include <cstdint>
#include <cstddef>
#include <span>

#include "varint_encoder.h"

namespace
{

/*
 *  FindMSb()
 *
 *  Description:
 *      This function will find the most significant bit in the given integer,
 *      with a value in the range of 0 to n, where n is the number of bits
 *      in the integer.  If the value of the integer is 0, the MSb value
 *      returned is 0.  It is the caller's responsibility to check that the
 *      integer has a non-zero value.
 *
 *  Parameters:
 *      v [in]
 *          The value for which the most significant bit position is sought.
 *
 *  Returns:
 *      The bit position having the most significant bit set to 1, with the
 *      range being 0 to n, where n is the number of bits in the integer.
 *      If the integer has a 0 value, this function will return 0.
 *
 *  Comments:
 *      None.
 */
constexpr std::size_t FindMSb(std::uint64_t v)
{
    std::size_t p = 0;

    if (v >= std::uint64_t(1) << 32) p += 32, v >>= 32;
    if (v >= std::uint64_t(1) << 16) p += 16, v >>= 16;
    if (v >= std::uint64_t(1) << 8) p += 8, v >>= 8;
    if (v >= std::uint64_t(1) << 4) p += 4, v >>= 4;
    if (v >= std::uint64_t(1) << 2) p += 2, v >>= 2;
    if (v >= std::uint64_t(1) << 1) p += 1, v >>= 1;

    return p;
}

/*
 *  FindMSb()
 *
 *  Description:
 *      This function will find the most significant bit in the given integer.
 *      This function operates on signed integers and behaves differently
 *      than the parallel function that operates on unsigned integers.  If
 *      the integer value is non-negative, it will locate the bit postion having
 *      the most significant bit value 1.  If the integer is negative, however,
 *      it will seek the bit position having the most significant bit position
 *      set to 0.  To understand why, consider that -1 is all 1s and -2 is all
 *      1s, except for bit position 0, which would contain a 0.  The value -3
 *      would be all 1s, except for bit position 1 having a 0.  This function is
 *      not seeking the "biggest value", but the "most significant" bit, and for
 *      negative numbers that is indicated by a 0 value bit.
 *
 *  Parameters:
 *      v [in]
 *          The value for which the most significant bit position is sought.
 *
 *  Returns:
 *      The bit position having the most significant bit set to 0 or 1,
 *      depending on whether the integer is negative or non-negative.  A
 *      value of -1, 0, or 1 will all return 0.
 *
 *  Comments:
 *      None.
 */
constexpr std::size_t FindMSb(std::int64_t v)
{
    return ((v >= 0) ? FindMSb(static_cast<std::uint64_t>(v)) :
                       FindMSb(static_cast<std::uint64_t>(~v)));
}

/*
 *  VarUintSize()
 *
 *  Description:
 *      This function will return the number of octets required to encode
 *      the given variable-width unsigned integer.
 *
 *  Parameters:
 *      value [in]
 *          The value of the variable width unsigned integer.
 *
 *  Returns:
 *      The number of octets required to encode the given variable-width
 *      integer.
 *
 *  Comments:
 *      None.
 */
constexpr std::size_t VarUintSize(const std::uint64_t value)
{
    return FindMSb(value) / 7 + 1;
}

/*
 *  DataBuffer::VarIntSize()
 *
 *  Description:
 *      This function will return the number of octets required to encode
 *      the given variable-width signed integer.
 *
 *  Parameters:
 *      value [in]
 *          The value of the variable width signed integer.
 *
 *  Returns:
 *      The number of octets required to encode the given variable-width
 *      integer.
 *
 *  Comments:
 *      None.
 */
constexpr std::size_t VarIntSize(const std::int64_t &value)
{
    return (FindMSb(value) + 1) / 7 + 1;
}

} // anonymous namespace

namespace VarIntEncoder
{

/*
 *  Serialize()
 *
 *  Description:
 *      This function will serialize the given value into the buffer using
 *      variable-length integer encoding.
 *
 *  Parameters:
 *      buffer [out]
 *          The buffer into which to serialize the integer.
 *
 *      value [in]
 *          The value to insert into the data buffer.
 *
 *  Returns:
 *      The number of octets required to serialize the variable-width unsigned
 *      integer, or zero if there was an error.
 *
 *  Comments:
 *      None.
 */
std::size_t Serialize(std::span<std::uint8_t> buffer, std::uint64_t value)
{
    // Determine space requirements for the variable-width integer
    const std::size_t octets_required = VarUintSize(value);

    // Ensure the buffer is of sufficient length
    if (buffer.size() < octets_required) return 0;

    // Write octets from right to left (reverse order)
    for (std::size_t i = octets_required; i > 0; i--)
    {
        // Get the group of 7 bits
        std::uint8_t octet = value & 0x7f;

        // Shift the data bits vector by 7 bits
        value >>= 7;

        // If this is not the last octet, set the MSb to 1
        if (i != octets_required) octet |= 0x80;

        // Write the value into the buffer
        buffer[i - 1] = octet;
    }

    return octets_required;
}

/*
 *  Deserialize()
 *
 *  Description:
 *      This function will deserialize the variable-length integer that is
 *      encoded in the given buffer.
 *
 *  Parameters:
 *      buffer [in]
 *          The buffer from which to deserialize the integer.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets deserialized from the buffer.  A zero indicates
 *      there was a deserialization error.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserialize(const std::span<std::uint8_t> buffer,
                        std::uint64_t &value)
{
    std::uint8_t octet{0x80};
    std::size_t total_octets{0};

    // Initialize the integer value
    value = 0;

    // Read octets until we find the last one having a 0 MSb
    while (octet & 0x80)
    {
        // A 64-bits value should never require more than 10 octets
        if (++total_octets == 11) return 0;

        // Ensure we do not read beyond the buffer
        if (total_octets > buffer.size()) return 0;

        // Get the target octet
        octet = buffer[total_octets - 1];

        // Add these bits to the returned value
        value = (value << 7) | (octet & 0x7f);
    }

    // If the total length is 10 octets, initial octet must be 0x81
    if ((total_octets == 10) && (buffer[0] != 0x81)) return 0;

    return total_octets;
}

/*
 *  Serialize()
 *
 *  Description:
 *      This function will serialize the given value into the buffer using
 *      variable-length integer encoding.
 *
 *  Parameters:
 *      buffer [out]
 *          The buffer into which to serialize the integer.
 *
 *      value [in]
 *          The value to insert into the data buffer.
 *
 *  Returns:
 *      The number of octets required to serialize the variable-width signed
 *      integer, or zero if there was an error.
 *
 *  Comments:
 *      None.
 */
std::size_t Serialize(std::span<std::uint8_t> buffer, std::int64_t value)
{
    // Determine space requirements for the variable-width integer
    std::size_t octets_required = VarIntSize(value);

    // Ensure there is sufficient space in the buffer
    if (octets_required > buffer.size()) return 0;

    // Write octets from right to left (reverse order)
    for (std::size_t i = octets_required; i > 0; i--)
    {
        // Get the group of 7 bits
        std::uint8_t octet = value & 0x7f;

        // Shift the data bits vector by 7 bits
        value >>= 7;

        // If this is not the last octet, set the MSb to 1
        if (i != octets_required) octet |= 0x80;

        // Write the value into the buffer
        buffer[i - 1] = octet;
    }

    return octets_required;
}

/*
 *  Deserialize()
 *
 *  Description:
 *      This function will deserialize the variable-length integer that is
 *      encoded in the given buffer.
 *
 *  Parameters:
 *      buffer [in]
 *          The buffer from which to deserialize the integer.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets deserialized from the buffer.  A zero indicates
 *      there was a deserialization error.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserialize(const std::span<std::uint8_t> buffer,
                        std::int64_t &value)
{
    std::uint8_t octet{0x80};
    std::size_t total_octets{0};

    // Ensure we do not read beyond the buffer
    if (buffer.empty()) return 0;

    // Determine the sign of the number by inspecting the leading sign bit
    value = (buffer[0] & 0x40) ? -1 : 0;

    // Read octets until we find the last one having a 0 MSb
    while (octet & 0x80)
    {
        // A 64-bits value should never require more than 10 octets
        if (++total_octets == 11) return 0;

        // Ensure we do not read beyond the buffer
        if ((total_octets) > buffer.size()) return 0;

        // Get the target octet
        octet = buffer[total_octets - 1];

        // Add these bits to the returned value
        value = (value << 7) | (octet & 0x7f);
    }

    // If the total length is 10 octets, ensure the initial octet is one
    // of the only two valid values
    if ((total_octets == 10) && (buffer[0] != 0x80) && (buffer[0] != 0xff))
    {
        return 0;
    }

    return total_octets;
}

} // namespace VarIntEncoder
