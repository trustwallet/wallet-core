// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../Data.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace TW;
using namespace TW::ARK;

Data Transaction::encoded() const {

    // transaction type
    auto data = Data{type};

    encode32LE(timestamp, data);
    append(data, publicKey);
    append(data, Data(to.bytes.begin(), to.bytes.end()));

    // vendor field
    Data vendor(64, 0);
    append(data, vendor);

    encode64LE(amount, data);
    encode64LE(fee, data);

    if (signature.size() > 0) {
        append(data, signature);
    }
    return data;
}

std::string Transaction::encodedJson() const {
    json j = {{"amount", amount},
              {"asset", {}},
              {"fee", fee},
              {"id", hex(Hash::sha256(encoded()))},
              {"recipientId", to.string()},
              {"senderPublicKey", hex(publicKey)},
              {"signature", hex(signature)},
              {"timestamp", timestamp},
              {"type", type}};
    return j.dump();
}
