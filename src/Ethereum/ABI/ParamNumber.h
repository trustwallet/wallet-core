// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "Util.h"

#include "../../Data.h"
#include "../../uint256.h"

#include <string>
#include <cassert>

namespace TW::Ethereum::ABI {

inline void encode(uint256_t value, Data& data) {
    Data bytes = store(value);
    append(data, Data(Util::encodedUInt256Size - bytes.size()));
    append(data, bytes);
}

inline bool decode(const Data& encoded, uint256_t& decoded, size_t& offset_inout)
{
    decoded = 0u;
    if (encoded.empty() || (encoded.size() < (Util::encodedUInt256Size + offset_inout))) {
        return false;
    }
    decoded = loadWithOffset(encoded, offset_inout);
    offset_inout += Util::encodedUInt256Size;
    return true;
}

/// Generic parameter class for scalar types that fit into 256 bits.
template<typename T> 
class ParamNumber : public ParamBase
{
public:
    ParamNumber() = default;
    ParamNumber(T val) { _val = val; }
    void setVal(T val) { _val = val; }
    T getVal() const { return _val; }
    virtual std::string getType() const = 0;
    virtual size_t getSize() const { return Util::encodedUInt256Size; }
    virtual bool isDynamic() const { return false; }
    virtual void encode(Data& data) const {
        // cast up
        uint256_t val256 = static_cast<uint256_t>(_val);
        TW::Ethereum::ABI::encode(val256, data);
    }
    static bool decodeNumber(const Data& encoded, T& decoded, size_t& offset_inout) {
        uint256_t val256;
        if (!TW::Ethereum::ABI::decode(encoded, val256, offset_inout)) { return false; }
        // cast down
        decoded = static_cast<T>(val256);
        return true;
    }
    virtual bool decode(const Data& encoded, size_t& offset_inout) {
        return decodeNumber(encoded, _val, offset_inout);
    }
private:
    T _val;
};

class ParamUInt256 : public ParamNumber<uint256_t>
{
public:
    ParamUInt256() : ParamNumber<uint256_t>(uint256_t(0)) {}
    ParamUInt256(uint256_t val) : ParamNumber<uint256_t>(val) {}
    virtual std::string getType() const { return "uint256"; }
    uint256_t getVal() const { return ParamNumber<uint256_t>::getVal(); }
};

class ParamInt256 : public ParamNumber<int256_t>
{
public:
    ParamInt256() : ParamNumber<int256_t>(int256_t(0)) {}
    ParamInt256(int256_t val) : ParamNumber<int256_t>(val) {}
    virtual std::string getType() const { return "int256"; }
};

class ParamBool : public ParamNumber<bool>
{
public:
    ParamBool() : ParamNumber<bool>(false) {}
    ParamBool(bool val) : ParamNumber<bool>(val) {}
    virtual std::string getType() const { return "bool"; }
    bool getVal() const { return ParamNumber<bool>::getVal(); }
};

class ParamByte : public ParamNumber<uint8_t>
{
public:
    ParamByte() : ParamNumber<uint8_t>(0) {}
    ParamByte(uint8_t val) : ParamNumber<uint8_t>(val) {}
    virtual std::string getType() const { return "uint8"; }
};

class ParamInt32 : public ParamNumber<int32_t>
{
public:
    ParamInt32() : ParamNumber<int32_t>(0) {}
    ParamInt32(int32_t val) : ParamNumber<int32_t>(val) {}
    virtual std::string getType() const { return "int32"; }
};

class ParamUInt32 : public ParamNumber<uint32_t>
{
public:
    ParamUInt32() : ParamNumber<uint32_t>(0) {}
    ParamUInt32(uint32_t val) : ParamNumber<uint32_t>(val) {}
    virtual std::string getType() const { return "uint32"; }
};

class ParamInt64 : public ParamNumber<int64_t>
{
public:
    ParamInt64() : ParamNumber<int64_t>(0) {}
    ParamInt64(int64_t val) : ParamNumber<int64_t>(val) {}
    virtual std::string getType() const { return "int64"; }
};

class ParamUInt64 : public ParamNumber<uint64_t>
{
public:
    ParamUInt64() : ParamNumber<uint64_t>(0) {}
    ParamUInt64(uint64_t val) : ParamNumber<uint64_t>(val) {}
    virtual std::string getType() const { return "uint64"; }
};

} // namespace TW::Ethereum::ABI
