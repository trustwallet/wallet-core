// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "OperationList.h"
#include "Forging.h"
#include "../Base58.h"

namespace TW::Tezos {

Tezos::OperationList::OperationList(const std::string& str) {
    branch = str;
}

void Tezos::OperationList::addOperation(const Operation& operation) {
    operation_list.push_back(operation);
}

// Forge the given branch to a hex encoded string.
Data Tezos::OperationList::forgeBranch() const {
    std::array<byte, 2> prefix = {1, 52};
    const auto decoded = Base58::decodeCheck(branch);
    if (decoded.size() != 34 || !std::equal(prefix.begin(), prefix.end(), decoded.begin())) {
        throw std::invalid_argument("Invalid branch for forge");
    }
    auto forged = Data();
    forged.insert(forged.end(), decoded.begin() + prefix.size(), decoded.end());
    return forged;
}

Data Tezos::OperationList::forge(const PrivateKey& privateKey) const {
    auto forged = forgeBranch();

    for (auto operation : operation_list) {
        // If it's REVEAL operation, inject the public key if not specified
        if (operation.kind() == Operation::REVEAL && operation.has_reveal_operation_data()) {
            auto* revealOperationData = operation.mutable_reveal_operation_data();
            if (revealOperationData->public_key().empty()) {
                auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
                revealOperationData->set_public_key(publicKey.bytes.data(), publicKey.bytes.size());
            }
        }

        append(forged, forgeOperation(operation));
    }

    return forged;
}

Data TW::Tezos::OperationList::forge() const {
    auto forged = forgeBranch();

    for (auto operation : operation_list) {
        append(forged, forgeOperation(operation));
    }

    return forged;
}

} // namespace TW::Tezos
