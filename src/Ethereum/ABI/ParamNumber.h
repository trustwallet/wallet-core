// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "ValueEncoder.h"

#include <Data.h>
#include <uint256.h>

#include <boost/lexical_cast.hpp>

#include <string>

namespace TW::Ethereum::ABI {


inline bool decode(const Data& encoded, uint256_t& decoded, size_t& offset_inout)
{
    decoded = 0u;
    if (encoded.empty() || (encoded.size() < (ValueEncoder::encodedIntSize + offset_inout))) {
        return false;
    }
    decoded = loadWithOffset(encoded, offset_inout);
    offset_inout += ValueEncoder::encodedIntSize;
    return true;
}

/// Generic parameter class for numeric types, like bool, uint32, int64, etc.  All are stored on 256 bits.
template<typename T>
class ParamNumberType : public ParamBase
{
public:
    ParamNumberType() = default;
    explicit ParamNumberType(const T& val): _val(val) { }
    void setVal(T val) { _val = val; }
    T getVal() const { return _val; }
    virtual std::string getType() const = 0;
    virtual size_t getSize() const { return ValueEncoder::encodedIntSize; }
    virtual bool isDynamic() const { return false; }
    virtual void encode(Data& data) const {
        // cast up
        ValueEncoder::encodeUInt256(static_cast<uint256_t>(_val), data);
    }
    static bool decodeNumber(const Data& encoded, T& decoded, size_t& offset_inout) {
        uint256_t val256;
        if (!ABI::decode(encoded, val256, offset_inout)) { return false; }
        // cast down
        decoded = static_cast<T>(val256);
        return true;
    }
    virtual bool decode(const Data& encoded, size_t& offset_inout) {
        return decodeNumber(encoded, _val, offset_inout);
    }
    virtual bool setValueJson(const std::string& value) {
        return boost::conversion::detail::try_lexical_convert(value, _val);
    }
protected:
    T _val;
};

class ParamUInt256 final : public ParamNumberType<uint256_t>
{
public:
    ParamUInt256() : ParamNumberType<uint256_t>(uint256_t(0)) {}
    explicit ParamUInt256(const uint256_t& val) : ParamNumberType<uint256_t>(val) {}
    std::string getType() const override { return "uint256"; }
    uint256_t getVal() const { return ParamNumberType<uint256_t>::getVal(); }
    bool setValueJson(const std::string& value) override { return setUInt256FromValueJson(_val, value); }
    static bool setUInt256FromValueJson(uint256_t& dest, const std::string& value);
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamUInt256>(_val); }
};

class ParamInt256 final : public ParamNumberType<int256_t>
{
public:
    ParamInt256() : ParamNumberType<int256_t>(int256_t(0)) {}
    explicit ParamInt256(const int256_t& val) : ParamNumberType<int256_t>(val) {}
    std::string getType() const override { return "int256"; }
    int256_t getVal() const { return ParamNumberType<int256_t>::getVal(); }
    bool setValueJson(const std::string& value) override { return setInt256FromValueJson(_val, value); }
    static bool setInt256FromValueJson(int256_t& dest, const std::string& value);
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamInt256>(_val); }
};

class ParamBool final : public ParamNumberType<bool>
{
public:
    ParamBool() : ParamNumberType<bool>(false) {}
    explicit ParamBool(bool val) : ParamNumberType<bool>(val) {}
    std::string getType() const override { return "bool"; }
    bool getVal() const { return ParamNumberType<bool>::getVal(); }
    bool setValueJson(const std::string& value) override;
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamBool>(_val); }
};

class ParamUInt8 final : public ParamNumberType<uint8_t>
{
public:
    ParamUInt8() : ParamNumberType<uint8_t>(0) {}
    explicit ParamUInt8(uint8_t val) : ParamNumberType<uint8_t>(val) {}
    std::string getType() const override { return "uint8"; }
    bool setValueJson(const std::string& value) override;
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamUInt8>(_val); }
};

class ParamInt8 final : public ParamNumberType<int8_t>
{
public:
    ParamInt8() : ParamNumberType<int8_t>(0) {}
    explicit ParamInt8(int8_t val) : ParamNumberType<int8_t>(val) {}
    std::string getType() const override { return "int8"; }
    bool setValueJson(const std::string& value) override;
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamInt8>(_val); }
};

class ParamUInt16 final : public ParamNumberType<uint16_t>
{
public:
    ParamUInt16() : ParamNumberType<uint16_t>(0) {}
    explicit ParamUInt16(uint16_t val) : ParamNumberType<uint16_t>(val) {}
    std::string getType() const override { return "uint16"; }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamUInt16>(_val); }
};

class ParamInt16 final : public ParamNumberType<int16_t>
{
public:
    ParamInt16() : ParamNumberType<int16_t>(0) {}
    explicit ParamInt16(int16_t val) : ParamNumberType<int16_t>(val) {}
    std::string getType() const override { return "int16"; }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamInt16>(_val); }
};

class ParamUInt32 final : public ParamNumberType<uint32_t>
{
public:
    ParamUInt32() : ParamNumberType<uint32_t>(0) {}
    explicit ParamUInt32(uint32_t val) : ParamNumberType<uint32_t>(val) {}
    std::string getType() const override { return "uint32"; }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamUInt32>(_val); }
};

class ParamInt32 final : public ParamNumberType<int32_t>
{
public:
    ParamInt32() : ParamNumberType<int32_t>(0) {}
    explicit ParamInt32(int32_t val) : ParamNumberType<int32_t>(val) {}
    std::string getType() const override { return "int32"; }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamInt32>(_val); }
};

class ParamUInt64 final : public ParamNumberType<uint64_t>
{
public:
    ParamUInt64() : ParamNumberType<uint64_t>(0) {}
    explicit ParamUInt64(uint64_t val) : ParamNumberType<uint64_t>(val) {}
    std::string getType() const override { return "uint64"; }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamUInt64>(_val); }
};

class ParamInt64 final : public ParamNumberType<int64_t>
{
public:
    ParamInt64() : ParamNumberType<int64_t>(0) {}
    explicit ParamInt64(int64_t val) : ParamNumberType<int64_t>(val) {}
    std::string getType() const override { return "int64"; }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamInt64>(_val); }
};

/// Generic parameter class for all other bit sizes, like UInt24, 40, 48, ... 248.
/// For predefined sizes (8, 16, 32, 64, 256) use the sepcial types like UInt32.
/// Stored on 256 bits.
class ParamUIntN final : public ParamBase
{
public:
    const size_t bits;
    ParamUIntN(size_t bits_in) : bits(bits_in) { init(); }
    ParamUIntN(size_t bits_in, uint256_t val) : bits(bits_in) { init(); setVal(val); }
    void setVal(uint256_t val);
    uint256_t getVal() const { return _val; }
    std::string getType() const override { return "uint" + std::to_string(bits); }
    size_t getSize() const override { return ValueEncoder::encodedIntSize; }
    bool isDynamic() const override { return false; }
    void encode(Data& data) const override { ValueEncoder::encodeUInt256(_val, data); }
    static bool decodeNumber(const Data& encoded, uint256_t& decoded, size_t& offset_inout) {
        return ABI::decode(encoded, decoded, offset_inout);
    }
    bool decode(const Data& encoded, size_t& offset_inout) override;
    static uint256_t maskForBits(size_t bits);
    bool setValueJson(const std::string& value) override { return ParamUInt256::setUInt256FromValueJson(_val, value); }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamUIntN>(bits, _val); }

private:
    void init();
    uint256_t _val;
    uint256_t _mask;
};

/// Generic parameter class for all other bit sizes, like Int24, 40, 48, ... 248.
/// For predefined sizes (8, 16, 32, 64, 256) use the sepcial types like Int32.
/// Stored on 256 bits.
class ParamIntN final : public ParamBase
{
public:
    const size_t bits;
    ParamIntN(size_t bits_in) : bits(bits_in) { init(); }
    ParamIntN(size_t bits_in, int256_t val) : bits(bits_in) { init(); setVal(val); }
    void setVal(int256_t val);
    int256_t getVal() const { return _val; }
    std::string getType() const override { return "int" + std::to_string(bits); }
    size_t getSize() const override { return ValueEncoder::encodedIntSize; }
    bool isDynamic() const override { return false; }
    void encode(Data& data) const override { ValueEncoder::encodeUInt256((uint256_t)_val, data); }
    static bool decodeNumber(const Data& encoded, int256_t& decoded, size_t& offset_inout);
    bool decode(const Data& encoded, size_t& offset_inout) override;
    bool setValueJson(const std::string& value) override { return ParamInt256::setInt256FromValueJson(_val, value); }
    std::shared_ptr<ParamBase> clone() const override { return std::make_shared<ParamIntN>(bits, _val); }

private:
    void init();
    int256_t _val;
    uint256_t _mask;
};

} // namespace TW::Ethereum::ABI
