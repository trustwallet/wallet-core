#include "Cell.h"

#include <map>

#include <TrezorCrypto/sha2.h>

#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::Everscale;

using CellHash = decltype(Cell::hash);

void encodeCellData(const Cell& cell, Data& os) {
    const auto len = 2 + (cell.bitLen + 7) / 8;
    os.reserve(os.size() + len);

    const auto [d1, d2] = cell.getDescriptorBytes();
    os.push_back(d1);
    os.push_back(d2);
    os.insert(os.end(), cell.data.begin(), cell.data.end());
}

struct TraverseContext {
    uint16_t index;
    std::map<CellHash, uint16_t> indices{};
    std::vector<const Cell*> reversedCells{};
};

void traverseCells(const std::shared_ptr<Cell>& cell, TraverseContext& ctx) {
    if (ctx.indices.find(cell->hash) != ctx.indices.end()) {
        return;
    }

    for (const auto& ref : cell->references) {
        if (ref == nullptr) {
            break;
        }
        traverseCells(ref, ctx);
    }

    ctx.indices.insert(std::make_tuple(cell->hash, ctx.index));
}

void Cell::serialize(Data& os) const {
    constexpr uint8_t REF_SIZE = 2;
    constexpr uint8_t OFFSET_SIZE = 2;

    encode32BE(0xb5ee9c72, os); // magic
    os.push_back(REF_SIZE);
    os.push_back(OFFSET_SIZE);
    encode16BE(0, os); // cell count
    encode16BE(1, os); // root count
    encode16BE(0, os); // absent cell count

    encode16BE(0, os); // TODO: total data size

    encode16BE(0, os); // root cell index
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
