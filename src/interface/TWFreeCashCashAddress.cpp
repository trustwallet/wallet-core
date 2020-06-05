// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFreeCashCashAddress.h>

#include "../FreeCash/CashAddress.h"
#include "../PublicKey.h"

#include <TrezorCrypto/cash_addr.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <cassert>
#include <cstring>
#include <vector>

static const size_t dataSize = 34;
static const char* const hrp = "freecash";

bool TWFreeCashCashAddressEqual(struct TWFreeCashCashAddress lhs, struct TWFreeCashCashAddress rhs) {
    return std::memcmp(lhs.bytes, rhs.bytes, dataSize) == 0;
}

bool TWFreeCashCashAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == dataSize && (TWDataGet(data, 0) == 0 || TWDataGet(data, 0) == 1);
}

bool TWFreeCashCashAddressIsValidString(TWString *_Nonnull string) {
    auto& stdString = *reinterpret_cast<const std::string*>(string);
    return TW::FreeCash::CashAddress::isValid(stdString);
}

bool TWFreeCashCashAddressInitWithString(struct TWFreeCashCashAddress *_Nonnull address, TWString *_Nonnull string) {
    auto& stdString = *reinterpret_cast<const std::string*>(string);
    try {
        const auto addr = TW::FreeCash::CashAddress(stdString);
        std::copy(addr.bytes.begin(), addr.bytes.end(), address->bytes);
        return true;
    } catch (std::exception) {
        return false;
    }
}

bool TWFreeCashCashAddressInitWithData(struct TWFreeCashCashAddress *_Nonnull address, TWData *_Nonnull data) {
    if (!TWFreeCashCashAddressIsValid(data)) {
        return false;
    }
    TWDataCopyBytes(data, 0, dataSize, address->bytes);
    return true;
}

void TWFreeCashCashAddressInitWithPublicKey(struct TWFreeCashCashAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey) {
    uint8_t payload[21];
    payload[0] = 0;
    ecdsa_get_pubkeyhash(publicKey->impl.bytes.data(), HASHER_SHA2_RIPEMD, payload + 1);

    size_t outlen = 0;
    cash_addr_to_data(address->bytes, &outlen, payload, 21);
}

TWString *_Nonnull TWFreeCashCashAddressDescription(struct TWFreeCashCashAddress address) {
    char result[104];
    cash_encode(result, hrp, address.bytes, dataSize);
    return TWStringCreateWithUTF8Bytes(result);
}

TWFreeCashAddress *_Nonnull TWFreeCashCashAddressLegacyAddress(struct TWFreeCashCashAddress address) {
    auto result = TW::Data(TW::FreeCash::Address::size);
    size_t outlen = 0;
    cash_data_to_addr(result.data(), &outlen, address.bytes, dataSize);
    assert(outlen == 21 && "Invalid length");
    return new TWFreeCashAddress{ TW::FreeCash::Address(result) };
}
