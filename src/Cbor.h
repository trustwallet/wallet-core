// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <string>
#include <vector>

namespace TW::Cbor {

/*
 * CBOR (Concise Binary Object Representation) encoding and decoding.
 * See  http://cbor.io  and   RFC 7049 https://tools.ietf.org/html/rfc7049
 */

/// CBOR-encoded data, encoder
/// See CborTests.cpp for usage.
class Encode {
public:
    Encode() {}
    /// Create from raw content, must be valid CBOR data, may throw
    Encode(const TW::Data& rawData);
    /// return encoded bytes
    TW::Data encoded() const { return data; }
    // builder/adder methods:
    /// encode an unsigned int
    static Encode uint(uint64_t value);
    /// encode a negative int (positive is given)
    static Encode negInt(uint64_t value);
    /// encode a string
    static Encode string(const std::string& str);
    /// encode a byte array
    static Encode bytes(const Data& str);
    /// encode an array of elements (of different types)
    static Encode array(const std::vector<Encode>& elems);
    /// encode a map
    static Encode map(const std::vector<std::pair<Encode, Encode>>& elems);
    /// encode a tag and following element
    static Encode tag(uint64_t value, const Encode& elem);
    /// Start an indefinite-length array
    static Encode indefArray();
    /// Add an element to indefinite-length array
    Encode addIDArrayElem(const Encode& elem);
    /// Close an indefinite-length array
    Encode closeIndefArray();

private:
    void appendValue(byte majorType, uint64_t value);
    void appendIndefinite(byte majorType);

private:
    TW::Data data;
    std::vector<int> indefElemCount = std::vector<int>();
};

/// CBOR-encoded data, decoder.  Read-only data or data slice.
/// See CborTests.cpp for usage.
class Decode {
private:
    const TW::byte* base;
    // Additional startIdxIdx index, to make skip ahead possible without touching the base data pointer
    uint32_t startIdx;
    uint32_t totlen;

public: // constructors
    Decode(const Data& input) : Decode((const TW::byte*)input.data(), input.size()) {}
    Decode(const TW::byte* ndata, uint32_t nlen);

public: // accessors
    uint32_t length() const { return totlen - startIdx; }
    TW::Data encoded() const { return TW::data(base + startIdx, totlen - startIdx); }

public: // decoding
    /// check if contains a valid CBOR byte stream
    bool isValid() const;
    /// Get the value of a simple type
    uint64_t getValue() const;
    /// Get the value of a string/bytes as string
    std::string getString() const;
    /// Get the value of a string/bytes as Data
    TW::Data getBytes() const;
    /// Get all elements of array
    std::vector<Decode> getArrayElements() const { return getCompoundElements(1, MT_array); }
    /// Get all elements of map
    std::vector<std::pair<Decode, Decode>> getMapElements() const;
    /// Get the tag number
    uint64_t getTagValue() const;
    /// Get the tag element
    Decode getTagElement() const;
    /// Dump to a JSON-like string (debugging)
    std::string dumpToString() const;

    enum MajorType {
        MT_uint = 0,
        MT_negint = 1,
        MT_bytes = 2,
        MT_string = 3,
        MT_array = 4,
        MT_map = 5,
        MT_tag = 6,
        MT_special = 7,
    };
    
private:
    Decode(const TW::byte* ndata, uint32_t nlen, uint32_t nStartIdx);
    // Skip ahead: from other Decode data with offset
    Decode skip(uint32_t offset) const;
    TW::byte byte(uint32_t idx) const {
        if (startIdx + idx >= totlen) { throw std::invalid_argument("CBOR data too short"); }
        return base[startIdx + idx];
    }
    struct TypeDesc {
        MajorType majorType; 
        TW::byte byteCount;
        uint64_t value;
        bool isIndefiniteValue;
    };
    TypeDesc getTypeDesc() const;
    uint32_t getTotalLen() const;
    uint32_t getCompoundLength(uint32_t countMultiplier) const;
    std::vector<Decode> getCompoundElements(uint32_t countMultiplier, TW::byte expectedType) const;
    bool isBreak() const;
    std::string dumpToStringInternal() const;
};

} // namespace TW::Cbor
