// Copyright © 2017-2020 Trust Wallet.
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
class ParamByteArray: public ParamCollection
{
private:
    Data _bytes;
public:
    ParamByteArray() = default;
    ParamByteArray(const Data& val) : ParamCollection() { setVal(val); }
    void setVal(const Data& val) { _bytes = val; }
    const Data& getVal() const { return _bytes; }
    virtual std::string getType() const { return "bytes"; };
    virtual size_t getSize() const { return 32 + ValueEncoder::paddedTo32(_bytes.size()); }
    virtual bool isDynamic() const { return true; }
    virtual size_t getCount() const { return _bytes.size(); }
    static void encodeBytes(const Data& bytes, Data& data);
    virtual void encode(Data& data) const { encodeBytes(_bytes, data); }
    static bool decodeBytes(const Data& encoded, Data& decoded, size_t& offset_inout);
    virtual bool decode(const Data& encoded, size_t& offset_inout) {
        return decodeBytes(encoded, _bytes, offset_inout);
    }
    virtual bool setValueJson(const std::string& value);
    virtual Data hashStruct() const;
};

/// Fixed-size array of bytes, "bytes<N>"
class ParamByteArrayFix: public ParamCollection
{
private:
    size_t _n;
    Data _bytes;
public:
    ParamByteArrayFix(size_t n): ParamCollection(), _n(n), _bytes(Data(_n)) {}
    ParamByteArrayFix(size_t n, const Data& val): ParamCollection(), _n(n), _bytes(Data(_n)) { setVal(val); }
    void setVal(const Data& val);
    const std::vector<uint8_t>& getVal() const { return _bytes; }
    virtual std::string getType() const { return "bytes" + std::to_string(_n); };
    virtual size_t getSize() const { return ValueEncoder::paddedTo32(_bytes.size()); }
    virtual bool isDynamic() const { return false; }
    virtual size_t getCount() const { return _bytes.size(); }
    virtual void encode(Data& data) const;
    static bool decodeBytesFix(const Data& encoded, size_t n, Data& decoded, size_t& offset_inout);
    virtual bool decode(const Data& encoded, size_t& offset_inout) {
        return decodeBytesFix(encoded, _n, _bytes, offset_inout);
    }
    virtual bool setValueJson(const std::string& value);
    virtual Data hashStruct() const;
};

/// Var-length string parameter
class ParamString: public ParamCollection
{
private:
    std::string _str;
public:
    ParamString() = default;
    ParamString(std::string val): ParamCollection() { setVal(val); }
    void setVal(const std::string& val) { _str = val; }
    const std::string& getVal() const { return _str; }
    virtual std::string getType() const { return "string"; };
    virtual size_t getSize() const { return 32 + ValueEncoder::paddedTo32(_str.size()); }
    virtual bool isDynamic() const { return true; }
    virtual size_t getCount() const { return _str.size(); }
    static void encodeString(const std::string& decoded, Data& data);
    virtual void encode(Data& data) const { ParamString::encodeString(_str, data); }
    static bool decodeString(const Data& encoded, std::string& decoded, size_t& offset_inout);
    virtual bool decode(const Data& encoded, size_t& offset_inout) {
        return decodeString(encoded, _str, offset_inout);
    }
    virtual bool setValueJson(const std::string& value) { _str = value; return true; }
    virtual Data hashStruct() const;
};

} // namespace TW::Ethereum::ABI
