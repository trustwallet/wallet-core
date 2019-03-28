// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSteemAddress.h>

#include "../Data.h"
#include "../Steem/Address.h"
#include "../Bravo/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <memory>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::Bravo;

bool TWSteemAddressEqual(struct TWSteemAddress *_Nonnull lhs, struct TWSteemAddress *_Nonnull rhs) {
	return lhs->impl == rhs->impl;
}

bool TWSteemAddressIsValidString(TWString *_Nonnull string) {
	auto s = reinterpret_cast<const std::string *>(string);
	return Address::isValid(*s, { TWSteemMainnetPrefix, TWSteemTestnetPrefix });
}

struct TWSteemAddress *_Nullable TWSteemAddressCreateWithString(TWString *_Nonnull string) {
	auto s = reinterpret_cast<const std::string*>(string);

	try {
		return new TWSteemAddress{ Address(*s, { TWSteemMainnetPrefix, TWSteemTestnetPrefix }) };
	}
	catch (...) {
		return nullptr;
	}
}

struct TWSteemAddress *_Nonnull TWSteemAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, TWSteemAddressType type) {
		return new TWSteemAddress{ Address(publicKey->impl, (type == TWSteemAddressType::TWSteemAddressTypeMainNet)? TWSteemMainnetPrefix : TWSteemTestnetPrefix) };
}

struct TWSteemAddress *_Nullable TWSteemAddressCreateWithKeyHash(TWData *_Nonnull keyHash, TWSteemAddressType type) {
	auto d = reinterpret_cast<const Data *>(keyHash);
	try {
		return new TWSteemAddress{ Address(*d, (type == TWSteemAddressType::TWSteemAddressTypeMainNet) ? TWSteemMainnetPrefix : TWSteemTestnetPrefix) };
	}
	catch (...) {
		return nullptr;
	}
}

void TWSteemAddressDelete(struct TWSteemAddress *_Nonnull address) {
	delete address;
}

TWString *_Nonnull TWSteemAddressDescription(struct TWSteemAddress *_Nonnull address) {
	const auto string = address->impl.string();
	return TWStringCreateWithUTF8Bytes(string.c_str());
}