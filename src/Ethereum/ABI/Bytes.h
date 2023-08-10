// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "ValueEncoder.h"
#include <Data.h>

namespace TW::Ethereum::ABI {

/// Dynamic array of bytes "bytes"
class ParamByteArray final: public ParamCollection
{
private:
    Data _bytes;
public:
    ParamByteArray() = default;
    ParamByteArray(const Data& val) : ParamCollection() { setVal(val); }
    void setVal(const Data& val) { _bytes = val; }
    const Data& getVal() const { return _bytes; }
    std::string getType() const override { return "bytes"; };
    size_t getSize() const override { return 32 + ValueEncoder::paddedTo32(_bytes.size()); }
    bool isDynamic() const override { return true; }
    size_t getCount() const override { return _bytes.size(); }
    static void encodeBytes(const Data& bytes, Data& data);
    void encode(Data& data) const override { encodeBytes(_bytes, data); }
    static bool decodeBytes(const Data& encoded, Data& decoded, size_t& offset_inout);
    bool decode(const Data& encoded, size_t& offset_inout) override {
        return decodeBytes(encoded, _bytes, offset_inout);
    }
    bool setValueJson(const std::string& value) override;
    Data hashStruct() const override;
    std::shared_ptr<ParamBase> clone() const override;
};

/// Fixed-size array of bytes, "bytes<N>"
class ParamByteArrayFix final: public ParamCollection
{
private:
    size_t _n;
    Data _bytes;
public:
    ParamByteArrayFix(size_t n): ParamCollection(), _n(n), _bytes(Data(_n)) {}
    ParamByteArrayFix(size_t n, const Data& val): ParamCollection(), _n(n), _bytes(Data(_n)) { setVal(val); }
    void setVal(const Data& val);
    const std::vector<uint8_t>& getVal() const { return _bytes; }
    std::string getType() const override { return "bytes" + std::to_string(_n); };
    size_t getSize() const override { return ValueEncoder::paddedTo32(_bytes.size()); }
    bool isDynamic() const override { return false; }
    size_t getCount() const override { return _bytes.size(); }
    void encode(Data& data) const override;
    static bool decodeBytesFix(const Data& encoded, size_t n, Data& decoded, size_t& offset_inout);
    bool decode(const Data& encoded, size_t& offset_inout) override {
        return decodeBytesFix(encoded, _n, _bytes, offset_inout);
    }
    bool setValueJson(const std::string& value) override;
    Data hashStruct() const override;
    std::shared_ptr<ParamBase> clone() const override;
};

/// Var-length string parameter
class ParamString final: public ParamCollection
{
private:
    std::string _str;
public:
    ParamString() = default;
    ParamString(const std::string& val): ParamCollection() { setVal(val); }
    void setVal(const std::string& val) { _str = val; }
    const std::string& getVal() const { return _str; }
    std::string getType() const override { return "string"; };
    size_t getSize() const override { return 32 + ValueEncoder::paddedTo32(_str.size()); }
    bool isDynamic() const override { return true; }
    size_t getCount() const override { return _str.size(); }
    static void encodeString(const std::string& decoded, Data& data);
    void encode(Data& data) const override { ParamString::encodeString(_str, data); }
    static bool decodeString(const Data& encoded, std::string& decoded, size_t& offset_inout);
    bool decode(const Data& encoded, size_t& offset_inout) override {
        return decodeString(encoded, _str, offset_inout);
    }
    bool setValueJson(const std::string& value) override { _str = value; return true; }
    Data hashStruct() const override;
    std::shared_ptr<ParamBase> clone() const override;
};

} // namespace TW::Ethereum::ABI
