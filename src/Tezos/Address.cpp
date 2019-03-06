// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Tezos/Address.h"
#include <TrustWalletCore/TWTezosAddress.h>

#include <TrustWalletCore/TWBase58.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

#include <cstring>
#include <string>
#include <vector>
#include <array>

using namespace TW;
using namespace TW::Tezos;

bool TWTezosAddressIsValid(TWData *_Nonnull data) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(data);
    size_t size = 0;
    b58enc(nullptr, &size, d, Address::size);
    size += 16;

    std::string str(size, ' ');
    base58_encode_check(d->data(), Address::size, HASHER_SHA2D, &str[0], size);
    return Address::isValid(str);
}

bool TWTezosAddressIsValidString(TWString *_Nonnull string) {
    TWData *data = TWBase58DecodeNoCheck(string);
    if (data == nullptr) {
        return false;
    }
    return TWTezosAddressIsValid(data);
}

bool TWTezosAddressInitWithString(struct TWTezosAddress *_Nonnull address, TWString *_Nonnull string) {
    TWData *data = TWBase58DecodeNoCheck(string);
    if (!TWTezosAddressIsValidString(string) || data == nullptr || TWDataSize(data) != TWTezosAddressSize) {
        return false;
    }

    TWDataCopyBytes(data, 0, TWTezosAddressSize, address->bytes);
    return true;
}

bool TWTezosAddressInitWithPublicKey(struct TWTezosAddress *_Nonnull address, struct TWPublicKey publicKey) {
    std::array<std::uint8_t, 3> prefix {6, 161, 159};
    TWData *data = TWDataCreateWithSize(prefix.size());
    for (size_t i = 0; i < prefix.size(); i++) {
        TWDataSet(data, i, prefix[i]);
    }

    TWData *publicKeyData = TWPublicKeyData(publicKey);
    TWData *publicKeyHash = TWHashBlake2b(publicKeyData, 20);
    TWDataAppendData(data, publicKeyHash);
    TWString *addressString = TWBase58Encode(data);

    return TWTezosAddressInitWithString(address, addressString);
}

TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address) {
    TWData *addressData = TWTezosAddressData(address);
    return TWBase58EncodeNoCheck(addressData);
}

TWData *_Nonnull TWTezosAddressData(struct TWTezosAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address -> bytes, TWTezosAddressSize);
}