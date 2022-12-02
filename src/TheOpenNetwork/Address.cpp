// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "Base64.h"
#include "Crc.h"

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
    if (!Address::isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
}

Address::Address(const PublicKey& publicKey, int8_t workchainId)
    : Address(createFromWallet(publicKey, workchainId)) {
}

std::string Address::string() const {
    Data data;
    Data hashData(hash.begin(), hash.end());

    append(data, 0x11);         // bounceable
    append(data, workchainId);
    append(data, hashData);

    const uint16_t crc16 = Crc::crc16(data.data(), (uint32_t) data.size());

    append(data, (crc16 >> 8) & 0xff);
    append(data, crc16 & 0xff);

    return Base64::encodeBase64Url(data);
}

} // namespace TW::TheOpenNetwork
