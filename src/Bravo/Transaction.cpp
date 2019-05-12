#include "Transaction.h"

#include <stdexcept>
#include <ctime>

#include "../Hash.h"
#include "../HexCoding.h"

using namespace TW::Bravo;
using json = nlohmann::json;

/// Signature
Signature::Signature(Data sig): data(sig) {
    if (sig.size() != Signature::DataSize) {
        throw std::invalid_argument("Invalid signature!");
    }
}

void Signature::serialize(Data& os) const noexcept {
    append(os, data);
}

std::string Signature::string() const noexcept {
    return hex(data);
}

Transaction::Transaction(const Data& referenceBlockId, int32_t referenceBlockTime) {
    setReferenceBlock(referenceBlockId);
    expiration = referenceBlockTime + Transaction::ExpirySeconds;
}

void Transaction::setReferenceBlock(const Data& refBlockId) {
    if (refBlockId.size() != Hash::ripemdSize) {
        throw std::invalid_argument("Invalid Reference Block Id!");
    }

    refBlockNumber = decode16BE(refBlockId.data() + 2);
    refBlockPrefix = decode32LE(refBlockId.data() + 4);
}

void Transaction::serialize(Data& os) const noexcept {
    encode16LE(refBlockNumber, os);
    encode32LE(refBlockPrefix, os);
    encode32LE(expiration, os);
    encodePointerCollection(operations, os);
    encodePointerCollection(extensions, os);
}

json Transaction::serialize() const noexcept {
    char formattedDate[20];
    time_t time = expiration;
    if (strftime(formattedDate, 19, "%FT%T", std::gmtime(&time)) != 19) {
        std::runtime_error("Error creating a formatted string!");
    }

    json sigs = json::array();
    for (const auto& sig : signatures) {
        sigs.push_back(sig.string());
    }

    json obj;
    obj["ref_block_num"] = refBlockNumber;
    obj["ref_block_prefix"] = refBlockPrefix;
    obj["expiration"] = std::string(formattedDate, 19);
    obj["operations"] = encodePointerCollection(operations);
    obj["extensions"] = encodePointerCollection(extensions);
    obj["signatures"] = sigs;

    return obj;
}
