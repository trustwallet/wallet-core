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

namespace TW::TON {

using namespace TW;
using namespace std;

Slice::Slice() {
    _sizeBits = 0;
}

Slice::Slice(const Slice& from) : _data(from._data), _sizeBits(from._sizeBits) {}

Slice Slice::createFromData(const Data& data) {
    if (data.size() == 0) {
        throw std::runtime_error("empty data");
    }
    Slice s;
    s.appendBytes(data);
    return s;
}

Slice Slice::createFromHex(std::string const& dataStr) {
    Data data = parse_hex(dataStr);
    return createFromData(data);
}

Slice Slice::createFromBits(const Data& data, size_t sizeBits) {
    if (sizeBits <= 0) {
        throw std::runtime_error("empty data");
    }
    // compute number of bytes needed
    size_t size1 = sizeBits / 8 + (((sizeBits & 7) == 0) ? 0 : 1);
    if (data.size() < size1) {
        throw std::runtime_error("too few bytes given");
    }
    assert(data.size() >= size1);
    // fill bytes ...
    Slice s = createFromData(data);
    // ... and adjust sizeBits and last byte
    s._sizeBits = sizeBits;
    if ((s._sizeBits & 7) != 0) {
        size_t diffBits = s.size() * 8 - s._sizeBits;
        assert(diffBits > 0 && diffBits <= 7);
        // zero unused bits
        s._data[s.size() - 1] &= ~((1 << diffBits) - 1);
        // set highest unused bit to 1
        s._data[s.size() - 1] |= (1 << (diffBits - 1));
    }
    return s;
}

Slice Slice::createFromBitsStr(std::string const& dataStr, size_t sizeBits) {
    Data data = parse_hex(dataStr);
    return createFromBits(data, sizeBits);
}

void Slice::appendBytes(const Data& data_in) {
    size_t diffBits = size() * 8 - _sizeBits;
    assert(diffBits == 0);
    // at byte-boundary
    append(_data, data_in);
    _sizeBits = _data.size() * 8;
    return;
}

std::string Slice::asBytesStr() const {
    return hex(_data);
}

Data Slice::hash() const {
    return Hash::sha256(_data);
}


Cell::Cell(const Cell& from) : _cells(from._cells), _slice(from._slice) {}

void Cell::setSlice(Slice const& slice) {
    _slice = slice;
}

void Cell::setSliceBytes(const Data& data) {
    Slice s = Slice::createFromData(data);
    setSlice(s);
}

void Cell::setSliceBytesStr(std::string const& sliceStr) {
    Slice s = Slice::createFromHex(sliceStr);
    setSlice(s);
}

void Cell::setSliceBitsStr(std::string const& sliceStr, size_t sizeBits) {
    Slice s = Slice::createFromBitsStr(sliceStr, sizeBits);
    setSlice(s);
}

void Cell::addCell(std::shared_ptr<Cell> const& cell) {
    if (cellCount() >= max_cells) {
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
    if (cellCount() == 0) {
        s << ", no children";
    } else {
        s << ", " << cellCount() << " children";
        int cnt = 1;
        for (auto i = _cells.begin(), n = _cells.end(); i != n; ++i, ++cnt) {
            s << std::endl << "  child " << cnt << ": " << i->get()->toString();
        }
    }
    return s.str();
}

Data Cell::hash() const {
    // Need to copy data together into a contiguous area
    Data hashData;
    // number of children
    hashData.push_back(static_cast<byte>(cellCount()));
    // number of hex digits
    size_t bits = _slice.sizeBits(); // may be 0
    hashData.push_back(static_cast<byte>(d2(bits)));
    // data
    if (_slice.size() > 0) {
        append(hashData, _slice.data());
    }
    // children
    if (cellCount() > 0) {
        for (auto i = _cells.begin(), n = _cells.end(); i != n; ++i) {
            hashData.push_back(0);
            hashData.push_back(0);
        }
        for (auto i = _cells.begin(), n = _cells.end(); i != n; ++i) {
            auto childHash = i->get()->hash();
            append(hashData, childHash);
        }
    }
    // compute hash
    return Hash::sha256(hashData);
}

size_t Cell::d2(size_t bits) {
    return (bits / 8) * 2 + (((bits & 7) == 0) ? 0 : 1);
}

} // namespace TW::TON