// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bitcoin/CashAddress.h"
#include "../PublicKey.h"

#include <TrezorCrypto/cash_addr.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWBitcoinCashAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <cassert>
#include <cstring>
#include <vector>

static const size_t dataSize = 34;
static const char* const hrp = "bitcoincash";

bool TWBitcoinCashAddressEqual(struct TWBitcoinCashAddress lhs, struct TWBitcoinCashAddress rhs) {
    return std::memcmp(lhs.bytes, rhs.bytes, dataSize) == 0;
}

bool TWBitcoinCashAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == dataSize && (TWDataGet(data, 0) == 0 || TWDataGet(data, 0) == 1);
}

bool TWBitcoinCashAddressIsValidString(TWString *_Nonnull string) {
    auto& stdString = *reinterpret_cast<const std::string*>(string);
    return TW::Bitcoin::CashAddress::isValid(stdString);
}

bool TWBitcoinCashAddressInitWithString(struct TWBitcoinCashAddress *_Nonnull address, TWString *_Nonnull string) {
    auto& stdString = *reinterpret_cast<const std::string*>(string);
    try {
        const auto addr = TW::Bitcoin::CashAddress(stdString);
        std::copy(addr.bytes.begin(), addr.bytes.end(), address->bytes);
        return true;
    } catch (std::exception) {
        return false;
    }
}

bool TWBitcoinCashAddressInitWithData(struct TWBitcoinCashAddress *_Nonnull address, TWData *_Nonnull data) {
    if (!TWBitcoinCashAddressIsValid(data)) {
        return false;
    }
    TWDataCopyBytes(data, 0, dataSize, address->bytes);
    return true;
}

void TWBitcoinCashAddressInitWithPublicKey(struct TWBitcoinCashAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey) {
    uint8_t payload[21];
    payload[0] = 0;
    ecdsa_get_pubkeyhash(publicKey->impl.bytes.data(), HASHER_SHA2_RIPEMD, payload + 1);

    size_t outlen = 0;
    cash_addr_to_data(address->bytes, &outlen, payload, 21);
}

TWString *_Nonnull TWBitcoinCashAddressDescription(struct TWBitcoinCashAddress address) {
    char result[104];
    cash_encode(result, hrp, address.bytes, dataSize);
    return TWStringCreateWithUTF8Bytes(result);
}

TWBitcoinAddress *_Nonnull TWBitcoinCashAddressLegacyAddress(struct TWBitcoinCashAddress address) {
    auto result = TW::Data(TW::Bitcoin::Address::size);
    size_t outlen = 0;
    cash_data_to_addr(result.data(), &outlen, address.bytes, dataSize);
    assert(outlen == 21 && "Invalid length");
    return new TWBitcoinAddress{ TW::Bitcoin::Address(result) };
}
