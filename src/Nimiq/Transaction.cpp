// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"

#include "Signer.h"
#include "../BinaryCoding.h"

namespace TW::Nimiq {

const uint8_t EMPTY_FLAGS = 0;

std::vector<uint8_t> Transaction::serialize() const {
    // Source code:
    // https://github.com/nimiq/core-rs-albatross/blame/b8ed402c9096ffb54afea52347b91ab7831e75de/primitives/transaction/src/lib.rs#L699

    std::vector<uint8_t> data;

    data.push_back(0x00); // Basic TX type
    if (isAlbatross()) {
        data.push_back(0x00); // Signature Proof type and flags (Ed25519 type and no flags)
    }
    data.insert(data.end(), sender_pub_key.begin(), sender_pub_key.end());
    data.insert(data.end(), destination.bytes.begin(), destination.bytes.end());
    encode64BE(amount, data);
    encode64BE(fee, data);
    encode32BE(vsh, data);
    data.push_back(consensusNetworkId());
    data.insert(data.end(), signature.begin(), signature.end());

    return data;
}

std::vector<uint8_t> Transaction::getPreImage() const {
    // Source code:
    // https://github.com/nimiq/core-rs-albatross/blame/b8ed402c9096ffb54afea52347b91ab7831e75de/primitives/transaction/src/lib.rs#L582

    std::vector<uint8_t> data;

    // Build pre-image
    Address sender(Signer::publicKeyFromBytes(sender_pub_key));
    encode16BE(0x00, data); // Data size (+ 0 bytes of data)
    data.insert(data.end(), sender.bytes.begin(), sender.bytes.end());
    data.push_back(0); // Sender is basic account type
    data.insert(data.end(), destination.bytes.begin(), destination.bytes.end());
    data.push_back(0); // Recipient is basic account type
    encode64BE(amount, data);
    encode64BE(fee, data);
    encode32BE(vsh, data);
    data.push_back(consensusNetworkId());
    data.push_back(EMPTY_FLAGS);
    if (isAlbatross()) {
        data.push_back(0x00); // Sender Data size (+ 0 bytes of data)
    }

    return data;
}

bool Transaction::isAlbatross() const {
    if (networkId == Proto::NetworkId::MainnetAlbatross) {
        return true;
    }
    return false;
}

uint8_t Transaction::consensusNetworkId() const {
    switch (networkId) {
        case Proto::NetworkId::UseDefault:
        case Proto::NetworkId::Mainnet:
            return static_cast<uint8_t>(Proto::NetworkId::Mainnet);
        case Proto::NetworkId::MainnetAlbatross:
            return static_cast<uint8_t>(Proto::NetworkId::MainnetAlbatross);
        default:
            throw std::invalid_argument("Invalid network ID");
    }
}

} // namespace TW::Nimiq
