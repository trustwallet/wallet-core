// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cell.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <boost/crc.hpp>  // for boost::crc_32_type

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
    Slice s;
    s.appendBits(data, sizeBits);
    return s;
}

Slice Slice::createFromBitsStr(std::string const& dataStr, size_t sizeBits) {
    Data data = parse_hex(dataStr);
    return createFromBits(data, sizeBits);
}

void Slice::appendBytes(const Data& data_in) {
    int diffBits = (int)(size() * 8 - _sizeBits);
    assert(diffBits >= 0 && diffBits <= 7);
    if (diffBits == 0) {
        // at byte-boundary
        append(_data, data_in);
        _sizeBits = _data.size() * 8;
        return;
    }
    // not at byte boundary, bit operations needed
    return appendBits(data_in, data_in.size() * 8);
}

void Slice::appendBits(const Data& data_in, size_t sizeBits) {
    if (sizeBits == 0) { return; }
    // compute number of bytes needed
    size_t size1 = sizeBits / 8 + (((sizeBits & 7) == 0) ? 0 : 1);
    if (data_in.size() != size1) {
        // wrong number of bytes/bits
        throw std::runtime_error("mismatch between bytes and bits size");
    }
    assert(data_in.size() == size1);
    if ((_sizeBits & 7) == 0) {
        appendBitsAligned(data_in, sizeBits);
        return;
    }
    // all new bits have to be shifted
    appendBitsNotAligned(data_in, sizeBits);
}

void Slice::appendBitsAligned(const Data& data_in, size_t sizeBits) {
    // old is aligned
    assert((_sizeBits & 7) == 0);
    size_t size1 = sizeBits / 8 + (((sizeBits & 7) == 0) ? 0 : 1);
    assert(data_in.size() == size1);
    int diffBitsNew = (int)(size1 * 8 - sizeBits);
    assert(diffBitsNew >= 0 && diffBitsNew <= 7);
    if (diffBitsNew == 0) {
        // both old and new are aligned, no bit operations needed
        return appendBytes(data_in);
    }
    // old is aligned, but new is not
    assert(diffBitsNew >= 1 && diffBitsNew <= 7);
    // bytes, except last
    if (size1 > 1) {
        append(_data, TW::data(data_in.data(), size1 - 1));
    }
    // last byte
    byte last = data_in[size1 - 1];
    // zero unused bits
    last &= ~((1 << (byte)diffBitsNew) - 1);
    _data.push_back(last);
    _sizeBits += sizeBits;
    // set highest unused bit to 1
    int diffBits = (int)(size() * 8 - _sizeBits);
    assert(diffBits >= 1 && diffBits <= 7);
    _data[_data.size() - 1] |= (1 << (byte)(diffBits - 1));
}

void Slice::appendBitsNotAligned(const Data& data_in, size_t sizeBits) {
    // old is not aligned
    assert((_sizeBits & 7) != 0);
    size_t size1 = sizeBits / 8 + (((sizeBits & 7) == 0) ? 0 : 1);
    assert(data_in.size() == size1);
    int diffBitsNew = (int)(size1 * 8 - sizeBits);
    assert(diffBitsNew >= 0 && diffBitsNew <= 7);
    // all new bits have to be shifted
    size_t diffBitsOld = size() * 8 - _sizeBits;
    assert(diffBitsOld >= 1 && diffBitsOld <= 7);
    byte oldMask = (byte)((byte)0xff << (byte)diffBitsOld);
    size_t newSize = data_in.size();
    for (size_t newIdx = 0; newIdx < newSize; ++newIdx) {
        // first part -- affects current last byte in old
        byte newByte = data_in[newIdx];
        if (newIdx == newSize - 1) {
            // last byte in new
            // zero unused bits
            newByte &= ~((1 << (byte)diffBitsNew) - 1);
            _sizeBits += (8 - diffBitsNew);
        } else {
            _sizeBits += 8;
        }
        byte first = newByte >> (byte)(8 - diffBitsOld);
        _data[_data.size() - 1] = (_data[_data.size() - 1] & oldMask) | first;
        // second part -- add as new byte
        byte second = (byte)(newByte << (byte)diffBitsOld);
        //cerr << (int)first << " " << (int)second << endl;
        if (_sizeBits > size() * 8) {
            _data.push_back(second);
        }
        // set highest unused bit to 1
        int diffBits = (int)(size() * 8 - _sizeBits);
        assert(diffBits >= 0 && diffBits <= 7);
        if (diffBits > 0) {
            _data[_data.size() - 1] |= (1 << (byte)(diffBits - 1));
        }
    }
}

std::string Slice::asBytesStr() const {
    return hex(_data);
}

void Slice::serialize(TW::Data& data_inout) {
    append(data_inout, _data);
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
    hashData.push_back(d2(bits));
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

size_t Cell::serializedSize(bool topLevel) const {
    if (cellCount() == 0) {
        // no children
        return _slice.size() + 2;
    }
    // has children
    size_t ss = _slice.size() + 4;
    for(auto c: _cells) {
        ss += c->serializedSize(false);
    }
    return ss;
}

void Cell::serialize(TW::Data& data_inout, bool topLevel) {
    size_t startIdx = data_inout.size();
    if (topLevel) {
        // magic
        append(data_inout, parse_hex("b5ee9c72"));
        data_inout.push_back(0x41);
        data_inout.push_back(0x01);
        data_inout.push_back(0x03);
        data_inout.push_back(0x01);
        data_inout.push_back(0x00);
        size_t len1 = serializedSize(topLevel);
        data_inout.push_back((byte)len1);
        data_inout.push_back(0x00);
    }
    // slice
    data_inout.push_back((byte)cellCount());
    data_inout.push_back(d2(_slice.sizeBits()));
    append(data_inout, _slice.data());
    // cell refs?
    uint8_t cidx = 0;
    for(auto c: _cells) {
        ++cidx;
        data_inout.push_back(cidx);
    }
    // cells
    for(auto c: _cells) {
        c->serialize(data_inout, false);
    }
    if (topLevel) {
        // CRC32-C
        using crc_32c_type = boost::crc_optimal<32, 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
        crc_32c_type result;
        result.process_bytes(data_inout.data() + startIdx, data_inout.size() - startIdx);
        uint32_t crc = result.checksum();
        data_inout.push_back(crc & 0x000000FF);
        data_inout.push_back((crc & 0x0000FF00) >> 8);
        data_inout.push_back((crc & 0x00FF0000) >> 16);
        data_inout.push_back((crc & 0xFF000000) >> 24);
    }
}

byte Cell::d2(size_t bits) {
    return (byte)((bits / 8) * 2 + (((bits & 7) == 0) ? 0 : 1));
}

} // namespace TW::TON
