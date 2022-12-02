// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"

#include "wallet/WalletV4R2.h"

namespace TW::TheOpenNetwork {

namespace {
Address createFromWallet(const PublicKey& publicKey, int8_t workchainId) {
    WalletV4R2 wallet(publicKey, workchainId);
    return wallet.getAddress();
}
} // namespace

bool Address::isValid(const std::string& string) noexcept {
    // TODO: Finalize implementation
    return false;
}

Address::Address(const std::string& string) {
    // TODO: Finalize implementation

    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
}

Address::Address(const PublicKey& publicKey, int8_t workchainId)
    : Address(createFromWallet(publicKey, workchainId)) {
}

std::string Address::string() const {
    // TODO(vbushev): base64url, crc16 calc
    std::string address = std::to_string(workchainId) + ":" + hex(hash);
    return address;
}

} // namespace TW::TheOpenNetwork
