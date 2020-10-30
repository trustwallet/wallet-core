// Copyright © 2017-2020 Trust Wallet.
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
    if (sizeBits == 0) {
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

Cell::SerializationInfo Cell::getSerializationInfo(SerializationMode mode) const {
    SerializationInfo info = SerializationInfo();
    size_t rawDataSize = serializedOwnSize();
    for (auto c: _cells) {
        rawDataSize += c->serializedOwnSize();
    }
    int intRefs = (int)cellCount();
    uint8_t refSize = 1;
    while (cellCount() >= ((size_t)1 << (refSize * 8))) { ++refSize; }
    assert(refSize >= 1 && refSize <= 8);
    size_t hashes = 0;
    size_t dataBytesAdj = rawDataSize + (size_t)intRefs * refSize + hashes;
    size_t maxOffset = (mode & SerializationMode::WithCacheBits) ? dataBytesAdj * 2 : dataBytesAdj;
    int offsetSize = 0;
    while (maxOffset >= (1ULL << (offsetSize * 8))) { ++offsetSize; }
    if (refSize > 4 || offsetSize > 8) { return info; }

    info.refByteSize = refSize;
    info.offsetByteSize = 1;
    info.rootCount = 1;
    info.cellCount = info.rootCount + (int)cellCount();  // including self/roots
    info.hasCrc32c = mode & SerializationMode::WithCRC32C;
    int crcSize = info.hasCrc32c ? 4 : 0;
    int rootCount = 0;
    unsigned long rootsOffset = 4 + 1 + 1 + 3 * info.refByteSize + info.offsetByteSize;
    unsigned long indexOffset = rootsOffset + rootCount * info.refByteSize;
    unsigned long dataOffset = indexOffset;
    //if (info.has_index) {
        //info.data_offset += (long long)cell_count * info.offset_byte_size;
    //}
    // Magic num idx 68ff65f3  idxCrc32c acc3a728  generic b5ee9c72
    info.magic = parse_hex("b5ee9c72");
    info.dataSize = dataBytesAdj;
    info.totalSize = dataOffset + dataBytesAdj + crcSize;
    return info;
}

size_t Cell::serializedOwnSize(bool withHashes) const {
    if (withHashes) { throw std::invalid_argument("Cell::serializedOwnSize: WithHashes not supported"); }
    return _slice.size() + 2; // bits/8 rounded up + 2
}

size_t Cell::serializedSize(SerializationMode mode) const {
    auto info = getSerializationInfo(mode);
    size_t ss = 0;
    ss += 4; // magic
    ss += 5; // byte1, offsetByteSize, cellCount, rootCount, 
    ss += info.offsetByteSize; // dataSize
    ss += info.rootCount; // roots

    ss += serializedOwnSize(false);
    ss += cellCount(); // cell refs

    // child cells
    for(auto c: _cells) {
        ss += c->serializedOwnSize(false);
    }

    if (mode & SerializationMode::WithCRC32C) {
        ss += 4;
    }
    return ss;
}

void Cell::serializeOwn(TW::Data& data_inout, bool withHashes) {
    if (withHashes) { throw std::invalid_argument("Cell::serializedOwnSize: WithHashes not supported"); }
    //auto info = getSerializationInfo(mode);
    // slice
    data_inout.push_back((byte)cellCount());
    data_inout.push_back(d2(_slice.sizeBits()));
    append(data_inout, _slice.data());
}

void Cell::serialize(TW::Data& data_inout, SerializationMode mode) {
    if (mode != SerializationMode::None && mode != SerializationMode::WithCRC32C) {
        throw std::invalid_argument("Cell::serialize: Mode " + std::to_string((int)mode) + " not supported");
    }
    // save current start position
    size_t startIdx = data_inout.size();
    auto info = getSerializationInfo(mode);

    // magic
    append(data_inout, info.magic);

    byte byte1 = 0;
    //if (info.hasIndex) { byte |= 1 << 7; }
    if (info.hasCrc32c) { byte1 |= 1 << 6; }
    //if (info.has_cache_bits) { byte |= 1 << 5; }
    // 3, 4 - flags
    if (info.refByteSize < 1 || info.refByteSize > 7) {
        //cerr << info.refByteSize << endl;
        return;
    }
    byte1 |= static_cast<byte>(info.refByteSize);
    data_inout.push_back(byte1);
    data_inout.push_back((byte)info.offsetByteSize);
    data_inout.push_back((byte)info.cellCount);
    data_inout.push_back((byte)info.rootCount);
    data_inout.push_back(0);
    data_inout.push_back((byte)info.dataSize); // offset
    data_inout.push_back(0); // roots

    // own cell (slice)
    serializeOwn(data_inout, false);
    // cell refs?
    uint8_t cidx = 0;
    for(auto c: _cells) {
        ++cidx;
        data_inout.push_back(cidx);
    }

    // child cells
    for(auto c: _cells) {
        c->serializeOwn(data_inout, false);
    }

    if (mode & SerializationMode::WithCRC32C) {
        // CRC32-C, of the serialized data so far
        uint32_t crc = computeCrc(data_inout.data() + startIdx, data_inout.size() - startIdx);
        data_inout.push_back(crc & 0x000000FF);
        data_inout.push_back((crc & 0x0000FF00) >> 8);
        data_inout.push_back((crc & 0x00FF0000) >> 16);
        data_inout.push_back((crc & 0xFF000000) >> 24);
    }
}

uint32_t Cell::computeCrc(const byte* data, size_t len) {
    // CRC32-C
    using crc_32c_type = boost::crc_optimal<32, 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, true, true>;
    crc_32c_type result;
    result.process_bytes((const void*)data, len);
    return result.checksum();
}

byte Cell::d2(size_t bits) {
    return (byte)((bits / 8) * 2 + (((bits & 7) == 0) ? 0 : 1));
}

} // namespace TW::TON
