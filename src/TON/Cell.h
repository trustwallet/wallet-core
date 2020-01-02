// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <memory>

namespace TW::TON {

/**
 * Simplified implementation of TON Slice, Cell, etc. structures, for minimal represenatation and
 * hash computation.
 */

/// Represents a Slice.
class Slice {
public:
    Slice();
    Slice(const Slice& from);
    virtual ~Slice() {}
    /// Constructor methods.  May throw.
    static Slice createFromData(const Data& data);
    static Slice createFromHex(std::string const& dataStr);
    static Slice createFromBits(const Data& data, size_t sizeBits);
    static Slice createFromBitsStr(std::string const& dataStr, size_t sizeBits);
    void appendBytes(const Data& data_in);
    /// Append bytes, possible incomplete bytes at the end.  SizeBits should be equal to data.size() * 8, or less by at most 7.
    void appendBits(const TW::Data& data_in, size_t sizeBits);
    Data data() const { return _data; }
    inline size_t size() const { return _data.size(); }
    size_t sizeBits() const { return _sizeBits; }
    std::string asBytesStr() const;
    void serialize(TW::Data& data_inout);
    Data hash() const;

protected:
    void appendBitsAligned(const TW::Data& data_in, size_t sizeBits);
    void appendBitsNotAligned(const TW::Data& data_in, size_t sizeBits);

private:
    Data _data;
    size_t _sizeBits;
};

/// Represents a Cell, with references to other cells.
class Cell {
public:
    enum SerializationMode: uint8_t {
        None = 0,
        WithIndex = 1,
        WithCRC32C = 2,
        WithTopHash = 4,
        WithIntHashes = 8,
        WithCacheBits = 16,
        max = 31
    };
    class SerializationInfo {
    public:
        std::vector<uint8_t> magic;
        int rootCount;
        int cellCount;
        //int absent_count;
        int refByteSize;
        int offsetByteSize;
        //bool valid;
        //bool has_index;
        //bool has_roots{false};
        bool hasCrc32c;
        //bool has_cache_bits;
        unsigned long dataSize;
        unsigned long totalSize;
        //unsigned long rootsOffset, dataOffset, indexOffset
    };

public:
    Cell() {}
    Cell(const Cell& from);
    void setSlice(Slice const& slice);
    /// Convenience method for setting slice directly from bytes.  May throw.
    void setSliceBytes(const Data& data);
    /// Convenience method for setting slice directly from hex string.  May throw.
    void setSliceBytesStr(std::string const& sliceStr);
    /// Convenience method for setting slice directly from bits.  May throw.
    void setSliceBitsStr(std::string const& sliceStr, size_t sizeBits);
    void addCell(std::shared_ptr<Cell> const& cell);
    size_t cellCount() const { return _cells.size(); }
    Slice const& getSlice() const { return _slice; }
    const std::vector<std::shared_ptr<Cell>>& getCells() const { return _cells; }
    std::string toString() const;
    Data hash() const;
    /// Serialized size of this cell only, without children
    size_t serializedOwnSize(bool withHashes = false) const;
    /// Serialized size, including children
    size_t serializedSize(SerializationMode mode = SerializationMode::None) const;
    /// Serialize this cell only, without children
    void serializeOwn(TW::Data& data_inout, bool withHashes = false);
    /// Serialize this cell, including children
    void serialize(TW::Data& data_inout, SerializationMode mode = SerializationMode::None);
    static const size_t max_cells = 4;
    /// second byte in length
    static byte d2(size_t bits);
private:
    /// Compute 4-byte CRC32-C checksum, used in serialization
    static uint32_t computeCrc(const byte* data, size_t len);
    // Prepare serialization properties
    SerializationInfo getSerializationInfo(SerializationMode mode = SerializationMode::None) const;

private:
    std::vector<std::shared_ptr<Cell>> _cells;
    Slice _slice;
};

} // namespace TW::TON
