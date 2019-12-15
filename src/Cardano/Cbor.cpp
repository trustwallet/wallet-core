// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cbor.h"

#include <iostream>
#include <sstream>

namespace TW::Cardano::Cbor {

using namespace std;

Encode Encode::addPInt(uint64_t value) {
    appendValue(0, value);
    return *this;
}

Encode Encode::addString(const Data& string) {
    appendValue(2, string.size());
    append(data, string);
    return *this;
}

Encode Encode::addArray(const vector<Encode>& elems) {
    auto n = elems.size();
    appendValue(4, n);
    for (int i = 0; i < n; ++i) {
        append(data, elems[i].getData());
    }
    return *this;
}

Encode Encode::addMap(const vector<std::pair<Encode, Encode>>& elems) {
    auto n = elems.size();
    appendValue(5, n);
    for (int i = 0; i < n; ++i) {
        append(data, elems[i].first.getData());
        append(data, elems[i].second.getData());
    }
    return *this;
}

Encode Encode::addTag(uint64_t value, const Encode& elem) {
    appendValue(6, value);
    append(data, elem.getData());
    return *this;
}

void Encode::appendValue(byte majorType, uint64_t value) {
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
    //cerr << "appendValue " << (int)majorType << " " << (int)minorType << endl;
    TW::append(data, (majorType << 5) | (minorType & 0x1F));
    Data valBytes = Data(byteCount - 1);
    for (int i = 0; i < valBytes.size(); ++i) {
        valBytes[valBytes.size() - 1 - i] = (byte)(value & 0xFF);
        value = value >> 8;
    }
    TW::append(data, valBytes);
}


Decode::Decode(const TW::byte* ndata, uint32_t nlen) {
    base = ndata;
    startIdx = 0;
    totlen = nlen;
}

Decode::Decode(const Data& input) {
    base = (const TW::byte*)input.data();
    startIdx = 0;
    totlen = input.size();
}

Decode Decode::skip(uint32_t offset) const {
    Decode skipped = Decode(base, totlen);
    skipped.startIdx = startIdx + offset;
    return skipped;
}

void Decode::getTypes(TW::byte& majorType, TW::byte& byteCount, uint64_t& value) const {
    majorType = byte(0) >> 5;
    TW::byte minorType = (TW::byte)((uint8_t)byte(0) & 0x1F);
    if (minorType < 24) {
        // direct value
        byteCount = 1;
        value = minorType;
        return;
    }
    if (minorType == 24) {
        byteCount = 1 + 1;
        value = byte(1);
        return;
    }
    if (minorType == 25) {
        byteCount = 1 + 2;
        value = 0x100 * (uint16_t)byte(1) + (uint16_t)byte(2);
        return;
    }
    if (minorType == 26) {
        byteCount = 1 + 4;
        value = 0x1000000 * (uint32_t)byte(1) + 0x10000 * (uint32_t)byte(2) + 0x100 * (uint32_t)byte(3) + (uint32_t)byte(4);
        return;
    }
    throw std::invalid_argument("CBOR longer values not supported");
}

uint32_t Decode::getSimpleTotalLen() const {
    TW::byte majorType;
    TW::byte byteCount;
    uint64_t value;
    getTypes(majorType, byteCount, value);
    //cerr << "major, bytecnt, val  " << (int)majorType << " " << (int)byteCount << " " << (uint32_t)value << endl;
    switch (majorType) {
        case 0:
        case 1:
            // simple types
            return byteCount;
        case 2:
        case 3:
            // string
            return (uint32_t)byteCount + (uint32_t)value;
        case 5:
            {
                auto elems = getMapElements();
                uint32_t len = byteCount;
                for (int i = 0; i < elems.size(); ++i) {
                    len += elems[i].first.getSimpleTotalLen() + elems[i].second.getSimpleTotalLen();
                }
                return len;
            }
        case 6: // tag
            {
                uint32_t dataLen = skip(byteCount).getSimpleTotalLen();
                return byteCount + dataLen;
            }
        default:
            //cerr << "majorType " << (int)majorType << endl;
            throw std::invalid_argument("CBOR length type not supported");
    }
}

uint64_t Decode::getValue() const {
    TW::byte majorType;
    TW::byte byteCount;
    uint64_t value;
    getTypes(majorType, byteCount, value);
    if (majorType != 0 && majorType != 1) {
        throw std::invalid_argument("CBOR data type not a value-type");
    }
    return value;
}

Data Decode::getStringData() const {
    TW::byte majorType;
    TW::byte byteCount;
    uint64_t value;
    getTypes(majorType, byteCount, value);
    if (majorType != 2 && majorType != 3) {
        throw std::invalid_argument("CBOR data type not string");
    }
    uint32_t len = value;
    if (length() < (uint32_t)byteCount + (uint32_t)len) {
        throw std::invalid_argument("CBOR string data too short");
    }
    return data(base + (startIdx + byteCount), value); 
}

vector<Decode> Decode::getCompoundElements(uint32_t countMultiplier, TW::byte expectedType) const {
    TW::byte majorType;
    TW::byte byteCount;
    uint64_t value;
    getTypes(majorType, byteCount, value);
    if (majorType != expectedType) {
        throw std::invalid_argument("CBOR data type not array");
    }
    vector<Decode> elems;
    uint32_t count = (uint32_t)(value * countMultiplier);
    // process elements
    uint32_t idx = byteCount;
    for (int i = 0; i < count; ++i) {
        uint32_t elemLen = skip(idx).getSimpleTotalLen();
        if (idx + elemLen > length()) {
            throw std::invalid_argument("CBOR array data too short");
        }
        elems.push_back(Decode(base + (startIdx + idx), elemLen));
        idx += elemLen;
    }
    return elems;
}

vector<pair<Decode, Decode>> Decode::getMapElements() const {
    auto elems = getCompoundElements(2, 5);
    vector<pair<Decode, Decode>> map;
    for (int i = 0; i < elems.size(); i += 2) {
        map.push_back(make_pair(elems[i], elems[i+1]));
    }
    return map;
}

uint64_t Decode::getTagValue() const {
    TW::byte majorType;
    TW::byte byteCount;
    uint64_t value;
    getTypes(majorType, byteCount, value);
    if (majorType != 6) {
        throw std::invalid_argument("CBOR data type not tag");
    }
    return value;
}

Decode Decode::getTagElement() const {
    TW::byte majorType;
    TW::byte byteCount;
    uint64_t value;
    getTypes(majorType, byteCount, value);
    if (majorType != 6) {
        throw std::invalid_argument("CBOR data type not tag");
    }
    return skip(byteCount);
}

string Decode::dumpToString() const {
    stringstream s;
    s << "["<< length() << "] ";
    TW::byte majorType;
    TW::byte byteCount;
    uint64_t value;
    getTypes(majorType, byteCount, value);
    switch (majorType) {
        case 0: // pint
            s << "pint " << (int)byteCount << " (" << value << ")";
            break;

        case 1: // nint
            s << "nint " << (int)byteCount << " (" << value << ")";
            break;

        case 2: // string
            s << "string " << (int)byteCount << "+" << value;
            break;

        case 3: // string
            s << "utf string " << (int)byteCount << "+" << value;
            break;

        case 4: // array
            {
                s << "array " << (int)byteCount << " " << value << " (";
                vector<Decode> elems = getArrayElements();
                for (int i = 0; i < elems.size(); ++i) {
                    if (i > 0) s << ", ";
                    s << elems[i].dumpToString();
                }
                s << ")";
            }
            break;

        case 5: // map
            {
                s << "map " << (int)byteCount << " " << value << " (";
                auto elems = getMapElements();
                for (int i = 0; i < elems.size(); ++i) {
                    if (i > 0) s << ", ";
                    s << elems[i].first.dumpToString() << ": " << elems[i].second.dumpToString();
                }
                s << ")";
            }
            break;

        case 6: // tag
            s << "tag " << (int)byteCount << " " << value << " (" << getTagElement().dumpToString() << ")";
            break;

        default:
            //cerr << "majorType " << (int)majorType << endl;
            throw std::invalid_argument("CBOR analyze type not supported");
    }
    return s.str();
}

} // namespace TW::Cardano
