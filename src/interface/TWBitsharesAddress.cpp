// Copyright � 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitsharesAddress.h>

#include "../Data.h"
#include "../Bitshares/Address.h"
#include "../Bravo/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::Bitshares;
using Address = Bravo::Address;

bool TWBitsharesAddressEqual(struct TWBitsharesAddress *_Nonnull lhs, struct TWBitsharesAddress *_Nonnull rhs) {
	return lhs->impl == rhs->impl;
}

bool TWBitsharesAddressIsValidString(TWString *_Nonnull string) {
	auto s = reinterpret_cast<const std::string *>(string);
	return Address::isValid(*s, { AddressPrefix });
}

struct TWBitsharesAddress *_Nullable TWBitsharesAddressCreateWithString(TWString *_Nonnull string) {
	auto s = reinterpret_cast<const std::string*>(string);

	try {
		return new TWBitsharesAddress{ Address(*s, { AddressPrefix }) };
	}
	catch (...) {
		return nullptr;
	}
}

struct TWBitsharesAddress *_Nonnull TWBitsharesAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
		return new TWBitsharesAddress{ Address(publicKey->impl, AddressPrefix) };
}

struct TWBitsharesAddress *_Nullable TWBitsharesAddressCreateWithKeyHash(TWData *_Nonnull keyHash) {
	auto d = reinterpret_cast<const Data *>(keyHash);
	try {
		return new TWBitsharesAddress{ Address(*d, AddressPrefix) };
	}
	catch (...) {
		return nullptr;
	}
}

void TWBitsharesAddressDelete(struct TWBitsharesAddress *_Nonnull address) {
	delete address;
}

TWString *_Nonnull TWBitsharesAddressDescription(struct TWBitsharesAddress *_Nonnull address) {
	const auto string = address->impl.string();
	return TWStringCreateWithUTF8Bytes(string.c_str());
}