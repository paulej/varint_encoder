/*
 *  varint_encoder.cpp
 *
 *  Copyright (C) 2023
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *  Description:
 *      This module implements functions that will perform variable-length
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

#pragma once

#include <cstddef>
#include <span>
#include <cstdint>

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
std::size_t Serialize(std::span<std::uint8_t> buffer, std::uint64_t value);

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
                        std::uint64_t &value);

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
std::size_t Serialize(std::span<std::uint8_t> buffer, std::int64_t value);

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
                        std::int64_t &value);

} // namespace VarIntEncoder
