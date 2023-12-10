# Variable Length Integer Encoding Library

## Background

There are several methods for variable-length encoding of integers for
transmission over a network. The most recent one I encountered was defined in
section 16 of the [QUIC](https://www.packetizer.com/rfc/rfc9000/) protocol
specification. In that document, 64-bit integers are encoded as 1, 2, 4, or 8
octets based on the value of the integer. Another format is specified in
Appendix B of the [CBOR](https://www.packetizer.com/rfc/rfc8949) specification,
which specifies how to encode any one of the typical integer sizes commonly
used on modern computers today. Using a rigid encoding like these offers a
means of quickly serializing data, but at a cost of increasing the number of
octets required. In the case of CBOR, it perhaps isn’t accurate to refer to it
as a variable-length encoding, but rather an encoding for each possible signed
and unsigned integer type. However, the effect is more-or-less similar, though
larger integers are encoded with less space efficiency.

When considering space-efficiency, one encoding approach is to encode integers
so that the MSb of each serialized octet indicates whether this is the final
octet or whether there is another octet to consume. The following illustrates
that idea:

```text
10000011 11111111 01111111
^        ^        ^--- 0 == final octet
```

Here, if a 1 is present as the MSb, it means the next octet is a part of the
integer. If the MSb is a 0, it indicates this octet is the last octet in the
serialized sequence of octets. Following this method, a value between 0 and 127
can be serialized into a single octet, while incrementally larger integers
consume additional octets. This method can be used for both unsigned and signed
integers, where the signed integers are stored in twos-complement format.

Serializing and deserializing data can be efficient. This library has functions
that will perform those operations on a buffer given an unsigned or signed
64-bit integer. This code uses C++ `constexpr` functions, though these could
easily be transformed into C macros if one prefers those.

## Library Functions

The library is composed of just a few functions to perform variable-length
encoding of integers as described above.  Those functions are:

* VarIntEncoder::Serialize()
* VarIntEncoder::Deserialize()

There are two forms of each, one accepting an unsigned integer and one
accepting a signed integer to be serialized or deserialized.  They are
fully documented in the varint_encoder.h header file.
