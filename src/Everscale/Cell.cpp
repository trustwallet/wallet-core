#include "Cell.h"

#include <cassert>
#include <map>
#include <optional>
#include <unordered_map>

#include <TrezorCrypto/sha2.h>

#include "../BinaryCoding.h"

using namespace TW;

namespace TW::Everscale {

constexpr static uint32_t BOC_MAGIC = 0xb5ee9c72;

uint16_t computeBitLen(const Data& data, bool aligned) {
    auto bitLen = static_cast<uint16_t>(data.size() * 8);
    if (aligned) {
        return bitLen;
    }

    for (size_t i = data.size() - 1; i >= 0; ++i) {
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

    inline const uint8_t* _Nonnull data() const {
        return buffer + offset;
    }

    inline uint32_t readNextU32() {
        const auto* _Nonnull p = data();
        advance(sizeof(uint32_t));
        return decode32BE(p);
    }

    inline size_t readNextUint(uint8_t len) {
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

std::shared_ptr<Cell> Cell::deserialize(const uint8_t* _Nonnull data, size_t len) {
    Reader reader(data, len);

    // Parse header
    reader.require(6);
    // 1. Magic
    if (reader.readNextU32() != BOC_MAGIC) {
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

    std::map<size_t, IntermediateCell> intermediate{};

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
            if (index > cellCount || index < i) {
                throw std::runtime_error("invalid child index");
            }

            references.push_back(index);
        }

        const auto bitLen = computeBitLen(cellData, (d2 & 0b1) == 0);
        intermediate.emplace(
            i,
            IntermediateCell{
                .bitLen = bitLen,
                .data = std::move(cellData),
                .references = std::move(references),
            });
    }

    std::unordered_map<size_t, Cell::Ref> doneCells{};

    size_t index = cellCount;
    for (auto it = intermediate.rbegin(); it != intermediate.rend(); ++it, --index) {
        auto& raw = it->second;

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
        os.reserve(os.size() + serializedSize);

        const auto cellCount = static_cast<uint16_t>(reversedCells.size());

        // Write header
        encode32BE(BOC_MAGIC, os);
        os.push_back(REF_SIZE);
        os.push_back(OFFSET_SIZE);
        encode16BE(static_cast<uint16_t>(cellCount), os);
        encode16BE(1, os); // root count
        encode16BE(0, os); // absent cell count
        encode16BE(static_cast<uint16_t>(serializedSize), os);
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
    constexpr static uint8_t REF_SIZE = 2;
    constexpr static uint8_t OFFSET_SIZE = 2;

    // NOTE: Initialized with header size
    size_t serializedSize =
        /*magic*/ sizeof(BOC_MAGIC) +
        /*ref_size*/ 1 +
        /*offset_size*/ 1 +
        /*cell_count*/ REF_SIZE +
        /*root_count*/ REF_SIZE +
        /*absent_count*/ REF_SIZE +
        /*data_size*/ OFFSET_SIZE +
        /*root_cell_index*/ REF_SIZE;

    uint16_t index = 0;
    std::map<Cell::CellHash, uint16_t> indices{};
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
        ctx.serializedSize += cell.serializedSize();
    }
};

void Cell::serialize(Data& os) const {
    if (!finalized) {
        throw std::runtime_error("cell was not finalized");
    }
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
    // NOTE: raw context is used here to reduce allocations
    SHA256_CTX context{};
    sha256_Init(&context);

    // Write descriptor bytes
    const auto [d1, d2] = getDescriptorBytes();
    sha256_Update(&context, &d1, 1);
    sha256_Update(&context, &d2, 1);

    // Write data
    sha256_Update(&context, data.data(), data.size());

    // Write all children depths
    for (const auto& ref : references) {
        if (ref == nullptr) {
            break;
        }
        const auto depthHigh = static_cast<uint8_t>(ref->depth >> 8);
        sha256_Update(&context, &depthHigh, 1);
        const auto depthLow = static_cast<uint8_t>(ref->depth);
        sha256_Update(&context, &depthLow, 1);
    }

    // Write all children hashes
    for (const auto& ref : references) {
        if (ref == nullptr) {
            break;
        }
        sha256_Update(&context, ref->hash.data(), Hash::sha256Size);
    }

    // Done
    sha256_Final(&context, hash.data());
    finalized = true;
}

} // namespace TW::Everscale
