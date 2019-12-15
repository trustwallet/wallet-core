// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <string>
#include <vector>

namespace TW::Cardano::Cbor {

// CBOR-encoded data, encoder
class Encode {
public:
    Encode() {}
    // Create from raw content
    Encode(const TW::Data& rawData) { data = rawData; }
    TW::Data getData() const { return data; }
    // builder/adder methods
    Encode addPInt(uint64_t value);
    Encode addString(const Data& string);
    Encode addArray(const std::vector<Encode>& elems);
    Encode addMap(const std::vector<std::pair<Encode, Encode>>& elems);
    Encode addTag(uint64_t value, const Encode& elem);
protected:
    void appendValue(byte majorType, uint64_t value);
private:
    TW::Data data;
};

// CBOR-encoded data, decoder.  Read-only data or data slice.
class Decode {
private:
    const TW::byte* base;
    // Additional startIdxIdx index, to make skip ahead possible without touching the base data pointer
    uint32_t startIdx;
    uint32_t totlen;
public: // constructors
    Decode(const TW::byte* ndata, uint32_t nlen);
    Decode(const Data& input);
    // Skip ahead: from other Decode data with offset
    Decode skip(uint32_t offset) const;
public: // accessors
    uint32_t length() const { return totlen - startIdx; }
    TW::byte byte(uint32_t idx) const {
        if (startIdx + idx >= totlen) { throw std::invalid_argument("CBOR data too short"); }
        return base[startIdx + idx];
    }
    TW::Data getData() const { return TW::data(base + startIdx, totlen - startIdx); }
public: // decoding helpers
    void getTypes(TW::byte& majorType, TW::byte& byteCount, uint64_t& value) const;
    uint32_t getSimpleTotalLen() const;
    std::vector<Decode> getCompoundElements(uint32_t countMultiplier, TW::byte expectedType) const;
public: // decoding
    // Get the value of a simple type
    uint64_t getValue() const;
    TW::Data getStringData() const;
    std::vector<Decode> getArrayElements() const { return getCompoundElements(1, 4); }
    std::vector<std::pair<Decode, Decode>> getMapElements() const;
    uint64_t getTagValue() const;
    Decode getTagElement() const;
    std::string dumpToString() const;
};

} // namespace TW::Cardano
