// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cell.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

using namespace TW;
using namespace TW::TON;

Slice::Slice() {
    _data = nullptr;
    _size = 0;
    _sizeBits = 0;
}

Slice::~Slice() {
    if (_data != nullptr) {
        delete[] _data;
        _data = nullptr;
    }
}

void Slice::allocate(size_t size, uint8_t *data) {
    assert(_data == nullptr);
    if (data != nullptr && size > 0) {
        _size = size;
        _data = new uint8_t[_size];
        assert(data != nullptr);
        std::copy(data, data + _size, _data);
    }
}

Slice &Slice::operator=(const Slice &from) {
    if (&from != this) {
        // check for self 
        _size = from._size;
        _sizeBits = from._sizeBits;
        _data = nullptr;
        if (from._data != nullptr) {
            allocate(from._size, from._data);
        }
    }
    return *this;
}

Slice Slice::createFromData(uint8_t *data, size_t size) {
    if (data == nullptr || size == 0) {
        throw std::runtime_error("empty data");
    }
    assert(data != nullptr);
    // Create, allocate and copy
    Slice s;
    s.allocate(size, data);
    s._sizeBits = s._size * 8;
    return s;
}

Slice Slice::createFromData(Data data) {
    return createFromData(data.data(), data.size());
}

Slice Slice::createFromHex(std::string const &dataStr) {
    Data data = parse_hex(dataStr);
    return createFromData(data.data(), data.size());
}

Slice Slice::createFromBits(uint8_t *data, size_t size, size_t sizeBits) {
    // compute number of bytes needed
    size_t size1 = sizeBits / 8 + (((sizeBits & 7) == 0) ? 0 : 1);
    if (size < size1) {
        throw std::runtime_error("too few bytes given");
    }
    assert(size >= size1);
    // fill bytes ...
    Slice s = createFromData(data, size1);
    // ... and adjust sizeBits and last byte
    s._sizeBits = sizeBits;
    if ((s._sizeBits & 7) != 0) {
        size_t diffBits = s._size * 8 - s._sizeBits;
        assert(diffBits > 0 && diffBits <= 7);
        // zero unused bits
        s._data[s._size - 1] &= ~((1 << diffBits) - 1);
        // set highest unused bit to 0
        s._data[s._size - 1] |= (1 << (diffBits - 1));
    }
    return s;
}

Slice Slice::createFromBits(Data data, size_t sizeBits) {
    return createFromBits(data.data(), data.size(), sizeBits);
}

Slice Slice::createFromBitsStr(std::string const &dataStr, size_t sizeBits) {
    Data data = parse_hex(dataStr);
    return createFromBits(data.data(), data.size(), sizeBits);
}

std::string Slice::asBytesStr() const {
    if (_data == nullptr)
        return "??";
    assert(_data != nullptr);
    return hex(_data, _data + _size);
}

Data Slice::hash() const {
    if (_data == nullptr)
        return Data(0);
    assert(_data != nullptr);
    return Hash::sha256(_data, _data + _size);
}

void Cell::setSlice(Slice const &slice) {
    _slice = slice;
}

void Cell::setSliceBytes(uint8_t *data, size_t size) {
    Slice s = Slice::createFromData(data, size);
    setSlice(s);
}

void Cell::setSliceBytesStr(std::string const &sliceStr) {
    Slice s = Slice::createFromHex(sliceStr);
    setSlice(s);
}

void Cell::setSliceBitsStr(std::string const &sliceStr, size_t sizeBits) {
    Slice s = Slice::createFromBitsStr(sliceStr, sizeBits);
    setSlice(s);
}

void Cell::addCell(std::shared_ptr<Cell> const &cell) {
    if (cell_count() >= max_cells) {
        throw std::runtime_error("too many cells");
    }
    _cells.push_back(cell);
}

std::string Cell::toString() const {
    std::stringstream s;
    s << "Cell: ";
    if (_slice.size() == 0) {
        s << " no slice";
    } else {
        s << " slice: " << _slice.asBytesStr();
    }
    if (cell_count() == 0) {
        s << ", no children";
    } else {
        s << ", " << cell_count() << " children";
        int cnt = 1;
        for (auto i = _cells.begin(), n = _cells.end(); i != n; ++i, ++cnt) {
            s << std::endl << "  child " << cnt << ": " << i->get()->toString();
        }
    }
    return s.str();
}

Data Cell::hash() const {
    // Need to copy data together into a contiguous area
    std::vector<uint8_t> hashData;
    // number of children
    hashData.push_back(static_cast<uint8_t>(cell_count()));
    // number of hex digits
    size_t bits = _slice.sizeBits(); // may be 0
    hashData.push_back(static_cast<uint8_t>(d2(bits)));
    // data
    if (_slice.size() > 0) {
        hashData.insert(hashData.end(), _slice.data(), _slice.data() + _slice.size());
    }
    // children
    if (cell_count() > 0) {
        for (auto i = _cells.begin(), n = _cells.end(); i != n; ++i) {
            hashData.push_back(0);
            hashData.push_back(0);
        }
        for (auto i = _cells.begin(), n = _cells.end(); i != n; ++i) {
            auto childHash = i->get()->hash();
            hashData.insert(hashData.end(), childHash.begin(), childHash.end());
        }
    }
    // std::cerr << "hashData size " << hashData.size() << std::endl;
    // compute hash
    return Hash::sha256(hashData.data(), hashData.data() + hashData.size());
}

size_t Cell::d2(size_t bits) {
    return (bits / 8) * 2 + (((bits & 7) == 0) ? 0 : 1);
}
