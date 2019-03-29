#include "Transaction.h"

#include <stdexcept>
#include <ctime>

#include "../Hash.h"
#include "../HexCoding.h"

using namespace TW::EOS;
using json = nlohmann::json;

void Extensions::serialize(Data& os) const noexcept {
    encode16LE(type, os);
    Bravo::encodeVarInt32(buffer.size(), os);
    append(os, buffer);
}

Transaction::Transaction(const std::string& referenceBlockId, uint32_t referenceBlockTime) {
    setReferenceBlock(parse_hex(referenceBlockId));
    expiration = referenceBlockTime + Transaction::ExpirySeconds;
}

void Transaction::setReferenceBlock(const Data& refBlockId) {
    if (refBlockId.size() != Hash::ripemdSize) {
        throw std::invalid_argument("Invalid Reference Block Id!");
    }

    refBlockNumber = decode16BE(refBlockId.data() + 2);
    refBlockPrefix = decode32LE(refBlockId.data() + 4);
}

void Transaction::serialize(Data& os) const noexcept{
    using namespace Bravo;

    encode32LE(expiration, os);
    encode16LE(refBlockNumber, os);
    encode32LE(refBlockPrefix, os);
    encodeVarInt32(maxNetUsageWords, os);
    os.push_back(maxCPUUsageInMS);
    encodeVarInt32(delaySeconds, os);

    encodeCollection(contextFreeActions, os);
    encodeCollection(actions, os);
    encodeCollection(transactionExtensions, os);
}

json Transaction::serialize() const noexcept {
    using namespace Bravo;

    char formattedDate[20];
    time_t time = expiration;
    if (strftime(formattedDate, 19, "%FT%T", std::gmtime(&time)) != 19) {
        std::runtime_error("Error creating a formatted string!");
    }

    json sigs = json::array();
    for (const auto& sig : signatures) {
        sigs.push_back(sig.string());
    }

    json cfdJSON = json::array();
    for (const auto& d : contextFreeData) {
        cfdJSON.push_back(hex(d));
    }

    json obj;
    obj["ref_block_num"] = refBlockNumber;
    obj["ref_block_prefix"] = refBlockPrefix;
    obj["expiration"] = std::string(formattedDate, 19);
    obj["max_net_usage_words"] = maxNetUsageWords;
    obj["max_cpu_usage_ms"] = maxCPUUsageInMS;
    obj["delay_sec"] = delaySeconds;
    obj["context_free_actions"] = encodeCollection(contextFreeActions);
    obj["actions"] = encodeCollection(actions);
    obj["transaction_extensions"] = encodeCollection(transactionExtensions);
    obj["signatures"] = sigs;
    obj["context_free_data"] = cfdJSON;

    return obj;
}
