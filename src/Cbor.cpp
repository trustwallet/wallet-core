// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cbor.h"
#include "HexCoding.h"
#include "Numeric.h"

#include <sstream>
#include <cassert>

namespace TW::Cbor {

using namespace std;


TW::Data Encode::encoded() const {
    if (openIndefCount > 0) {
        throw invalid_argument("CBOR Unclosed indefinite length building");
    }
    return _data;
}

Encode Encode::uint(uint64_t value) {
    return Encode().appendValue(Decode::MT_uint, value);
}

Encode Encode::negInt(uint64_t value) {
    if (value == 0) {
        // special handling for -1, to avoid underflow
        return Encode().appendValue(Decode::MT_uint, 0);
    }
    return Encode().appendValue(Decode::MT_negint, value - 1);
}

Encode Encode::string(const std::string& str) {
    return Encode().appendValue(Decode::MT_string, str.size()).append(TW::data(str));
}

Encode Encode::bytes(const Data& str) {
    return Encode().appendValue(Decode::MT_bytes, str.size()).append(str);
}

Encode Encode::array(const vector<Encode>& elems) {
    Encode e;
    auto n = elems.size();
    e.appendValue(Decode::MT_array, n);
    for (auto i = 0ul; i < n; ++i) {
        e.append(elems[i].encoded());
    }
    return e;
}

Encode Encode::map(const std::map<Encode, Encode>& elems) {
    Encode enc;
    auto n = elems.size();
    enc.appendValue(Decode::MT_map, n);
    for (const auto& e: elems) {
        enc.append(e.first.encoded());
        enc.append(e.second.encoded());
    }
    return enc;
}

Encode Encode::tag(uint64_t value, const Encode& elem) {
    Encode e;
    e.appendValue(Decode::MT_tag, value);
    e.append(elem.encoded());
    return e;
}

Encode Encode::null() {
    Encode e;
    e.appendValue(Decode::MT_special, 0x16);
    return e;
}

Encode Encode::indefArray() {
    Encode e;
    e.appendIndefinite(Decode::MT_array);
    ++e.openIndefCount;
    return e;
}

Encode Encode::addIndefArrayElem(const Encode& elem) {
    if (openIndefCount == 0) {
        throw invalid_argument("CBOR Not inside indefinite-length array");
    }
    append(elem.encoded());
    return *this;
}

Encode Encode::closeIndefArray() {
    if (openIndefCount == 0) {
        throw invalid_argument("CBOR Not inside indefinite-length array");
    }
    // add closing break command
    TW::append(_data, 0xFF);
    // close counter
    --openIndefCount;
    return *this;
}

Encode Encode::fromRaw(const TW::Data& rawData) {
    // check validity, may throw
    Decode check(rawData);
    if (!check.isValid()) { throw invalid_argument("Invalid CBOR data"); }
    return Encode(rawData);
}

Encode Encode::appendValue(byte majorType, uint64_t value) {
    byte byteCount = 0;
    byte minorType = 0;
    if (value < 24) {
        byteCount = 1;
        minorType = (byte)value;
    } else if (value <= 0xFF) {
        byteCount = 1 + 1;
        minorType = 24;
    } else if (value <= 0xFFFF) {
        byteCount = 1 + 2;
        minorType = 25;
    } else if (value <= 0xFFFFFFFF) {
        byteCount = 1 + 4;
        minorType = 26;
    } else {
        byteCount = 1 + 8;
        minorType = 27;
    }
    // add bytes
    TW::append(_data, (byte)((majorType << 5) | (minorType & 0x1F)));
    Data valBytes = Data(byteCount - 1);
    for (auto i = 0ul; i < valBytes.size(); ++i) {
        valBytes[valBytes.size() - 1 - i] = (byte)(value & 0xFF);
        value = value >> 8;
    }
    append(valBytes);
    return *this;
}

void Encode::appendIndefinite(byte majorType) {
    byte minorType = 31;
    TW::append(_data, (byte)((majorType << 5) | (minorType & 0x1F)));
}


Decode::Decode(const Data& input)
: data(std::make_shared<OrigDataRef>(input)) {
    // shared_ptr to original input data created
    subStart = 0;
    subLen = (uint32_t)input.size();
}

Decode::Decode(const std::shared_ptr<OrigDataRef>& nData, uint32_t nSubStart, uint32_t nSubLen)
: data(nData) {
    // shared_ptr to original input data added
    subStart = nSubStart;
    subLen = nSubLen;
    assert(subStart + subLen <= data->origData.size());
}

Decode Decode::skipClone(uint32_t offset) const {
    assert(subStart + offset <= data->origData.size());
    return Decode(data, subStart + offset, subLen - offset);
}

Decode::TypeDesc Decode::getTypeDesc() const {
    TypeDesc typeDesc;
    typeDesc.isIndefiniteValue = false;
    typeDesc.majorType = (MajorType)(getByte(0) >> 5);
    assert((int)typeDesc.majorType >= 0 && (int)typeDesc.majorType <= 7);
    auto minorType = (TW::byte)((uint8_t)getByte(0) & 0x1F);
    if (minorType < 24) {
        // direct value
        typeDesc.byteCount = 1;
        typeDesc.value = minorType;
        return typeDesc;
    }
    if (minorType == 24) {
        typeDesc.byteCount = 1 + 1;
        typeDesc.value = getByte(1);
        return typeDesc;
    }
    if (minorType == 25) {
        typeDesc.byteCount = 1 + 2;
        typeDesc.value = (uint16_t)(((uint16_t)getByte(1) << 8) + (uint16_t)getByte(2));
        return typeDesc;
    }
    if (minorType == 26) {
        typeDesc.byteCount = 1 + 4;
        typeDesc.value = (uint32_t)(((uint32_t)getByte(1) << 24) + ((uint32_t)getByte(2) << 16) + ((uint32_t)getByte(3) << 8) + (uint32_t)getByte(4));
        return typeDesc;
    }
    if (minorType == 27) {
        typeDesc.byteCount = 1 + 8;
        typeDesc.value =
            (uint64_t)(
                ((uint64_t)getByte(1) << 56) +
                ((uint64_t)getByte(2) << 48) +
                ((uint64_t)getByte(3) << 40) +
                ((uint64_t)getByte(4) << 32) +
                ((uint64_t)getByte(5) << 24) +
                ((uint64_t)getByte(6) << 16) +
                ((uint64_t)getByte(7) << 8) +
                ((uint64_t)getByte(8)));
        return typeDesc;
    }
    if (minorType >= 28 && minorType <= 30) {
        throw std::invalid_argument("CBOR unassigned type not supported");
    }
    // minorType == 31
    // stop code
    typeDesc.byteCount = 1;
    typeDesc.value = 0;
    typeDesc.isIndefiniteValue = true;
    return typeDesc;
}

uint32_t Decode::getTotalLen() const {
    TypeDesc typeDesc = getTypeDesc();
    switch (typeDesc.majorType) {
        case MT_uint:
        case MT_negint:
        case MT_special:
            // simple types
            return typeDesc.byteCount;
        case MT_bytes:
        case MT_string:
            return (uint32_t)typeDesc.byteCount + (uint32_t)typeDesc.value;
        case MT_array:
            return getCompoundLength(1);
        case MT_map:
            return getCompoundLength(2);
        default:
        case MT_tag:
            {
                uint32_t dataLen = skipClone(typeDesc.byteCount).getTotalLen();
                return typeDesc.byteCount + dataLen;
            }
    }
}

uint64_t Decode::getValue() const {
    TypeDesc typeDesc = getTypeDesc();
    if (typeDesc.majorType != MT_uint && typeDesc.majorType != MT_negint) {
        throw std::invalid_argument("CBOR data type not a value-type");
    }
    return typeDesc.value;
}

std::string Decode::getString() const {
    Data strData = getBytes();
    return std::string((const char*)strData.data(), strData.size()); 
}

Data Decode::getBytes() const {
    TypeDesc typeDesc = getTypeDesc();
    if (typeDesc.majorType != MT_bytes && typeDesc.majorType != MT_string) {
        throw std::invalid_argument("CBOR data type not bytes/string");
    }
    auto len = (uint32_t)typeDesc.value;
    if (length() < (uint32_t)typeDesc.byteCount + (uint32_t)len) {
        throw std::invalid_argument("CBOR bytes/string data too short");
    }
    assert(subStart + typeDesc.byteCount + len <= data->origData.size());
    return TW::data(data->origData.data() + (subStart + typeDesc.byteCount), len); 
}

bool Decode::isBreak() const {
    TypeDesc typeDesc = getTypeDesc();
    if (typeDesc.majorType == MT_special && typeDesc.isIndefiniteValue) { return true; }
    return false;
}

uint32_t Decode::getCompoundLength(uint32_t countMultiplier) const {
    uint32_t len = 0;
    TypeDesc typeDesc = getTypeDesc();
    uint32_t count = typeDesc.isIndefiniteValue ? 0 : (uint32_t)(typeDesc.value * countMultiplier);
    // process elements
    len += typeDesc.byteCount;
    for (auto i = 0ul; i < count || typeDesc.isIndefiniteValue; ++i) {
        Decode nextElem = skipClone(len);
        if (typeDesc.isIndefiniteValue && nextElem.isBreak()) {
            // end of indefinite-length
            len += 1; // account for break
            break;
        }
        uint32_t elemLen = nextElem.getTotalLen();
        if (len + elemLen > length()) {
            throw std::invalid_argument("CBOR array data too short");
        }
        len += elemLen;
    }
    return len;
}

vector<Decode> Decode::getCompoundElements(uint32_t countMultiplier, TW::byte expectedType) const {
    TypeDesc typeDesc = getTypeDesc();
    if (typeDesc.majorType != expectedType) {
        throw std::invalid_argument("CBOR data type mismatch");
    }
    vector<Decode> elems;
    uint32_t count = typeDesc.isIndefiniteValue ? 0 : (uint32_t)(typeDesc.value * countMultiplier);
    // process elements
    uint32_t idx = typeDesc.byteCount;
    for (auto i = 0ul; i < count || typeDesc.isIndefiniteValue; ++i) {
        Decode nextElem = skipClone(idx);
        if (typeDesc.isIndefiniteValue && nextElem.isBreak()) {
            // end of indefinite-length
            break;
        }
        uint32_t elemLen = nextElem.getTotalLen();
        if (elemLen == 0 || checkAddUnsignedOverflow(idx, elemLen)) {
            throw std::invalid_argument("CBOR invalid element length");
        }
        if (idx + elemLen > length()) {
            throw std::invalid_argument("CBOR invalid array data");
        }
        elems.emplace_back(Decode(data, subStart + idx, elemLen));
        idx += elemLen;
    }
    return elems;
}

vector<pair<Decode, Decode>> Decode::getMapElements() const {
    auto elems = getCompoundElements(2, MT_map);
    vector<pair<Decode, Decode>> map;
    for (auto i = 0ul; i < elems.size(); i += 2) {
        map.emplace_back(make_pair(elems[i], elems[i + 1]));
    }
    return map;
}

uint64_t Decode::getTagValue() const {
    TypeDesc typeDesc = getTypeDesc();
    if (typeDesc.majorType != MT_tag) {
        throw std::invalid_argument("CBOR data type not tag");
    }
    return typeDesc.value;
}

Decode Decode::getTagElement() const {
    TypeDesc typeDesc = getTypeDesc();
    if (typeDesc.majorType != MT_tag) {
        throw std::invalid_argument("CBOR data type not tag");
    }
    return skipClone(typeDesc.byteCount);
}

bool Decode::isValid() const {
    try {
        TypeDesc typeDesc = getTypeDesc();
        switch (typeDesc.majorType) {
            case MT_uint:
            case MT_negint:
            case MT_special:
                return (typeDesc.byteCount <= subLen);

            case MT_bytes:
            case MT_string:
                {
                    auto len = (uint32_t)(typeDesc.byteCount + typeDesc.value);
                    return (len <= subLen);
                }

            case MT_array:
            case MT_map:
                {
                    uint32_t countMultiplier = (typeDesc.majorType == MT_map) ? 2 : 1;
                    uint32_t len = getCompoundLength(countMultiplier);
                    if (len > subLen) { return false; }
                    auto count = typeDesc.isIndefiniteValue ? 0 : countMultiplier * typeDesc.value;
                    uint32_t idx = typeDesc.byteCount;
                    for (auto i = 0ul; i < count || typeDesc.isIndefiniteValue; ++i)
                    {
                        Decode nextElem = skipClone(idx);
                        if (typeDesc.isIndefiniteValue && nextElem.isBreak()) { break; }
                        if (!nextElem.isValid()) { return false; }
                        idx += nextElem.getTotalLen();
                    }
                    return true;
                }

            default:
            case MT_tag:
                return skipClone(typeDesc.byteCount).isValid();
        }
    } catch (exception& ex) {
        return false;
    }
}

string Decode::dumpToStringInternal() const {
    stringstream s;
    TypeDesc typeDesc = getTypeDesc();
    switch (typeDesc.majorType) {
        case MT_uint:
            s << typeDesc.value;
            break;

        case MT_negint:
            s << (int64_t)-1 - (int64_t)typeDesc.value;
            break;

        case MT_bytes:
            s << "h\"" << TW::hex(getString()) << "\"";
            break;

        case MT_string:
            s << "\"" << getString() << "\"";
            break;

        case MT_array:
            {
                if (typeDesc.isIndefiniteValue) {
                    s << "[_ ";
                } else {
                    s << "[";
                }
                vector<Decode> elems = getArrayElements();
                for (auto i = 0ul; i < elems.size(); ++i) {
                    if (i > 0) s << ", ";
                    s << elems[i].dumpToStringInternal();
                }
                s << "]";
            }
            break;

        case MT_map:
            {
                if (typeDesc.isIndefiniteValue) {
                    s << "{_ ";
                } else {
                    s << "{";
                }
                auto elems = getMapElements();
                for (auto i = 0ul; i < elems.size(); ++i) {
                    if (i > 0) s << ", ";
                    s << elems[i].first.dumpToStringInternal() << ": " << elems[i].second.dumpToStringInternal();
                }
                s << "}";
            }
            break;

        case MT_tag:
            s << "tag " << typeDesc.value << " " << getTagElement().dumpToStringInternal();
            break;

        default:
        case MT_special: // float or simple
            if (typeDesc.isIndefiniteValue) {
                // skip break command
            } else {
                if (typeDesc.value == 0x16) {
                    s << "null";
                } else {
                    s << "spec " << typeDesc.value;
                }
            }
            break;
    }
    return s.str();
}

string Decode::dumpToString() const {
    stringstream s;
    s << dumpToStringInternal();
    return s.str();
}

Data Decode::encoded() const
{
    assert(subStart + subLen <= data->origData.size());
    return TW::data(data->origData.data() + subStart, subLen);
}

} // namespace TW::Cbor
