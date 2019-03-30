// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "Address.h"
#include "Forging.h"

#include "../HexCoding.h"
#include "../PublicKey.h"
#include <variant>

using namespace std;
using namespace TW::Tezos;

Data Transaction::forge() {
    auto forged = Data();
    auto forgedSource = source.forge();
    auto forgedFee = forgeZarith(fee);
    auto forgedCounter = forgeZarith(counter);
    auto forgedGasLimit = forgeZarith(gas_limit);
    auto forgedStorageLimit = forgeZarith(storage_limit);

    if (kind == operationtype::REVEAL) {
        if (auto publicKey = std::get_if<PublicKey>(&destination_or_public_key)) {
            forged.push_back(0x07);
            append(forged, forgedSource);
            append(forged, forgedFee);
            append(forged, forgedCounter);
            append(forged, forgedGasLimit);
            append(forged, forgedStorageLimit);
            append(forged, forgePublicKey(*publicKey));
            return forged;
        } else {
            throw std::invalid_argument("Invalid publicKey");
        }
    }
    auto forgedAmount = forgeZarith(amount);
    if (auto destination = std::get_if<Address>(&destination_or_public_key)) {
        forged.push_back(0x08);
        append(forged, forgedSource);
        append(forged, forgedFee);
        append(forged, forgedCounter);
        append(forged, forgedGasLimit);
        append(forged, forgedStorageLimit);
        append(forged, forgedAmount);
        append(forged, destination->forge());
        append(forged, forgeBool(false));
        return forged;
    } else {
        throw std::invalid_argument("Invalid destination");
    }
}
