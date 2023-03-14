// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Script.h"
#include "Constants.h"
#include "Serialization.h"
#include "../Bech32.h"

#include <algorithm>
#include <iterator>

namespace TW::Nervos {

Data Script::hash() const {
    Data data;
    encode(data);
    return Hash::blake2b(data, 32, Constants::gHashPersonalization);
}

[[nodiscard]] bool Script::empty() const {
    return std::all_of(codeHash.begin(), codeHash.end(), [](byte element) { return element == 0; });
}

void Script::encode(Data& data) const {
    Data hashTypeData(1);
    Data argsData;
    if (empty()) {
        return;
    }
    hashTypeData[0] = hashType;
    encode32LE(uint32_t(args.size()), argsData);
    argsData.insert(argsData.end(), args.begin(), args.end());
    Serialization::encodeDataArray(std::vector<Data>{codeHash, hashTypeData, argsData}, data);
}

nlohmann::json Script::json() const {
    if (empty()) {
        return nullptr;
    } else {
        return nlohmann::json{{"code_hash", hexEncoded(codeHash)},
                              {"hash_type", HashTypeString[hashType]},
                              {"args", hexEncoded(args)}};
    }
}

} // namespace TW::Nervos
