// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTezosAddress.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <TrustWalletCore/TWBase58.h>
#include <TrustWalletCore/TWData.h>
=======
#include "../Tezos/Address.h"

>>>>>>> 72d04c8794778698cee13ac1974d402d73bd4cd7
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <string.h>
#include <memory>

using namespace TW;
using namespace TW::Tezos;

bool TWTezosAddressEqual(struct TWTezosAddress *_Nonnull lhs, struct TWTezosAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWTezosAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWTezosAddress *_Nullable TWTezosAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    const auto address = Address(*s);
    return new TWTezosAddress{ std::move(address) };
}

struct TWTezosAddress *_Nonnull TWTezosAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWTezosAddress{ Address(publicKey->impl) };
}

void TWTezosAddressDelete(struct TWTezosAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

<<<<<<< HEAD
TWData *_Nonnull TWTezosAddressData(struct TWTezosAddress *_Nonnull address) {
  return TWDataCreateWithBytes(address -> bytes, TWTezosAddressSize);
=======
#include "../Tezos/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <string.h>
#include <memory>

using namespace TW;
using namespace TW::Tezos;

bool TWTezosAddressEqual(struct TWTezosAddress *_Nonnull lhs, struct TWTezosAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWTezosAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWTezosAddress *_Nullable TWTezosAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    const auto address = Address(*s);
    return new TWTezosAddress{ std::move(address) };
}

struct TWTezosAddress *_Nonnull TWTezosAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWTezosAddress{ Address(publicKey->impl) };
}

void TWTezosAddressDelete(struct TWTezosAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWTezosAddressKeyHash(struct TWTezosAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
>>>>>>> Integrate Tezos Address functionality. (#93)
=======
TWData *_Nonnull TWTezosAddressKeyHash(struct TWTezosAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
>>>>>>> 72d04c8794778698cee13ac1974d402d73bd4cd7
}
