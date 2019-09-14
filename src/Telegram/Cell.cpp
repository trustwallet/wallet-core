// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cell.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <cassert>

using namespace TW::Telegram;

Slice::Slice()
{
    _data = nullptr;
    _size = 0;
    _sizeBits = 0;
}

Slice::~Slice()
{
    if (_data != nullptr)
    {
        delete[] _data;
        _data = nullptr;
    }
}

Slice & Slice::operator=(const Slice & from)
{
    _size = from._size;
    _sizeBits = from._sizeBits;
    _data = nullptr;
    if (from._data != nullptr)
    {
        _data = new unsigned char[_size];
        assert(from._data != nullptr);
        std::copy(from._data, from._data + from._size, _data);
    }
    return *this;
}

bool Slice::loadFromBytes(unsigned char* data, size_t size)
{
    if (_data != nullptr || _size != 0) return false; // already loaded
    assert(_data == nullptr);
    assert(_size == 0);
    if (data == nullptr) return false; // null ptr
    assert(data != nullptr);
    _size = size;
    _sizeBits = size * 8;
    // allocate and copy
    _data = new unsigned char[_size];
    assert(data != nullptr);    
    std::copy(data, data + size, _data);
    return true;
}

bool Slice::loadFromBytes(TW::Data data)
{
    return loadFromBytes(data.data(), data.size());
}

bool Slice::loadFromBytesStr(std::string const & dataStr)
{
    TW::Data data = TW::parse_hex(dataStr);
    return loadFromBytes(data.data(), data.size());
}

bool Slice::loadFromBits(unsigned char* data, size_t size, size_t sizeBits)
{
    // compute number of bytes needed
    size_t size1 = sizeBits / 8 + (((sizeBits & 7) == 0) ? 0 : 1);
    if (size < size1) return false; // to few bytes given
    assert(size >= size1);
    // fill bytes ...
    if (!loadFromBytes(data, size1)) return false;
    // ... and adjust sizeBits and last byte
    _sizeBits = sizeBits;
    if ((_sizeBits & 7) != 0)
    {
        size_t diffBits = _size * 8 - _sizeBits;
        assert(diffBits > 0 && diffBits <= 7);
        // zero unused bits
        _data[_size - 1] &= ~((1 << diffBits) - 1);
        // set highest unused bit to 0
        _data[_size - 1] |= (1 << (diffBits - 1));
    }
    return true;
}

bool Slice::loadFromBits(TW::Data data, size_t sizeBits)
{
    return loadFromBits(data.data(), data.size(), sizeBits);
}

bool Slice::loadFromBitsStr(std::string const & dataStr, size_t sizeBits)
{
    TW::Data data = TW::parse_hex(dataStr);
    return loadFromBits(data.data(), data.size(), sizeBits);
}

std::string Slice::asBytesStr() const
{
    if (_data == nullptr) return "??";
    assert(_data != nullptr);
    return TW::hex(_data, _data + _size);
}

TW::Data Slice::hash() const
{
    if (_data == nullptr) return TW::Data(0);
    assert(_data != nullptr);
    return TW::Hash::sha256(_data, _data + _size);
}

void Cell::setSlice(Slice const & slice)
{
    _slice = slice;
}

bool Cell::setSliceBytes(unsigned char* data, size_t size)
{
    Slice s;
    if (!s.loadFromBytes(data, size)) return false;
    setSlice(s);
    return true;
}

bool Cell::setSliceBytesStr(std::string const & sliceStr)
{
    Slice s;
    if (!s.loadFromBytesStr(sliceStr)) return false;
    setSlice(s);
    return true;
}

bool Cell::setSliceBitsStr(std::string const & sliceStr, size_t sizeBits)
{
    Slice s;
    if (!s.loadFromBitsStr(sliceStr, sizeBits)) return false;
    setSlice(s);
    return true;
}

bool Cell::addCell(std::shared_ptr<Cell> const & cell)
{
    if (cell_count() >= max_cells) return false; // too many cells
    _cells.push_back(cell);
    return true;
}

std::string Cell::toString() const
{
    std::stringstream s;
    s << "Cell: ";
    if (_slice.size() == 0)
    {
        s << " no slice";
    }
    else
    {
        s << " slice: " << _slice.asBytesStr();
    }
    if (cell_count() == 0)
    {
        s << ", no children";
    }
    else
    {
        s << ", " << cell_count() << " children";        
        int cnt = 1;
        for(auto i = _cells.begin(), n = _cells.end(); i != n; ++i, ++cnt)
        {
            s << std::endl << "  child " << cnt << ": " << i->get()->toString();
        }
    }
    return s.str();
}

TW::Data Cell::hash() const
{
    // Need to copy data together into a contiguous area
    std::vector<unsigned char> hashData;
    // number of children
    hashData.push_back(cell_count());
    // number of hex digits 
    size_t bits = _slice.sizeBits(); // may be 0
    hashData.push_back(d2(bits));
    // data
    if (_slice.size() > 0)
    {
        hashData.insert(hashData.end(), _slice.data(), _slice.data() + _slice.size());
    }
    // children
    if (cell_count() > 0)
    {
        for(auto i = _cells.begin(), n = _cells.end(); i != n; ++i)
        {
            hashData.push_back(0);
            hashData.push_back(0);
        }
        for(auto i = _cells.begin(), n = _cells.end(); i != n; ++i)
        {
            auto childHash = i->get()->hash();
            hashData.insert(hashData.end(), childHash.begin(), childHash.end());
        }
    }
    //std::cerr << "hashData size " << hashData.size() << std::endl;
    // compute hash
    return TW::Hash::sha256(hashData.data(), hashData.data() + hashData.size());
}

size_t Cell::d2(size_t bits)
{
    return (bits / 8) * 2 + (((bits & 7) == 0) ? 0 : 1);
}
