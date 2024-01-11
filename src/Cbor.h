// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace TW::Cbor {

/*
 * CBOR (Concise Binary Object Representation) encoding and decoding.
 * See  http://cbor.io  and   RFC 7049 https://tools.ietf.org/html/rfc7049
 */

/// CBOR Encoder, and container for data being encoded.
/// See CborTests.cpp for usage.
class Encode {
public:
    /// Return encoded bytes
    TW::Data encoded() const;

    // Static state-less encoder methods:
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
    static Encode map(const std::map<Encode, Encode>& elems);
    /// encode a tag and following element
    static Encode tag(uint64_t value, const Encode& elem);
    /// encode a null value (special)
    static Encode null();

    /// Stateful building (for indefinite length)
    /// Start an indefinite-length array
    static Encode indefArray();
    /// Add an element to indefinite-length array
    Encode addIndefArrayElem(const Encode& elem);
    /// Close an indefinite-length array
    Encode closeIndefArray();

    /// Create from raw content, must be valid CBOR data, may throw
    static Encode fromRaw(const TW::Data& rawData);
    const Data& getDataInternal() const { return _data; }

private:
    Encode() {}
    Encode(const TW::Data& rawData) : _data(rawData) {}
    /// Append types + value, on variable number of bytes (1..8). Return object to support chain syntax.
    Encode appendValue(byte majorType, uint64_t value);
    inline Encode append(const TW::Data& data) { TW::append(_data, data); return *this; }
    void appendIndefinite(byte majorType);

private:
    /// Encoded data is stored here, always well-formed, but my be partial.
    TW::Data _data;
    /// number of currently open indefinite buildingds (0, 1, or more for nested)
    int openIndefCount = 0;
};

/// Comparator, needed for map keys
inline bool operator<(const Encode& lhs, const Encode& rhs) {
    return lhs.getDataInternal() < rhs.getDataInternal();
}

/// CBOR Decoder and container for data for decoding.  Contains reference to read-only CBOR data.
/// See CborTests.cpp for usage.
class Decode {
public:
    /// Constructor, create from CBOR byte stream
    Decode(const Data& input);

public: // decoding
    /// Check if contains a valid CBOR byte stream.
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
    uint32_t length() const { return subLen; }
    /// Return encoded form (useful e.g for parsed out sub-parts)
    Data encoded() const;

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
    /// Struct used to keep reference to original data
    struct OrigDataRef {
        Data origData;
        OrigDataRef(const Data& o) : origData(o) {}
    };
    Decode(const std::shared_ptr<OrigDataRef>& nData, uint32_t nSubIdx, uint32_t nSubLen);
    /// Skip ahead: form other Decode data with offset
    Decode skipClone(uint32_t offset) const;
    /// Get the Nth byte
    inline TW::byte getByte(uint32_t idx) const {
        if (subStart + idx >= data->origData.size()) { throw std::invalid_argument("CBOR data too short"); }
        return data->origData[subStart + idx];
    }
    struct TypeDesc {
        MajorType majorType = MT_uint;
        TW::byte byteCount = 0;
        uint64_t value = 0;
        bool isIndefiniteValue = false;
    };
    /// Parse out type sepcifiers
    TypeDesc getTypeDesc() const;
    uint32_t getTotalLen() const;
    uint32_t getCompoundLength(uint32_t countMultiplier) const;
    std::vector<Decode> getCompoundElements(uint32_t countMultiplier, TW::byte expectedType) const;
    bool isBreak() const;
    std::string dumpToStringInternal() const;

private:
    /// Reference to raw data, to the whole orginal, smart ptr
    std::shared_ptr<OrigDataRef> data;
    // Additional substring start and len, to make skip ahead possible without touching the base data pointer
    uint32_t subStart;
    uint32_t subLen;
};

} // namespace TW::Cbor
