// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cell.h"

#include <cassert>
#include <map>
#include <optional>
#include <unordered_map>

#include "BinaryCoding.h"

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

using namespace TW;

namespace TW::CommonTON {

constexpr static uint32_t BOC_MAGIC = 0xb5ee9c72;

uint16_t computeBitLen(const Data& data, bool aligned) {
    auto bitLen = static_cast<uint16_t>(data.size() * 8);
    if (aligned) {
        return bitLen;
    }

    for (auto i = static_cast<int64_t>(data.size() - 1); i >= 0; --i) {
        if (data[i] == 0) {
            bitLen -= 8;
        } else {
            auto skip = 1;
            uint8_t mask = 1;
            while ((data[i] & mask) == 0) {
                skip += 1;
                mask <<= 1;
            }
            bitLen -= skip;
            break;
        }
    }
    return bitLen;
}

struct Reader {
    const uint8_t* _Nonnull buffer;
    size_t bufferLen;
    size_t offset = 0;

    explicit Reader(const uint8_t* _Nonnull buffer, size_t len) noexcept
        : buffer(buffer), bufferLen(len) {
    }

    void require(size_t bytes) const {
        if (offset + bytes > bufferLen) {
            throw std::runtime_error("unexpected eof");
        }
    }

    void advance(size_t bytes) {
        offset += bytes;
    }

    const uint8_t* _Nonnull data() const {
        return buffer + offset;
    }

    size_t readNextUint(uint8_t len) {
        const auto* _Nonnull p = data();
        advance(len);

        switch (len) {
        case 1:
            return static_cast<size_t>(*p);
        case 2:
            return static_cast<size_t>(decode16BE(p));
        case 3:
            return static_cast<size_t>(p[2]) | (static_cast<size_t>(p[1]) << 8) | (static_cast<size_t>(p[0]) << 16);
        case 4:
            return static_cast<size_t>(decode32BE(p));
        default:
            // Unreachable in valid cells
            return 0;
        }
    }
};

std::shared_ptr<Cell> Cell::fromBase64(const std::string& encoded) {
    // `Hash::base64` trims \0 bytes from the end of the _decoded_ data, so
    // raw transform is used here
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    Data boc(It(begin(encoded)), It(end(encoded)));
    return Cell::deserialize(boc.data(), boc.size());
}

std::shared_ptr<Cell> Cell::deserialize(const uint8_t* _Nonnull data, size_t len) {
    Reader reader(data, len);

    // Parse header
    reader.require(6);
    // 1. Magic
    if (reader.readNextUint(sizeof(BOC_MAGIC)) != BOC_MAGIC) {
        throw std::runtime_error("unknown magic");
    }
    // 2. Flags
    struct Flags {
        uint8_t refSize : 3;
        uint8_t : 2; // unused
        bool hasCacheBits : 1;
        bool hasCrc : 1;
        bool indexIncluded : 1;
    };

    static_assert(sizeof(Flags) == 1, "flags must be represented as 1 byte");
    const auto flags = reinterpret_cast<const Flags*>(reader.data())[0];
    const auto refSize = flags.refSize;
    const auto offsetSize = reader.data()[1];
    reader.advance(2);

    // 3. Counters and root index
    reader.require(refSize * 3 + offsetSize + refSize);
    const auto cellCount = reader.readNextUint(refSize);
    const auto rootCount = reader.readNextUint(refSize);
    if (rootCount != 1) {
        throw std::runtime_error("unsupported root count");
    }
    if (rootCount > cellCount) {
        throw std::runtime_error("root count is greater than cell count");
    }
    const auto absent_count = reader.readNextUint(refSize);
    if (absent_count > 0) {
        throw std::runtime_error("absent cells are not supported");
    }

    reader.readNextUint(offsetSize); // total cell size

    const auto rootIndex = reader.readNextUint(refSize);

    // 4. Cell offsets (skip if specified)
    if (flags.indexIncluded) {
        reader.advance(cellCount * offsetSize);
    }

    // 5. Deserialize cells
    struct IntermediateCell {
        uint16_t bitLen;
        Data data;
        std::vector<size_t> references;
    };

    std::vector<IntermediateCell> intermediate{};
    intermediate.reserve(cellCount);

    for (size_t i = 0; i < cellCount; ++i) {
        struct Descriptor {
            uint8_t refCount : 3;
            bool exotic : 1;
            bool storeHashes : 1;
            uint8_t level : 3;
        };

        static_assert(sizeof(Descriptor) == 1, "cell descriptor must be represented as 1 byte");

        reader.require(2);
        const auto d1 = reinterpret_cast<const Descriptor*>(reader.data())[0];
        if (d1.level != 0) {
            throw std::runtime_error("non-zero level is not supported");
        }
        if (d1.exotic) {
            throw std::runtime_error("exotic cells are not supported");
        }
        if (d1.refCount == 7 && d1.storeHashes) {
            throw std::runtime_error("absent cells are not supported");
        }
        if (d1.refCount > 4) {
            throw std::runtime_error("invalid ref count");
        }
        const auto d2 = reader.data()[1];
        const auto byteLen = (d2 >> 1) + (d2 & 0b1);
        reader.advance(2);

        // Skip stored hashes
        if (d1.storeHashes) {
            reader.advance(sizeof(uint16_t) + Hash::sha256Size);
        }

        reader.require(byteLen);
        Data cellData(byteLen);
        std::memcpy(cellData.data(), reader.data(), byteLen);
        reader.advance(byteLen);

        std::vector<size_t> references{};
        references.reserve(refSize * d1.refCount);
        reader.require(refSize * d1.refCount);
        for (size_t r = 0; r < d1.refCount; ++r) {
            const auto index = reader.readNextUint(refSize);
            if (index > cellCount || index <= i) {
                throw std::runtime_error("invalid child index");
            }

            references.push_back(index);
        }

        const auto bitLen = computeBitLen(cellData, (d2 & 0b1) == 0);
        intermediate.emplace_back(
            IntermediateCell{
                .bitLen = bitLen,
                .data = std::move(cellData),
                .references = std::move(references),
            });
    }

    std::unordered_map<size_t, Cell::Ref> doneCells{};

    size_t index = cellCount;
    for (auto it = intermediate.rbegin(); it != intermediate.rend(); ++it, --index) {
        auto& raw = *it;

        Cell::Refs references{};
        for (size_t r = 0; r < raw.references.size(); ++r) {
            const auto child = doneCells.find(raw.references[r]);
            if (child == doneCells.end()) {
                throw std::runtime_error("child cell not found");
            }
            references[r] = child->second;
        }

        auto cell = std::make_shared<Cell>(raw.bitLen, std::move(raw.data), raw.references.size(), std::move(references));
        cell->finalize();
        doneCells.emplace(index - 1, cell);
    }

    const auto root = doneCells.find(rootIndex);
    if (root == doneCells.end()) {
        throw std::runtime_error("root cell not found");
    }
    return std::move(root->second);
}

class SerializationContext {
public:
    static SerializationContext build(const Cell& cell) {
        SerializationContext ctx{};
        fillContext(cell, ctx);
        return ctx;
    }

    void encode(Data& os) const {
        os.reserve(os.size() + HEADER_SIZE + cellsSize);

        const auto cellCount = static_cast<ref_t>(reversedCells.size());

        // Write header
        encode32BE(BOC_MAGIC, os);
        os.push_back(REF_SIZE);
        os.push_back(OFFSET_SIZE);
        encode16BE(static_cast<ref_t>(cellCount), os);
        encode16BE(1, os); // root count
        encode16BE(0, os); // absent cell count
        encode16BE(static_cast<offset_t>(cellsSize), os);
        encode16BE(0, os); // root cell index

        // Write cells
        size_t i = cellCount - 1;
        while (true) {
            const auto& cell = *reversedCells[i];

            // Write cell data
            const auto [d1, d2] = cell.getDescriptorBytes();
            os.push_back(d1);
            os.push_back(d2);
            os.insert(os.end(), cell.data.begin(), cell.data.end());

            // Write cell references
            for (const auto& child : cell.references) {
                if (child == nullptr) {
                    break;
                }

                // Map cell hash to index (which must be presented)
                const auto it = indices.find(child->hash);
                assert(it != indices.end());

                encode16BE(cellCount - it->second - 1, os);
            }

            if (i == 0) {
                break;
            } else {
                --i;
            }
        }
    }

private:
    // uint16_t will be enough for wallet transactions (e.g. 64k is the size of the whole elector)
    using ref_t = uint16_t;
    using offset_t = uint16_t;

    constexpr static uint8_t REF_SIZE = sizeof(ref_t);
    constexpr static uint8_t OFFSET_SIZE = sizeof(offset_t);
    constexpr static size_t HEADER_SIZE =
        /*magic*/ sizeof(BOC_MAGIC) +
        /*ref_size*/ 1 +
        /*offset_size*/ 1 +
        /*cell_count*/ REF_SIZE +
        /*root_count*/ REF_SIZE +
        /*absent_count*/ REF_SIZE +
        /*data_size*/ OFFSET_SIZE +
        /*root_cell_index*/ REF_SIZE;

    size_t cellsSize = 0;
    ref_t index = 0;
    std::map<Cell::CellHash, ref_t> indices{};
    std::vector<const Cell* _Nonnull> reversedCells{};

    static void fillContext(const Cell& cell, SerializationContext& ctx) {
        if (ctx.indices.find(cell.hash) != ctx.indices.end()) {
            return;
        }

        for (const auto& ref : cell.references) {
            if (ref == nullptr) {
                break;
            }
            fillContext(*ref, ctx);
        }

        ctx.indices.insert(std::make_pair(cell.hash, ctx.index++));
        ctx.reversedCells.emplace_back(&cell);
        ctx.cellsSize += cell.serializedSize(REF_SIZE);
    }
};

void Cell::serialize(Data& os) const {
    assert(finalized);
    const auto ctx = SerializationContext::build(*this);
    ctx.encode(os);
}

void Cell::finalize() {
    if (finalized) {
        return;
    }

    if (bitLen > Cell::MAX_BITS || refCount > Cell::MAX_REFS) {
        throw std::invalid_argument("invalid cell");
    }

    // Finalize child cells and update current cell depth
    // NOTE: Use before context creation to reduce stack size
    for (const auto& ref : references) {
        if (ref == nullptr) {
            break;
        }
        ref->finalize();
        depth = std::max(depth, static_cast<uint16_t>(ref->depth + 1));
    }

    // Compute cell hash
    const auto dataSize = std::min(data.size(), static_cast<size_t>((bitLen + 7) / 8));

    Data normalized{};
    normalized.reserve(/* descriptor bytes */ 2 + dataSize + refCount * (sizeof(uint16_t) + Hash::sha256Size));

    // Write descriptor bytes
    const auto [d1, d2] = getDescriptorBytes();
    normalized.push_back(d1);
    normalized.push_back(d2);
    std::copy(data.begin(), std::next(data.begin(), static_cast<ssize_t>(dataSize)), std::back_inserter(normalized));

    // Write all children depths
    for (const auto& ref : references) {
        if (ref == nullptr) {
            break;
        }
        encode16BE(ref->depth, normalized);
    }

    // Write all children hashes
    for (const auto& ref : references) {
        if (ref == nullptr) {
            break;
        }
        std::copy(ref->hash.begin(), ref->hash.end(), std::back_inserter(normalized));
    }

    // Done
    const auto computedHash = Hash::sha256(normalized);
    assert(computedHash.size() == Hash::sha256Size);

    std::copy(computedHash.begin(), computedHash.end(), hash.begin());
    finalized = true;
}

} // namespace TW::CommonTON
