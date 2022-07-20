#include "Cell.h"

#include <cassert>
#include <map>

#include <TrezorCrypto/sha2.h>

#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::Everscale;

using CellHash = decltype(Cell::hash);

constexpr static uint32_t BOC_MAGIC = 0xb5ee9c72;

class SerializationContext {
public:
    static SerializationContext build(const Cell& cell) {
        SerializationContext ctx{};
        fillContext(cell, ctx);
        return ctx;
    }

    void encode(Data& os) {
        os.reserve(os.size() + serializedSize);

        const auto cellCount = reversedCells.size();

        encode32BE(BOC_MAGIC, os);
        os.push_back(REF_SIZE);
        os.push_back(OFFSET_SIZE);
        encode16BE(static_cast<uint16_t>(cellCount), os);
        encode16BE(1, os); // root count
        encode16BE(0, os); // absent cell count
        encode16BE(static_cast<uint16_t>(serializedSize), os);
        encode16BE(0, os); // root cell index

        for (auto i = cellCount - 1; i >= 0; --i) {
            const auto& cell = *reversedCells[i];

            // Write cell data
            const auto [d1, d2] = cell.getDescriptorBytes();
            os.push_back(d1);
            os.push_back(d2);
            os.insert(os.end(), cell.data.begin(), cell.data.end());

            // Write cell references
            for (const auto& child : cell.references) {
                // Map cell hash to index (which must be presented)
                const auto it = indices.find(child->hash);
                assert(it != indices.end());

                encode16BE(cellCount - it->second - 1, os);
            }
        }
    }

private:
    constexpr static uint8_t REF_SIZE = 2;
    constexpr static uint8_t OFFSET_SIZE = 2;

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
    std::map<CellHash, uint16_t> indices{};
    std::vector<const Cell*> reversedCells{};

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
        ctx.reversedCells.push_back(&cell);
        ctx.serializedSize += cell.serializedSize();
    }
};

void Cell::serialize(Data& os) const {
    if (!finalized) {
        throw std::runtime_error("cell was not finalized");
    }
    SerializationContext::build(*this).encode(os);
}

void Cell::finalize() {
    if (finalized) {
        return;
    }

    if (bitLen > 1024 || refCount > 4) {
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
